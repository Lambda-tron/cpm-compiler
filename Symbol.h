#ifndef SYMBOL_H
#define SYMBOL_H
#include "Node.h"
#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class Symbol {
public:
    string name,kind,type;
    Symbol(const string& name,const string& kind,const string& type = "UNDEFINED") 
    : name(name), kind(kind), type(type) {}
};

class Scope {
public:
    string name;
    Scope* parent = nullptr;
    vector<Scope*> childrenScopes;
    vector<Symbol*> symbols;
    
    int symbolIndexInParentTable = -1;
    int nextChildIndex = 0;
    
    Scope(const string& name, Scope* parent,const int symbolIndexInParentTable) 
    : name(name), parent(parent), symbolIndexInParentTable(symbolIndexInParentTable){}

    void insertSymbol(Symbol* symbol) {
        symbols.push_back(symbol);
    }

    void insertScope(Scope* scope) {
        childrenScopes.push_back(scope);
    }  

    ~Scope() {
        for (Symbol* sym : symbols) {
            delete sym;
        }
        for (Scope* child : childrenScopes) {
            delete child;
        }
    }
};

class ST {
    private:
        //travers the tree and build the ST
        void traverse(Node* n) {
            bool openedScope = false;
            if (n->type == "program") {
                rootScope = new Scope("global", nullptr,0);
                currentScope = rootScope;
                openedScope = true;
            }
            else if (n->type == "class" || n->type == "method" || n->type == "FOR" || n->type == "IF") {
                openedScope = true;
                createAndEnterScope(n);
            }
            else if ((currentScope->name == "IF" || currentScope->name == "IFELSE") && n->type == "Stmtblock" ){
                openedScope = true;
                if(currentScope->name == "IFELSE"){
                    if(currentScope->symbols.size() == 0){
                        n->value = "IFStmtblock";
                    }else{
                        n->value = "ELSEStmtblock";
                    }
                }
                createAndEnterScope(n);
            }
            else if (n->type == "v_var" || n->type == "var" || n->type == "param") {
                currentScope->insertSymbol(new Symbol(n->value, n->type));
            }
            else if (n->type == "type") {

                if (!currentScope->symbols.empty()) {
                    if (currentScope->symbols.back()->type == "UNDEFINED") {
                        currentScope->symbols.back()->type = n->value;
                    }else if(currentScope->parent->symbols.back()->type == "UNDEFINED"){
                        currentScope->parent->symbols.back()->type = n->value;
                    }
                } else if (currentScope->parent && currentScope->parent->symbols.back()->type == "UNDEFINED") {
                    currentScope->parent->symbols.back()->type = n->value;
                }
            }
            for (Node* child : n->children) {
                traverse(child);
            }
            if (openedScope) {
                leave_scope();
            }
        }
        
        //create a scope and enter it, used only when building tehe ST
        void createAndEnterScope(const Node* n) {
            Scope* sc = new Scope(n->value, currentScope, currentScope->symbols.size());
            Symbol* sy = new Symbol(n->value, n->type);
            currentScope->insertSymbol(sy);
            currentScope->insertScope(sc);
            currentScope = sc;
        }

        //print the symbol table
        void printRecursive(const Scope* s, int depth = 0) {
            if (s == nullptr) return;

            string indent(depth * 4, ' ');

            cout << indent << "========================================" << endl;
            cout << indent << " SCOPE TABLE: " << s->name << endl;
            cout << indent << "========================================" << endl;

            if (s->symbols.empty()) {
                cout << indent << " (No symbols in this scope)" << endl;
            } else {
                for (Symbol* sym : s->symbols) {
                    cout << indent << "  - Name: " << sym->name
                        << " | Kind: " << sym->kind
                        << " | Type: " << (sym->type.empty() ? "<undefined>" : sym->type)
                        << endl;
                }
            }
            cout << endl;

            for (Scope* child : s->childrenScopes) {
                printRecursive(child, depth + 1);
            }
        }

    public:
    Scope* rootScope = nullptr;
    Scope* currentScope = nullptr;
    ST(Node* rootNode) {
        traverse(rootNode);
        //printRecursive(rootScope);
    }
    ~ST() {
        delete rootScope;
    }

    //leave scope when traversing the tree
    void leave_scope() {
        if (currentScope->parent != nullptr) {
            currentScope = currentScope->parent;
        }
    }
};

#endif