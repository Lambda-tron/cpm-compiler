#ifndef IR_H
#define IR_H

#include "Node.h"
#include "Symbol.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_map>
using namespace std;

//expressions
class TAC{
    string name,operand1,operand2,OP;
    public:
    TAC(){}
    TAC(const string& name,const string& operand1 = "" ,const string& OP = "",const string& operand2 ="")
    : name(name), operand1(operand1), operand2(operand2), OP(OP){}
    string getName() const { return name; }
    string getOperand1() const { return operand1; }
    string getOperand2() const { return operand2; }
    string getOP() const { return OP; }
    string to_str() {
        if (name == "print" || name == "return" || name == "read") {
            return name + " " + operand1;
        }
        else if (name == "goto") {
            return "goto " + operand1;
        }
        else if (name == "ifFalse") {
            return "ifFalse " + operand1 + " goto " + operand2;
        }
        else if (operand1 == "" && OP == "" && operand2 == "") {
            return name + ":";
        }
        else if (name == "arr_access") {
            return operand1 + "[" + OP + "] = " + operand2;
        }
        else if (OP == "new") {
            if (operand2.empty()) {
                return name + " := new " + operand1;
            }
            return name + " := new " + operand1 + "[" + operand2 + "]";
        }
        else if (OP == "access") {
            return name + " := " + operand1 + "[" + operand2 + "]";
        }
        else if (OP == "length") {
            return name + " := " + operand1 + ".length";
        }
        else if (!OP.empty() && operand2.empty()) {
            return name + " := " + OP + operand1;
        }
        else if (OP.empty()) {
            return name + " := " + operand1;
        }
        else {
            return name + " := " + operand1 + " " + OP + " " + operand2;
        }
    }
};



//statements
class BasicBlock{
    public:
    string name,type,relatedToClass;
    vector<TAC> TACs;
    
    //for the CFG
    TAC condition;
    BasicBlock *falseBlock = nullptr,*trueBlock = nullptr;
    
    BasicBlock(const string& name, BasicBlock* trueBlock = nullptr, BasicBlock* falseBlock = nullptr, const TAC& condition = TAC())
        : name(name), condition(condition), falseBlock(falseBlock), trueBlock(trueBlock) {}};


class IR{
    private:
        unordered_map<string,vector<const Node*>> classAttributes;
        vector<BasicBlock*> blocks;
        BasicBlock* currentBB = nullptr,*rootBB = nullptr;
        string currentClassName = "";
    public:
        IR(const Node* node, const ST* symbolTable){
            traverse(node);
            printIR();
            generateDot();
        }

