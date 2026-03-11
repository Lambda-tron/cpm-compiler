#ifndef IR_H
#define IR_H

#include "Node.h"
#include "Symbol.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <unordered_map>
#include <cstdlib>

using namespace std;

// expressions
class TAC{
    string name, operand1, operand2, OP;

public:
    TAC() {}

    TAC(const string& name,
        const string& operand1 = "",
        const string& OP = "",
        const string& operand2 = "")
        : name(name), operand1(operand1), operand2(operand2), OP(OP) {}

    string getName() const { return name; }
    string getOperand1() const { return operand1; }
    string getOperand2() const { return operand2; }
    string getOP() const { return OP; }

    string to_str() const {
        if (name == "print" || name == "return" || name == "read") {
            return name + " " + operand1;
        } else if (name == "goto") {
            return "goto " + operand1;
        } else if (name == "ifFalse") {
            return "ifFalse " + operand1 + " goto " + operand2;
        } else if (operand1 == "" && OP == "" && operand2 == "") {
            return name + ":";
        } else if (name == "arr_access") {
            return operand1 + "[" + OP + "] = " + operand2;
        } else if (OP == "new") {
            if (operand2.empty()) {
                return name + " := new " + operand1;
            }
            return name + " := new " + operand1 + "[" + operand2 + "]";
        } else if (OP == "access") {
            return name + " := " + operand1 + "[" + operand2 + "]";
        } else if (OP == "length") {
            return name + " := " + operand1 + ".length";
        } else if (!OP.empty() && operand2.empty()) {
            return name + " := " + OP + operand1;
        } else if (OP.empty()) {
            return name + " := " + operand1;
        } else {
            return name + " := " + operand1 + " " + OP + " " + operand2;
        }
    }
};

// statements
class BasicBlock{
public:
    string name, type, relatedToClass;
    vector<TAC> TACs;

    TAC condition;
    BasicBlock *falseBlock = nullptr, *trueBlock = nullptr;

    // method metadata
    string returnType;
    vector<pair<string,string>> params; // {type, name}

    BasicBlock(const string& name,
               BasicBlock* trueBlock = nullptr,
               BasicBlock* falseBlock = nullptr,
               const TAC& condition = TAC())
        : name(name), condition(condition),
          falseBlock(falseBlock), trueBlock(trueBlock) {}
};

class IR{
private:
    unordered_map<string, vector<const Node*>> classAttributes;
    vector<BasicBlock*> blocks;
    BasicBlock* currentBB = nullptr;
    BasicBlock* rootBB = nullptr;
    string currentClassName = "";

public:
    IR(const Node* node, const ST* symbolTable){
        (void)symbolTable;
        traverse(node);
        printIR();
        generateDot();
    }

