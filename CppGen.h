#ifndef CPPGEN_H
#define CPPGEN_H

#include "ir.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class CppGen{
private:
    vector<BasicBlock*> blocks;
    unordered_map<string, vector<const Node*>> classAttributes;
    vector<string> pendingParams;

public:
    CppGen(const vector<BasicBlock*>& irBlocks, const unordered_map<string, vector<const Node*>>& attrs){
        this->classAttributes = attrs;
        this->blocks = irBlocks;
        generateCPP();
    }

    void generateCPP(const string& filename = "out.cpp"){
        ofstream file(filename);
        if(!file.is_open()){
            cerr << "Could not open " << filename << endl;
            return;
        }

        file << "#include <iostream>\n";
        file << "#include <vector>\n";
        file << "using namespace std;\n\n";

        //write attrs+method bodies
        writeClasses(file);
        //write main
        writeFreeFunctions(file);

        file.close();
    }

private:
    //get inbuilt type
    string getCppType(const string& typeName){
        if(typeName == "INT") return "int";
        if(typeName == "FLOAT") return "float";
        if(typeName == "BOOL") return "bool";
        if(typeName == "arr_INT") return "vector<int>";
        if(typeName == "arr_FLOAT") return "vector<float>";
        return typeName;
    }

    //compare operation
    string cppOp(const string& op){
        if(op == "=") return "==";
        return op;
    }

    // returns the element type of a vector type
    string elementType(const string& vecType){
        if(vecType == "vector<float>") return "float";
        if(vecType == "vector<int>")   return "int";
        return "int";
    }

    // varTypes lets us propagate float through access/arithmetic instead of blindly returning int
    string inferTacType(const TAC& tac, const unordered_map<string,string>& varTypes){
        string op  = tac.getOP();
        string op1 = tac.getOperand1();
        string op2 = tac.getOperand2();

        if(op == "new"){
            return getCppType(op1);
        }
        if(op == "access"){
            auto it = varTypes.find(op1);
            if(it != varTypes.end()) return elementType(it->second);
            return "int";
        }
        if(op == "length") return "int";
        if(op == "call")   return "int";
        if(op == "!")      return "bool";
        if(op == "=" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=") return "bool";

        // arithmetic propagate float if either operand is known float
        auto it1 = varTypes.find(op1);
        auto it2 = varTypes.find(op2);
        if((it1 != varTypes.end() && it1->second == "float") ||
           (it2 != varTypes.end() && it2->second == "float")){
            return "float";
        }

        return "int";
    }


    void writeFunctionDeclarations(ofstream& file, BasicBlock* root){
        // collect all reachable blocks first
        vector<BasicBlock*> pending;
        vector<BasicBlock*> visited;

        if(!root) return;
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

        vector<string> declared;
        vector<pair<string,string>> decls;
        unordered_map<string,string> varTypes;

        for(const auto& p : root->params){
            declared.push_back(p.second);
            varTypes[p.second] = getCppType(p.first);
        }

        if(!root->relatedToClass.empty()){
            auto it = classAttributes.find(root->relatedToClass);
            if(it != classAttributes.end()){
                for(const Node* attr : it->second){
                    declared.push_back(attr->value);
                }
            }
        }

        for(BasicBlock* block : visited){
            for(const TAC& tac : block->TACs){
                string name = tac.getName();
                string op   = tac.getOP();

                //ignore these TACs
                if(name == "print" || name == "read" || name == "return" || name == "goto" || name == "arr_access" || name == "param" || name.empty()){
                    continue;
                }

                
                string inferredType = inferTacType(tac, varTypes);
                varTypes[name] = inferredType;  // keep map up to date for downstream TACs

                bool seen = false;
                for(const string& d : declared){
                    if(d == name){
                        seen = true; 
                        break; 
                    }
                }
                if(seen || op.empty()) continue;

                decls.push_back({inferredType, name});
                declared.push_back(name);
            }
        }

        for(const auto& d : decls){
            file << "    " << d.first << " " << d.second << ";\n";
        }

        if(!decls.empty()){
            file << "\n";
        }
    }

    //print the class + attributes + method headers after that print the method declerations
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

    //print the main function body
    void writeFreeFunctions(ofstream& file){
        for(BasicBlock* block : blocks){
            if(!block || block->type != "method") continue;
            if(!block->relatedToClass.empty()) continue;

            writeMethodDefinition(file, block);
        }
    }

    //print class attributes
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

    //only the method header
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

    //write full function definition
    void writeMethodDefinition(ofstream& file, BasicBlock* block){
        pendingParams.clear();
        //method header
        writeMethodSignature(file, block, false);
        file << "{\n";
        //function deleration and declare needed variables
        writeFunctionDeclarations(file, block);
        writeMethodCFG(file, block);
        file << "}\n\n";
    }

    //print labels and TACs
    void writeMethodCFG(ofstream& file, BasicBlock* root){

        vector<BasicBlock*> pending;
        vector<BasicBlock*> visited;
        unordered_map<BasicBlock*, string> labels;

        int count = 0;
        pending.push_back(root);

        //get all the lables & branches
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

        //write TACs for each block
        for(BasicBlock* block : visited){
            file << labels[block] << ":\n";

            for(const TAC& tac : block->TACs){
                writeTAC(file, tac);
            }

            //conditional jumps
            if(block->condition.getName() == "ifFalse"){
                if(block->falseBlock){
                    file << "    if(" << block->condition.getOperand1() << ") goto " << labels[block->falseBlock] << ";\n";
                }
                if(block->trueBlock){
                    file << "    goto " << labels[block->trueBlock] << ";\n";
                }
            }else{
                //unconditional jumps
                if(block->trueBlock){
                    file << "    goto " << labels[block->trueBlock] << ";\n";
                }
            }

            file << "\n";
        }
    }
    //print TACs based on operation/name
    void writeTAC(ofstream& file, const TAC& tac){
        string name = tac.getName();
        string op1  = tac.getOperand1();
        string op2  = tac.getOperand2();
        string op   = tac.getOP();

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

        file << "    " << name << " = " << op1 << " " << cppOp(op) << " " << op2 << ";\n";
    }
};

#endif