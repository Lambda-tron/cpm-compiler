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

using namespace std;





//expressions
class TAC{
    string name,operand1,operand2,OP;
    public:
    TAC(const string& name,const string& operand1 = "" ,const string& OP = "",const string& operand2 ="")
    : name(name), operand1(operand1), operand2(operand2), OP(OP){}

    string to_str(){
        string text;
        if(this->name == "print" || this->name == "return"){
            text = this->name + " " + this->operand1 + this->OP + this->operand2 ;
        }else{
            text = this->name + " = " + this->operand1 + this->OP + this->operand2 ;
        }
        return text;
    }
};

//statements
class BasicBlock{
    public:
    string name;
    vector<TAC> TACs;
    BasicBlock* nextBlock = nullptr;
    BasicBlock(string name): name(name){}
};

class IR{
    private:
        BasicBlock* currentBB = nullptr;
        BasicBlock* rootBB = nullptr;

    public:
        IR(const Node* node, const ST* symbolTable){
            travers(node);
        }

        void travers(const Node* n, int depth = 0) {
            bool openScope = false;
            //create a block
            if (n->type == "method") {
                string blockName = n->type+":"+n->value;
                if(currentBB){
                    BasicBlock* block = new BasicBlock(blockName) ;
                    currentBB->nextBlock = block;
                    currentBB = block;
                }else{
                    BasicBlock* block = new BasicBlock(blockName) ;
                    currentBB = block;
                    rootBB = block;
                }
                
            }
            //convert to TAC
            else{

                if (n->type == "v_var" || n->type == "var"){
                    auto it = n->children.begin();
                    Node* lhs = *it++;
                    Node* rhs = nullptr;
                    if(n->children.size() > 1){
                        rhs = *it;
                    }
                    currentBB->TACs.push_back(TAC(n->value,"new","",lhs->value));
                    if(rhs){
                        string rhs_value = genTACS(rhs);
                        currentBB->TACs.push_back(TAC(n->value,rhs_value,"",""));  
                    }

                }else if(n->type == "print" || n->type == "return" || n->type == "read"){
                    auto it = n->children.begin();
                    Node* lhs = *it;
                    string returnedVariable = genTACS(lhs);
                    currentBB->TACs.push_back(TAC(n->type,returnedVariable));
                }
                else if(n->type == "assign"){
                    auto it = n->children.begin();
                    Node* lhs = *it++;
                    Node* rhs = *it;
                    string rhs_value = genTACS(rhs);
                    currentBB->TACs.push_back(TAC(lhs->value,rhs_value));  
                }
                else if(n->type == "FOR"){
                    
                }
            }
            
            for (Node* child : n->children) {
                travers(child, depth + 1);
            }
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

        string genTACS(const Node* n){
            //expression
            if(isArthemticOP(n->type)){
                return arthmetics(n);
            }
            //Unaryexpression !
            else if(n->type == "not"){
                string name = genStr(2);
                Node* child = *n->children.begin();
                string value = genTACS(child);
                currentBB->TACs.push_back(TAC(name,"","!",value));
                return name;
            }

            //methodcall
            /*else if(n->type == "call"){

            }*/
            return n->value;
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
            
            string op;
            if (n->type == "PLUSOP") op = "+";
            else if (n->type == "MULTOP") op = "*";
            else if (n->type == "DIVOP")  op = "/";
            else if (n->type == "MINOP")  op = "-";
            else if (n->type == "PWROP")  op = "^";
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

        void printIR(){
            BasicBlock* tempBlk = rootBB;
            while(tempBlk){
                cout << "Block: " << tempBlk->name << endl;
                for(TAC code: rootBB->TACs){
                    cout << "\t" <<code.to_str() << endl;
                }
                tempBlk = tempBlk->nextBlock;
            }
        }
};

#endif