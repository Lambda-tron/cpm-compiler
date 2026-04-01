#ifndef CODEGEN_H
#define CODEGEN_H

#include "ir.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>

using namespace std;

class CodeGen{
private:
    vector<BasicBlock*> blocks;
    unordered_map<string, vector<const Node*>> classAttributes;
    vector<string> pendingParams;

public:
    CodeGen(const vector<BasicBlock*>& irBlocks,
            const unordered_map<string, vector<const Node*>>& attrs)
        : blocks(irBlocks), classAttributes(attrs) {}

    void generateCPP(const string& filename = "out.cpp"){
        ofstream file(filename);
        if(!file.is_open()){
            cerr << "Could not open " << filename << endl;
            return;
        }

        file << "#include <iostream>\n";
        file << "#include <vector>\n";
        file << "#include <type_traits>\n";
        file << "using namespace std;\n\n";

        writeClasses(file);
        writeFreeFunctions(file);

        file.close();
    }

private:
    string getCppType(const string& typeName){
        if(typeName == "INT") return "int";
        if(typeName == "FLOAT") return "float";
        if(typeName == "BOOL") return "bool";
        if(typeName == "arr_INT") return "vector<int>";
        if(typeName == "arr_FLOAT") return "vector<float>";
        return typeName;
    }

    vector<BasicBlock*> collectReachableBlocks(BasicBlock* root){
        vector<BasicBlock*> pending;
        vector<BasicBlock*> visited;

        if(!root) return visited;
        pending.push_back(root);

        while(!pending.empty()){
            BasicBlock* block = pending.back();
            pending.pop_back();

            bool alreadyVisited = false;
            for(BasicBlock* b : visited){
                if(b == block){
                    alreadyVisited = true;
                    break;
                }
            }
            if(alreadyVisited) continue;

            visited.push_back(block);

            if(block->trueBlock) pending.push_back(block->trueBlock);
            if(block->falseBlock) pending.push_back(block->falseBlock);
        }

        return visited;
    }

    bool isAlreadyDeclared(const unordered_set<string>& declared, const string& name){
        return declared.find(name) != declared.end();
    }

    bool shouldIgnoreGeneratedName(const string& name){
        return name.empty()
            || name == "print"
            || name == "read"
            || name == "return"
            || name == "goto"
            || name == "arr_access";
    }

    string inferTacType(const TAC& tac, const vector<string>& currentPendingParams){
        string op1 = tac.getOperand1();
        string op2 = tac.getOperand2();
        string op = tac.getOP();

        if(op == "new"){
            return getCppType(op1);
        }

        if(op == "access"){
            return "std::remove_reference_t<decltype(" + op1 + "[" + op2 + "])>";
        }

        if(op == "length"){
            return "decltype(" + op1 + ".size())";
        }

        if(op == "call"){
            int expectedArgs = 0;
            if(!op2.empty()){
                expectedArgs = stoi(op2);
            }

            string args;
            for(int i = 0; i < expectedArgs && i < (int)currentPendingParams.size(); i++){
                if(i > 0) args += ", ";
                args += currentPendingParams[i];
            }

            return "decltype(" + op1 + "(" + args + "))";
        }

        if(op == "!"){
            return "decltype(!" + op1 + ")";
        }

        if(!op.empty()){
            return "decltype(" + op1 + " " + op + " " + op2 + ")";
        }

        return "";
    }

    void writeFunctionDeclarations(ofstream& file, BasicBlock* root){
        vector<BasicBlock*> visited = collectReachableBlocks(root);
        unordered_set<string> declared;
        vector<pair<string,string>> decls;
        vector<string> simulatedPendingParams;

        for(const auto& p : root->params){
            declared.insert(p.second);
        }

        if(!root->relatedToClass.empty()){
            auto it = classAttributes.find(root->relatedToClass);
            if(it != classAttributes.end()){
                for(const Node* attr : it->second){
                    declared.insert(attr->value);
                }
            }
        }

        for(BasicBlock* block : visited){
            for(const TAC& tac : block->TACs){
                string name = tac.getName();
                string op = tac.getOP();

                if(name == "param"){
                    simulatedPendingParams.push_back(tac.getOperand1());
                    continue;
                }

                if(shouldIgnoreGeneratedName(name)){
                    continue;
                }

                bool introducesVariable = false;

                if(op == "new" || op == "access" || op == "length" || op == "call" || op == "!"){
                    introducesVariable = true;
                }else if(!op.empty()){
                    introducesVariable = true;
                }

                if(introducesVariable && !isAlreadyDeclared(declared, name)){
                    string inferredType = inferTacType(tac, simulatedPendingParams);
                    if(!inferredType.empty()){
                        decls.push_back({inferredType, name});
                        declared.insert(name);
                    }
                }

                if(op == "call"){
                    simulatedPendingParams.clear();
                }
            }
        }

        for(const auto& d : decls){
            file << "    " << d.first << " " << d.second << ";\n";
        }

        if(!decls.empty()){
            file << "\n";
        }
    }

    void writeClasses(ofstream& file){
        for(const auto& entry : classAttributes){
            const string& className = entry.first;
            const vector<const Node*>& attrs = entry.second;

            file << "class " << className << "{\n";
            file << "public:\n";

            for(const Node* attr : attrs){
                writeClassAttribute(file, attr);
            }

            if(!attrs.empty()){
                file << "\n";
            }

            for(BasicBlock* block : blocks){
                if(block && block->type == "method" && block->relatedToClass == className){
                    file << "    ";
                    writeMethodSignature(file, block, true);
                    file << ";\n";
                }
            }

            file << "};\n\n";
        }

        for(BasicBlock* block : blocks){
            if(block && block->type == "method" && !block->relatedToClass.empty()){
                writeMethodDefinition(file, block);
            }
        }
    }