    void traverse(const Node* n){
        bool openScope = false;

        // create class / method block
        if (n->type == "class") {
            currentClassName = n->value;
            classAttributes.insert({currentClassName, vector<const Node*>()});
            openScope = true;

        } else if (n->type == "method") {
            openScope = true;
            string blockName = n->value;

            BasicBlock* block = new BasicBlock(blockName);
            block->type = "method";
            block->returnType = "INT";

            if (currentClassName != "") {
                block->relatedToClass = currentClassName;
            }

            // scan children: arg_method has params, type is return type
            for (Node* child : n->children) {
                if (child->type == "type") {
                    block->returnType = child->value;
                } else if (child->type == "arg_method") {
                    collectParamsFromArgMethod(child, block->params);
                }
            }

            blocks.push_back(block);
            currentBB = block;

            if (rootBB == nullptr) {
                rootBB = block;
            }
        }

        // currently inside class, collecting attributes
        bool isClassAttributes = currentClassName != "" && currentBB == nullptr;

        if (n->type == "v_var" || n->type == "var") {
            if (isClassAttributes) {
                classAttributes[currentClassName].push_back(n);
                return;
            }

            auto it = n->children.begin();
            Node* lhs = *it++;
            Node* rhs = nullptr;
            if (it != n->children.end()) {
                rhs = *it;
            }

            // arrays
            if (lhs->value == "arr_INT" || lhs->value == "arr_FLOAT") {
                if (rhs) {
                    auto itr = rhs->children.begin();
                    ++itr; // skip array type node
                    Node* elements = nullptr;
                    if (itr != rhs->children.end()) {
                        elements = *itr;
                    }

                    int sz = elements ? (int)elements->children.size() : 0;
                    currentBB->TACs.push_back(
                        TAC(n->value, lhs->value, "new", to_string(sz))
                    );

                    if (elements) {
                        int idx = 0;
                        for (Node* elem : elements->children) {
                            currentBB->TACs.push_back(
                                TAC("arr_access", n->value, to_string(idx), genTACS(elem))
                            );
                            idx++;
                        }
                    }
                } else {
                    currentBB->TACs.push_back(
                        TAC(n->value, lhs->value, "new")
                    );
                }
                return;
            }

            // object construction like: object : test := test()
            if (rhs && rhs->type == "call" && !isBuiltinType(lhs->value) && rhs->value == lhs->value) {
                currentBB->TACs.push_back(
                    TAC(n->value, lhs->value, "new")
                );
                return;
            }

            // normal variable declaration
            currentBB->TACs.push_back(
                TAC(n->value, lhs->value, "new")
            );

            if (rhs) {
                if (rhs->type == "call") {
                    string tmp = genTACS(rhs);
                    currentBB->TACs.push_back(
                        TAC(n->value, tmp)
                    );
                } else {
                    currentBB->TACs.push_back(
                        TAC(n->value, genTACS(rhs))
                    );
                }
            }

            return;

        } else if (n->type == "print" || n->type == "return" || n->type == "read") {
            auto it = n->children.begin();
            if (it != n->children.end()) {
                Node* lhs = *it;
                string returnedVariable = genTACS(lhs);
                currentBB->TACs.push_back(TAC(n->type, returnedVariable));
            }
            return;

        } else if (n->type == "assign") {
            auto it = n->children.begin();
            Node* lhs = *it++;
            Node* rhs = *it;

            if (lhs->type == "postfix") {
                auto it2 = lhs->children.begin();
                Node* post_lhs = *it2++;
                Node* post_rhs = *it2;

                if (post_rhs->type == "arr_access") {
                    Node* index = *post_rhs->children.begin();
                    currentBB->TACs.push_back(
                        TAC("arr_access", genTACS(post_lhs), index->value, genTACS(rhs))
                    );
                }
            } else {
                currentBB->TACs.push_back(
                    TAC(genTACS(lhs), genTACS(rhs))
                );
            }

            return;

        } else if (n->type == "FOR") {
            auto it = n->children.begin();

            Node* initNode = nullptr;
            Node* condNode = nullptr;
            Node* updateNode = nullptr;
            Node* stmtBlockNode = nullptr;

            if (it != n->children.end()) initNode = *it++;
            if (it != n->children.end()) condNode = *it++;
            if (it != n->children.end()) updateNode = *it++;
            if (it != n->children.end()) stmtBlockNode = *it++;

            string condBlockName = genStr(3);
            string exitBlockName = genStr(3);
            string bodyBlockName = genStr(3);

            BasicBlock* condBlock = new BasicBlock(condBlockName);
            BasicBlock* exitBlock = new BasicBlock(exitBlockName);
            BasicBlock* bodyBlock = new BasicBlock(bodyBlockName);

            blocks.push_back(condBlock);
            blocks.push_back(exitBlock);
            blocks.push_back(bodyBlock);

            // init in current block
            if (initNode) {
                traverse(initNode);
            }

            if (currentBB) {
                currentBB->trueBlock = condBlock;
            }

            // condition must be generated inside condition block
            currentBB = condBlock;
            if (condNode && condNode->type == "relop") {
                string boolVar = genTACS(condNode);
                condBlock->condition = TAC("ifFalse", boolVar, "goto", exitBlockName);

                // important: matches current CodeGen logic
                // if(cond) goto falseBlock; else goto trueBlock;
                condBlock->trueBlock = exitBlock; // false branch
                condBlock->falseBlock = bodyBlock; // true branch
            }

            // body
            currentBB = bodyBlock;
            if (stmtBlockNode && stmtBlockNode->type == "Stmtblock" && !stmtBlockNode->children.empty()) {
                Node* stmtList = *stmtBlockNode->children.begin();
                traverse(stmtList);
            }

            // update at end of body
            if (updateNode) {
                traverse(updateNode);
            }

            if (currentBB) {
                currentBB->trueBlock = condBlock;
            }

            currentBB = exitBlock;
            return;

        } else if (n->type == "IF") {
            auto it = n->children.begin();

            Node* relopNode = nullptr;
            Node* ifStmt = nullptr;
            Node* elseStmt = nullptr;

            if (it != n->children.end()) relopNode = *it++;
            if (it != n->children.end()) ifStmt = *it++;
            if (it != n->children.end()) elseStmt = *it++;

            string condBlockName = genStr(3);
            string trueBlockName = genStr(3);
            string falseBlockName = genStr(3);
            string continueBlockName = genStr(3);

            BasicBlock* continueBlock = new BasicBlock(continueBlockName);
            BasicBlock* elseBlock = new BasicBlock(trueBlockName, continueBlock);
            BasicBlock* ifBlock = new BasicBlock(falseBlockName, continueBlock);
            BasicBlock* condBlock = new BasicBlock(condBlockName);

            blocks.push_back(condBlock);
            blocks.push_back(continueBlock);
            blocks.push_back(elseBlock);
            blocks.push_back(ifBlock);

            if (currentBB) {
                currentBB->trueBlock = condBlock;
            }

            // build condition inside the condition block
            currentBB = condBlock;
            if (relopNode && relopNode->type == "relop") {
                string boolVar = genTACS(relopNode);

                if (n->value == "IFELSE") {
                    condBlock->condition = TAC("ifFalse", boolVar, "goto", trueBlockName);
                    condBlock->trueBlock = elseBlock; // false path
                    condBlock->falseBlock = ifBlock;  // true path
                } else {
                    condBlock->condition = TAC("ifFalse", boolVar, "goto", continueBlockName);
                    condBlock->trueBlock = continueBlock; // false path
                    condBlock->falseBlock = ifBlock;      // true path
                }
            }

            // if body
            if (ifStmt && ifStmt->type == "Stmtblock" && !ifStmt->children.empty()) {
                currentBB = ifBlock;
                Node* stmtList = *ifStmt->children.begin();
                traverse(stmtList);
            }

            if (currentBB && continueBlock) {
                currentBB->trueBlock = continueBlock;
            }

            // else body
            if (n->value == "IFELSE" && elseStmt && elseStmt->type == "Stmtblock" && !elseStmt->children.empty()) {
                currentBB = elseBlock;
                Node* stmtList = *elseStmt->children.begin();
                traverse(stmtList);
            }

            if (currentBB && continueBlock) {
                currentBB->trueBlock = continueBlock;
            }

            currentBB = continueBlock;
            return;

        } else if (n->type == "param") {
            // formal method params are already stored in block->params
            // do NOT emit runtime TAC for them
            return;

        } else if (n->type == "args") {
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

        if (openScope && n->type == "class") {
            currentClassName = "";
        } else if (openScope && n->type == "method") {
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

    vector<BasicBlock*> getBlocks() const {
        return blocks;
    }

    unordered_map<string, vector<const Node*>> getClassAttributes() const {
        return classAttributes;
    }

    void collectParamsFromArgMethod(const Node* argMethod, vector<pair<string,string>>& out) {
        if (!argMethod || argMethod->type != "arg_method") return;
        for (const Node* c : argMethod->children) {
            collectParamsFromParamList(c, out);
        }
    }

    void collectParamsFromParamList(const Node* n, vector<pair<string,string>>& out) {
        if (!n || n->type != "param") return;
        auto it = n->children.begin();
        if (it != n->children.end()) {
            const Node* typeNode = *it++;
            out.push_back({typeNode->value, n->value});
            for (; it != n->children.end(); ++it) {
                collectParamsFromParamList(*it, out);
            }
        }
    }

    string genStr(const int& len) {
        static const char alphanum[] =
            "0"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        string tmp_s;
        tmp_s.reserve(len);

        for (int i = 0; i < len; ++i) {
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        return tmp_s;
    }

    string genTACS(const Node* n, const string& className = "", const string& varName = "") {
        (void)className;
        (void)varName;

        // literals and identifiers
        if (n->children.empty()) {
            return n->value;
        }

        if (n->type == "ID") {
            return n->value;
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

            string str_lhs = genTACS(lhs);
            string str_rhs = genTACS(rhs);
            string name = genStr(2);

            currentBB->TACs.push_back(TAC(name, str_lhs, n->value, str_rhs));
            return name;
        }

        // postfix: object.method, array access, .length
        if (n->type == "postfix") {
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

        // plain function call
        if (n->type == "call") {
            string tmp = genStr(2);
            currentBB->TACs.push_back(
                TAC(tmp, n->value, "call", "0")
            );
            return tmp;
        }

        // fallback
        return n->value;
    }

    string getPosfixTac(const Node* n){
        auto it = n->children.begin();
        Node* lhs = *it++;
        Node* rhs = *it;

        string genName = genStr(2);
        string lhsTacStore = genTACS(lhs);

        if (rhs->type == "length") {
            currentBB->TACs.push_back(
                TAC(genName, lhsTacStore, "length")
            );

        } else if (rhs->type == "arr_access") {
            Node* index = *rhs->children.begin();
            currentBB->TACs.push_back(
                TAC(genName, lhsTacStore, "access", index->value)
            );

        } else if (rhs->type == "funcMethcall") {
            Node* args = *rhs->children.begin();
            traverse(args);

            string callee = lhsTacStore + "." + rhs->value;
            currentBB->TACs.push_back(
                TAC(genName, callee, "call", to_string(args->children.size()))
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

    string arthmetics(const Node* n, const string& initName = ""){
        if (n->children.empty()) {
            return n->value;
        }

        string name = genStr(2);

        auto it = n->children.begin();
        Node* lhs = *it++;
        Node* rhs = *it;

        string str_lhs = arthmetics(lhs);
        string str_rhs = arthmetics(rhs);

        string op = getArithmeticSymbol(n->type);
        if (initName != "") {
            name = initName;
        }

        currentBB->TACs.push_back(TAC(name, str_lhs, op, str_rhs));
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

            if (!block->params.empty()) {
                cout << "  params: ";
                for (size_t i = 0; i < block->params.size(); i++) {
                    cout << block->params[i].first << " " << block->params[i].second;
                    if (i + 1 < block->params.size()) cout << ", ";
                }
                cout << endl;
            }

            if (!block->returnType.empty()) {
                cout << "  returns: " << block->returnType << endl;
            }

            bool hasCondition = false;
            if (block->condition.to_str() != ":") {
                hasCondition = true;
                cout << "  condition: " << block->condition.to_str() << endl;
            }

            if ((hasCondition || !block->params.empty() || !block->returnType.empty()) && !block->TACs.empty()) {
                cout << "----------------------------------------" << endl;
            }

            if (!block->TACs.empty()) {
                for (const TAC& code : block->TACs) {
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
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Could not open " << filename << endl;
            return;
        }

        file << "digraph CFG {\n";
        file << "    node [shape=box];\n";

        vector<BasicBlock*> visited;
        vector<BasicBlock*> pending;
        unordered_map<BasicBlock*, string> ids;
        int count = 0;

        // Start from all methods
        for (BasicBlock* b : blocks) {
            if (b && b->type == "method") {
                pending.push_back(b);
            }
        }

        // Traverse graph and collect blocks
        while (!pending.empty()) {
            BasicBlock* block = pending.back();
            pending.pop_back();

            bool seen = false;
            for (BasicBlock* v : visited) {
                if (v == block) {
                    seen = true;
                    break;
                }
            }
            if (seen) continue;

            visited.push_back(block);
            ids[block] = "n" + to_string(count++);

            if (block->trueBlock) pending.push_back(block->trueBlock);
            if (block->falseBlock) pending.push_back(block->falseBlock);
        }

        // Write nodes
        for (BasicBlock* block : visited) {
            string label = block->name + "\\l";

            if (!block->params.empty()) {
                label += "params: ";
                for (size_t i = 0; i < block->params.size(); i++) {
                    label += block->params[i].first + " " + block->params[i].second;
                    if (i + 1 < block->params.size()) label += ", ";
                }
                label += "\\l";
            }

            if (!block->returnType.empty()) {
                label += "returns: " + block->returnType + "\\l";
            }

            if (block->condition.to_str() != ":") {
                label += "condition: " + block->condition.to_str() + "\\l";
            }

            for (const TAC& tac : block->TACs) {
                label += tac.to_str() + "\\l";
            }

            if (block->TACs.empty()) {
                label += "<empty>\\l";
            }

            file << "    " << ids[block] << " [label=\"" << label << "\"];\n";
        }

        // Write edges
        for (BasicBlock* block : visited) {
            if (block->trueBlock) {
                file << "    " << ids[block] << " -> " << ids[block->trueBlock]
                    << " [label=\"true\"];\n";
            }
            if (block->falseBlock) {
                file << "    " << ids[block] << " -> " << ids[block->falseBlock]
                    << " [label=\"false\"];\n";
            }
        }

        file << "}\n";
        file.close();
    }
};

#endif