        void traverse(const Node* n) {
            bool openScope = false;
            //create a block
            if (n->type == "class") {
                currentClassName = n->value;
                classAttributes.insert({currentClassName, vector<const Node*>()});
                openScope = true;
            }else if (n->type == "method") {
                openScope = true;
                string blockName = n->value;



                BasicBlock* block = new BasicBlock(blockName);
                if (currentClassName != "") {
                    block->relatedToClass = currentClassName;
                }
                block->type = "method";
                blocks.push_back(block);
                currentBB = block;

                if (rootBB == nullptr) {
                    rootBB = block;
                }

            }  
            //We are currently in class and collecting their attributes
            bool isClassAttributes = currentClassName != "" && currentBB == nullptr;
            
            if (n->type == "v_var" || n->type == "var"){

                if(isClassAttributes){
                    classAttributes[currentClassName].push_back(n);
                    return;
                }

                auto it = n->children.begin();
                Node* lhs = *it++;
                Node* rhs = nullptr;
                if (it != n->children.end()) {
                    rhs = *it;
                }

                if(lhs->value == "arr_INT" || lhs->value == "arr_FLOAT"){
                    if(rhs){
                        auto itr = rhs->children.begin();
                        itr++;
                        Node* elements = *itr;

                        currentBB->TACs.push_back(
                            TAC(n->value, lhs->value, "new", to_string(elements->children.size()))
                        );
                    }
                }else{
                    currentBB->TACs.push_back(
                        TAC(n->value, lhs->value, "new")
                    );

                    if (rhs) {
                        if(rhs->type == "call"){
                            genTACS(rhs, lhs->value, n->value);
                        }else{
                            currentBB->TACs.push_back(
                                TAC(n->value, genTACS(rhs))
                            );
                        }
                    }
                }

                return;
            }else if(n->type == "print" || n->type == "return" || n->type == "read"){
                auto it = n->children.begin();
                Node* lhs = *it;
                string returnedVariable = genTACS(lhs);
                currentBB->TACs.push_back(TAC(n->type,returnedVariable));
                return;
            }else if(n->type == "assign"){
                auto it = n->children.begin();
                Node* lhs = *it++;
                Node* rhs = *it;

                if(lhs->type == "postfix"){
                    auto it2 = lhs->children.begin(); 
                    Node* post_lhs = *it2++; 
                    Node* post_rhs = *it2;

                    if(post_rhs->type == "arr_access"){
                        Node* index = *post_rhs->children.begin(); 
                        currentBB->TACs.push_back(
                            TAC("arr_access", genTACS(post_lhs), index->value, genTACS(rhs))
                        );
                    }

                }else{
                    currentBB->TACs.push_back(
                        TAC(genTACS(lhs), genTACS(rhs))
                    );
                }

                return;   
            }else if(n->type == "FOR"){
                auto it = n->children.begin();

                //grap all the child nodes in the FOR node.
                Node* initNode = nullptr;
                Node* condNode = nullptr;
                Node* updateNode = nullptr;
                Node* stmtBlockNode = nullptr;

                if(it != n->children.end()) initNode = *it++;
                if(it != n->children.end()) condNode = *it++;
                if(it != n->children.end()) updateNode = *it++;
                if(it != n->children.end()) stmtBlockNode = *it++;

                //get names for each block that will be created.
                string isStmtName = genStr(3);
                string trueBlockName = genStr(3);
                string falseBlockName = genStr(3);

                //IfFalse stmt GOTO trueBlock <-- if the statement is true we break out.
                BasicBlock* trueBlock = new BasicBlock(trueBlockName);
                //IfFalse stmt GOTO trueBlock <-- if the statement is false go into the body of hte loop.
                BasicBlock* falseBlock = new BasicBlock(falseBlockName);
                BasicBlock* ifStmtBlock = nullptr;
                //push the TAC of the index assignment/init.
                if(initNode){
                    traverse(initNode);
                }

                //create a new block for the if statement of the forloop.
                if(condNode && condNode->type == "relop"){
                    //the bool variable final name in TAC.
                    string boolVar = genTACS(condNode);
                    //create a TAC condition for the if statement for the loop.
                    TAC stmt("ifFalse", boolVar, "goto", trueBlockName);
                    //create the block
                    ifStmtBlock = new BasicBlock(isStmtName,trueBlock,falseBlock,stmt);
                    //the current block will now point to the if block.
                    currentBB->trueBlock = ifStmtBlock;
                    //the false block will also point to the if block to loop.
                    //cout << "BLOCK: " << falseBlock->name << " WILL GO TO " << ifStmtBlock->name <<endl;
                    //falseBlock->trueBlock = ifStmtBlock;
                }

                //now build the body of the loop.
                if(stmtBlockNode && stmtBlockNode->type == "Stmtblock"){
                    //get into the block of the loop body
                    currentBB = falseBlock;
                    //push the children as TACs in the block.
                    Node* stmtList = *stmtBlockNode->children.begin();
                    traverse(stmtList);
                }

                //incase we left the false block during the traversal
                //also push the index increments as TACs.
                if(updateNode){
                    traverse(updateNode);
                }

                if(currentBB && ifStmtBlock){
                    currentBB->trueBlock = ifStmtBlock;
                }

                currentBB = trueBlock;
                return;

            }else if(n->type == "IF"){
                //access the children of the IF body
                auto it = n->children.begin();

                Node* relopNode = nullptr;
                Node* ifStmt = nullptr;
                Node* elseStmt = nullptr;

                if(it != n->children.end()) relopNode = *it++;
                if(it != n->children.end()) ifStmt = *it++;
                if(it != n->children.end()) elseStmt = *it++;

                //name the blocks
                string relopNodeName = genStr(3);
                string trueBlockName = genStr(3);
                string falseBlockName = genStr(3);
                string continueBlockName = genStr(3);
                
                //the exitblock body
                BasicBlock* continueBlock = new BasicBlock(continueBlockName);
                //Else statement block.
                BasicBlock* trueBlock = new BasicBlock(trueBlockName,continueBlock);
                //If statement body.
                BasicBlock* falseBlock = new BasicBlock(falseBlockName,continueBlock);

                //build the relop block that will have two conditions
                if(relopNode && relopNode->type == "relop"){
                    string boolVar = genTACS(relopNode);
                    
                    BasicBlock* ifStmtBlock = nullptr;
                    //if there is an else statement then if its false we go to else, if there is not, we go the continueBlock
                    if(n->value=="IFELSE"){
                        //if there is an ELSE statement then we go there if the if statement is false
                        TAC cond ("ifFalse", boolVar, "goto", trueBlockName);
                        ifStmtBlock = new BasicBlock(relopNodeName,trueBlock,falseBlock,cond);
                    }else{
                        //if there is NO ELSE statement then we go to the rest of the code
                        TAC cond("ifFalse", boolVar, "goto", continueBlockName);
                        ifStmtBlock = new BasicBlock(relopNodeName,continueBlock,falseBlock,cond);
                    }
                    currentBB->trueBlock = ifStmtBlock;
                }

                //fill the body of the if stmt
                if(ifStmt && ifStmt->type == "Stmtblock"){
                    currentBB = falseBlock;
                    Node* stmtList = *ifStmt->children.begin();
                    traverse(stmtList);
                }
                
                if(currentBB && continueBlock){
                    currentBB->trueBlock = continueBlock;
                }

                //fill the body of the else stmt
                if(n->value=="IFELSE" && elseStmt && elseStmt->type == "Stmtblock"){
                    currentBB = trueBlock;
                    Node* stmtList = *elseStmt->children.begin();
                    traverse(stmtList);
                }

                if(currentBB && continueBlock){
                    currentBB->trueBlock = continueBlock;
                }
                //leave to the rest of the code
                currentBB = continueBlock;
                return;
            }else if(n->type == "param"){
                auto it = n->children.begin();
                Node* lhs = *it++;
                Node* rhs = nullptr;
                if (it != n->children.end()) {
                    rhs = *it;
                }

                currentBB->TACs.push_back(
                    TAC(n->value, lhs->value, "param")
                );

                if (rhs) {
                    currentBB->TACs.push_back(
                        TAC(n->value, genTACS(rhs))
                    );
                }
                return;
            }else if (n->type == "args"){
                for (Node* child : n->children) {
                    string argName = genTACS(child);
                    currentBB->TACs.push_back(
                        TAC("param", argName)
                    );
                }
                return;
            }
            
            for (Node* child : n->children) {
                traverse(child);
            }
            
            if(openScope && n->type == "class"){
                currentClassName = "";
            }else if (openScope && n->type == "method"){
                currentBB = nullptr;
            }

        }