    void writeFreeFunctions(ofstream& file){
        for(BasicBlock* block : blocks){
            if(!block || block->type != "method") continue;
            if(!block->relatedToClass.empty()) continue;

            writeMethodDefinition(file, block);
        }
    }

    void writeClassAttribute(ofstream& file, const Node* attr){
        auto it = attr->children.begin();
        if(it == attr->children.end()) return;

        Node* typeNode = *it++;
        Node* initNode = nullptr;
        if(it != attr->children.end()){
            initNode = *it;
        }

        string cppType = getCppType(typeNode->value);
        file << "    " << cppType << " " << attr->value;

        if(initNode && initNode->children.empty()){
            file << " = " << initNode->value;
        }

        file << ";\n";
    }

    void writeMethodSignature(ofstream& file, BasicBlock* block, bool insideClass){
        string returnType = "int";
        if(!block->returnType.empty()){
            returnType = getCppType(block->returnType);
        }

        if(block->name == "main"){
            file << "int main()";
            return;
        }

        if(insideClass){
            file << returnType << " " << block->name << "(";
        }else{
            if(block->relatedToClass.empty()){
                file << returnType << " " << block->name << "(";
            }else{
                file << returnType << " " << block->relatedToClass << "::" << block->name << "(";
            }
        }

        for(size_t i = 0; i < block->params.size(); i++){
            file << getCppType(block->params[i].first) << " " << block->params[i].second;
            if(i + 1 < block->params.size()){
                file << ", ";
            }
        }

        file << ")";
    }

    void writeMethodDefinition(ofstream& file, BasicBlock* block){
        pendingParams.clear();
        writeMethodSignature(file, block, false);
        file << "{\n";
        writeFunctionDeclarations(file, block);
        writeMethodCFG(file, block);
        file << "}\n\n";
    }

    void writeMethodCFG(ofstream& file, BasicBlock* root){
        vector<BasicBlock*> pending;
        vector<BasicBlock*> visited;
        unordered_map<BasicBlock*, string> labels;

        int count = 0;
        pending.push_back(root);

        while(!pending.empty()){
            BasicBlock* block = pending.back();
            pending.pop_back();

            bool alreadyVisited = false;
            for(BasicBlock* b : visited){
                if(b == block){
                    alreadyVisited = true;
                    break;
                }
            }
            if(alreadyVisited) continue;

            visited.push_back(block);
            labels[block] = "L" + to_string(count++);

            if(block->trueBlock) pending.push_back(block->trueBlock);
            if(block->falseBlock) pending.push_back(block->falseBlock);
        }

        for(BasicBlock* block : visited){
            file << labels[block] << ":\n";

            for(const TAC& tac : block->TACs){
                writeTAC(file, tac);
            }

            if(block->condition.getName() == "ifFalse"){
                if(block->falseBlock){
                    file << "    if(" << block->condition.getOperand1() << ") goto "
                         << labels[block->falseBlock] << ";\n";
                }
                if(block->trueBlock){
                    file << "    goto " << labels[block->trueBlock] << ";\n";
                }
            }else{
                if(block->trueBlock){
                    file << "    goto " << labels[block->trueBlock] << ";\n";
                }
            }

            file << "\n";
        }
    }

    void writeTAC(ofstream& file, const TAC& tac){
        string name = tac.getName();
        string op1 = tac.getOperand1();
        string op2 = tac.getOperand2();
        string op = tac.getOP();

        if(name == "print"){
            file << "    cout << " << op1 << " << endl;\n";
            return;
        }

        if(name == "read"){
            file << "    cin >> " << op1 << ";\n";
            return;
        }

        if(name == "return"){
            file << "    return " << op1 << ";\n";
            return;
        }

        if(name == "goto"){
            file << "    goto " << op1 << ";\n";
            return;
        }

        if(name == "param"){
            pendingParams.push_back(op1);
            return;
        }

        if(name == "arr_access"){
            file << "    " << op1 << "[" << op << "] = " << op2 << ";\n";
            return;
        }

        if(op == "new"){
            if(op1 == "arr_INT"){
                if(!op2.empty()){
                    file << "    " << name << " = vector<int>(" << op2 << ");\n";
                }
            }else if(op1 == "arr_FLOAT"){
                if(!op2.empty()){
                    file << "    " << name << " = vector<float>(" << op2 << ");\n";
                }
            }
            return;
        }

        if(op == "access"){
            file << "    " << name << " = " << op1 << "[" << op2 << "];\n";
            return;
        }

        if(op == "length"){
            file << "    " << name << " = " << op1 << ".size();\n";
            return;
        }

        if(op == "call"){
            int expectedArgs = 0;
            if(!op2.empty()){
                expectedArgs = stoi(op2);
            }

            file << "    " << name << " = " << op1 << "(";
            for(int i = 0; i < expectedArgs && i < (int)pendingParams.size(); i++){
                file << pendingParams[i];
                if(i + 1 < expectedArgs && i + 1 < (int)pendingParams.size()){
                    file << ", ";
                }
            }
            file << ");\n";

            pendingParams.clear();
            return;
        }

        if(op == "!"){
            file << "    " << name << " = !" << op1 << ";\n";
            return;
        }

        if(op.empty()){
            file << "    " << name << " = " << op1 << ";\n";
            return;
        }

        file << "    " << name << " = " << op1 << " " << op << " " << op2 << ";\n";
    }
};

#endif