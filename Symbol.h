#ifndef SYMBOL_H
#define SYMBOL_H
#include "Node.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Symbol {
public:
    string name,kind,type;
    Symbol(string name, string kind, string type = "UNDEFINED") 
        : name(name), kind(kind), type(type) {}
};

class Scope {
public:
    string name;
    Scope* parent;
    vector<Scope*> childrenScopes;
    int symbolIndexInParentTable;
    vector<Symbol*> symbols;
    int nextChildIndex = 0;
    Scope(string name, Scope* parent, int symbolIndexInParentTable) 
    : name(name), parent(parent), symbolIndexInParentTable(symbolIndexInParentTable){}

    void insertSymbol(Symbol* symbol) {
        symbols.push_back(symbol);
    }

    void insertScope(Scope* scope) {
        childrenScopes.push_back(scope);
    }  
};

class ST {

public:
    Node* rootNode;
    Scope* rootScope;
    Scope* currentScope;
    ST(Node* rootNodeptr) {
        rootNode = rootNodeptr;
        travers(rootNode);
        //printRecursive(rootScope);
    }
    void travers(Node* n, int depth = 0) {
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
                }else{
                    currentScope->parent->symbols.back()->type = n->value;
                }
            } else if (currentScope->parent && currentScope->parent->symbols.back()->type == "UNDEFINED") {
                currentScope->parent->symbols.back()->type = n->value;
            }
        }
        for (Node* child : n->children) {
            travers(child, depth + 1);
        }
        if (openedScope) {
            leave_scope();
        }
    }
    
    void createAndEnterScope(Node* n) {
        Scope* sc = new Scope(n->value, currentScope, currentScope->symbols.size());
        Symbol* sy = new Symbol(n->value, n->type);
        currentScope->insertSymbol(sy);
        currentScope->insertScope(sc);
        currentScope = sc;
    }

    void leave_scope() {
        if (currentScope->parent != nullptr) {
            currentScope = currentScope->parent;
        }
    }

    void printRecursive(Scope* s, int depth = 0) {
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

    

};

#endif