        string getArithmeticSymbol(const string& type) {
            if (type == "PLUSOP") return "+";
            if (type == "MULTOP") return "*";
            if (type == "DIVOP")  return "/";
            if (type == "MINOP")  return "-";
            if (type == "PWROP")  return "^";
            return "";
        }


        string genStr(const int& len) {
            static const char alphanum[] =
                "0"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
            std::string tmp_s;
            tmp_s.reserve(len);

            for (int i = 0; i < len; ++i) {
                tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
            }
            
            return tmp_s;
        }

        string genTACS(const Node* n, const string& className = "", const string& varName = "") {

            // literals and identifiers
            if (n->children.empty()) {
                return n->value;
            }

            if(n->type == "ID"){
                return  n->value;
            }

            // arithmetic
            if (isArthemticOP(n->type)) {
                return arthmetics(n);
            }

            // relational
            if (n->type == "relop") {
                auto it = n->children.begin(); 
                Node* lhs = *it++; 
                Node* rhs = *it; 
                string str_lhs,str_rhs; 
                str_lhs = genTACS(lhs); 
                str_rhs = genTACS(rhs); 
                string name = genStr(2); 
                currentBB->TACs.push_back(TAC(name, str_lhs, n->value, str_rhs));
                return name;

            }

            //postfix like accessing object methods or .length or arrayaccess
            if(n->type == "postfix"){
                return getPosfixTac(n);
            }

            // unary not
            if (n->type == "not") {
                Node* child = *n->children.begin();
                string value = genTACS(child);
                string tmp = genStr(2);
                currentBB->TACs.push_back(TAC(tmp, value, "!"));                
                return tmp;
            }

            if(n->type == "call"){
                if(currentClassName == "" && className != "" && currentBB != nullptr){
                    currentBB->TACs.push_back(
                        TAC(varName, className, "new")
                    );
                    return varName;
                }else{
                    string tmp = genStr(2);
                    currentBB->TACs.push_back(
                        TAC(tmp, n->value, "call", "0")
                    );
                    return tmp;
                }
            }

            // fallback
            return n->value;
        }

        string getPosfixTac(const Node* n){
            auto it = n->children.begin(); 
            Node* lhs = *it++; 
            Node* rhs = *it;
            string genName = genStr(2);
            
            string lsh_TAC_store = genTACS(lhs);
            //push y.length TAC -> y length
            if(rhs->type == "length"){
                currentBB->TACs.push_back(
                    TAC(genName, lsh_TAC_store, "length")
                );

            }else if(rhs->type == "arr_access"){
                Node* index = *rhs->children.begin(); 
                currentBB->TACs.push_back(
                    TAC(genName, lsh_TAC_store, "access", index->value)
                );

            }else if(rhs->type == "funcMethcall"){
                Node* args = *rhs->children.begin();
                traverse(args);
                currentBB->TACs.push_back(
                    TAC(genName, rhs->value, "call", to_string(args->children.size()))
                );
            }
            return genName;      
        }

        bool isBuiltinType(const string& type) {
            return type == "INT"
            || type == "BOOL"
            || type == "FLOAT"
            || type == "arr_INT"
            || type == "arr_FLOAT";
        }

        string arthmetics(const Node* n,const string& initName = ""){
            if (n->children.empty()) {
                return n->value;
            }
            string name = genStr(2);
            
            auto it = n->children.begin();
            Node* lhs = *it++;
            Node* rhs = *it;

            string str_lhs,str_rhs;
            str_lhs = arthmetics(lhs);
            str_rhs = arthmetics(rhs);
            
            string op = getArithmeticSymbol(n->type);
            if(initName != ""){
                name = initName;
            }
            currentBB->TACs.push_back(TAC(name,str_lhs,op,str_rhs));
            
            return name;
        }

        bool isArthemticOP(const string& op){
            return op == "PLUSOP" 
            || op == "MULTOP"
            || op == "DIVOP"
            || op == "MINOP"
            || op == "PWROP";
        }

        void printIR() {
            if (blocks.empty()) return;

            vector<BasicBlock*> pending;
            vector<BasicBlock*> visited;

            for (BasicBlock* b : blocks) {
                if (b && b->type == "method") {
                    pending.push_back(b);
                }
            }

            while (!pending.empty()) {
                BasicBlock* block = pending.back();
                pending.pop_back();

                bool alreadyVisited = false;
                for (BasicBlock* b : visited) {
                    if (b == block) {
                        alreadyVisited = true;
                        break;
                    }
                }
                if (alreadyVisited) continue;

                visited.push_back(block);

                cout << "========================================" << endl;
                cout << "Block: " << block->name << endl;
                if (!block->relatedToClass.empty()) {
                    cout << "Class: " << block->relatedToClass << endl;
                }
                cout << "----------------------------------------" << endl;

                bool hasCondition = false;
                if (block->condition.to_str() != ":") {
                    hasCondition = true;
                    cout << "  condition: " << block->condition.to_str() << endl;
                }

                if (hasCondition && !block->TACs.empty()) {
                    cout << "----------------------------------------" << endl;
                }

                if (!block->TACs.empty()) {
                    for (TAC& code : block->TACs) {
                        cout << "  " << code.to_str() << endl;
                    }
                } else if (!hasCondition) {
                    cout << "  <empty>" << endl;
                }

                cout << "----------------------------------------" << endl;
                cout << "  true  -> " << (block->trueBlock ? block->trueBlock->name : "null") << endl;
                cout << "  false -> " << (block->falseBlock ? block->falseBlock->name : "null") << endl;
                cout << "========================================" << endl << endl;

                if (block->falseBlock) pending.push_back(block->falseBlock);
                if (block->trueBlock) pending.push_back(block->trueBlock);
            }
        }

        void generateDot(const string& filename = "cfg.dot") {
            if (blocks.empty()) return;

            ofstream file(filename);
            if (!file.is_open()) {
                cerr << "Could not open " << filename << endl;
                return;
            }

            file << "digraph CFG {\n";
            file << "    node [shape=box];\n";

            vector<BasicBlock*> pending;
            vector<BasicBlock*> visited;
            unordered_map<BasicBlock*, string> ids;

            int count = 0;

            for (BasicBlock* b : blocks) {
                if (b && b->type == "method") {
                    pending.push_back(b);
                }
            }

            while (!pending.empty()) {
                BasicBlock* block = pending.back();
                pending.pop_back();

                bool alreadyVisited = false;
                for (BasicBlock* b : visited) {
                    if (b == block) {
                        alreadyVisited = true;
                        break;
                    }
                }
                if (alreadyVisited) continue;

                visited.push_back(block);
                ids[block] = "n" + to_string(count++);

                string label = "Block: " + block->name + "\\l";

                if (!block->relatedToClass.empty()) {
                    label += "Class: " + block->relatedToClass + "\\l";

                    auto itAttr = classAttributes.find(block->relatedToClass);
                    if (itAttr != classAttributes.end()) {
                        if (!itAttr->second.empty()) {
                            label += "Attributes:\\l";

                            for (const Node* attr : itAttr->second) {
                                string attrLine = attr->value;

                                auto itChild = attr->children.begin();
                                if (itChild != attr->children.end()) {
                                    Node* typeNode = *itChild;
                                    attrLine += " : " + typeNode->value;
                                }

                                auto itInit = attr->children.begin();
                                if (itInit != attr->children.end()) {
                                    ++itInit;
                                    if (itInit != attr->children.end()) {
                                        Node* initNode = *itInit;
                                        if (initNode->children.empty()) {
                                            attrLine += " := " + initNode->value;
                                        } else {
                                            attrLine += " := ...";
                                        }
                                    }
                                }

                                label += attrLine + "\\l";
                            }
                        }
                    }
                }

                if (block->condition.to_str() != ":") {
                    label += "condition: " + block->condition.to_str() + "\\l";
                }

                for (TAC& code : block->TACs) {
                    label += code.to_str() + "\\l";
                }

                if (block->TACs.empty() && block->condition.to_str() == ":") {
                    label += "<empty>\\l";
                }

                string safeLabel = "";
                for (char c : label) {
                    if (c == '"') safeLabel += "\\\"";
                    else safeLabel += c;
                }

                file << "    " << ids[block] << " [label=\"" << safeLabel << "\"];\n";

                if (block->trueBlock) pending.push_back(block->trueBlock);
                if (block->falseBlock) pending.push_back(block->falseBlock);
            }

            for (BasicBlock* block : visited) {
                if (block->trueBlock) {
                    file << "    " << ids[block] << " -> " << ids[block->trueBlock] << " [label=\"true\"];\n";
                }
                if (block->falseBlock) {
                    file << "    " << ids[block] << " -> " << ids[block->falseBlock] << " [label=\"false\"];\n";
                }
            }

            file << "}\n";
            file.close();
        }

};

#endif