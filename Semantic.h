#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "Node.h"
#include "Symbol.h"
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class SemanticAnalyzer {
private:
    ST* symbolTable;
public:
    int errorCount = 0;
    SemanticAnalyzer(ST* st,const Node* root) : symbolTable(st) {
        analyze(root);
    }

    bool didReturn = false;
    int returnLineNo = -1;

    void analyze(const Node* node) {
        bool openedScope = false;
        bool hasReturnType = false;
        
        //check for dead code
        if(didReturn && (node->lineno != returnLineNo-1 && node->lineno != returnLineNo)){
            err_dead_code(node->type, node->value, node->lineno);
        }

        //check dublicate definitions & defined datatypes
        if (node->type == "var"  || node->type == "v_var" || node->type == "method" || node->type == "class" || node->type == "param"){
            if(checkDublicateDefinition(node->value)){
                err_duplicate_definition(node->value, node->lineno);
            }

            //check if data Type Exist
            if (node->type == "var" || node->type == "v_var") {
                Symbol* symbol = lookup(node->value);
                if (!symbol) return; // already reported elsewhere

                if (!isBuiltinType(symbol->type)) {
                    Scope* scope = getClassScope(symbol->type);
                    if (!scope) {
                        err_undefined_datatype(symbol->name, symbol->type, node->lineno);
                    }
                }

                //check if the definition is valid
                validateDefiniation(node);
            }
        }

        //make sure the statement in IF returns bool
        if(node->type == "IF"){
            Node* stmt = *node->children.begin();
            string dataType = evalExprType(stmt);

            if (dataType != "BOOL") {
                err_invalid_condition_type(dataType, stmt->lineno);
            }
        }

        //evaluate postfixes
        if(node->type == "postfix"){
            string postFixType = evalExprType(node);
        }

        //Enter scope
        if (node->type == "class" || node->type == "method" || node->type == "IF" || node->type == "FOR" || 
            ((symbolTable->currentScope->name == "IF" || symbolTable->currentScope->name == "IFELSE") && node->type == "Stmtblock")) {
                if(node->type == "method"){
                    Symbol* sy = lookup(node->value);
                    if(sy->type != "VOID"){
                        hasReturnType=true;
                        
                    }
                }

		    enterScope(node);
            openedScope = true;
        }
        else if (node->type == "return"){
            Symbol* funcSymbol = symbolTable->currentScope->parent->symbols[symbolTable->currentScope->symbolIndexInParentTable];
            
            if(funcSymbol->type != "VOID"){
                Node* returned = *node->children.begin();
                string returnType = evalExprType(returned); 
                
                if (funcSymbol->type != returnType) {
                    err_return_type_mismatch(funcSymbol->name, funcSymbol->type, returnType, node->lineno);
                }else{
                    returnLineNo = node->lineno;
                    didReturn = true;
                
                }
            }else{
                err_void_return_value(funcSymbol->name, node->lineno);
            }

            

        }
        else if(node->type == "assign"){
            assigmentCheck(node);
        }

        for (Node* child : node->children) {
            analyze(child);
        }

        if (openedScope) {
            if(hasReturnType && !didReturn){
                err_missing_return(symbolTable->currentScope->name, node->lineno);
            }
                
            leaveScope();
            
            didReturn=false;
            returnLineNo=-1;
           
        }

    }
    //enter Scope
    void enterScope(const Node* n){
        symbolTable->currentScope = symbolTable->currentScope->childrenScopes[symbolTable->currentScope->nextChildIndex];
        symbolTable->currentScope->parent->nextChildIndex++;
        symbolTable->currentScope->nextChildIndex = 0; 
    }

    //validate the definition
    void validateDefiniation(const Node* n){
        
        //return if we are not assigning anything at the definition
        if(n->children.size() < 2) return;
        //access the type node and the value node
        auto it = n->children.begin();
        Node* dataType = *it++;
        Node* value = *it;
        //cout << "validating: " << n->type << ":" << n->value << endl;
        string nodeDT = evalExprType(value);
        //datatypes can only be INT,BOOL,FLOAT,arr_INT,arr_FLOAT
        if(dataType->value != nodeDT){
            if (((nodeDT == "arr_INT"  && dataType->value != "INT") || (nodeDT == "arr_FLOAT" && dataType->value != "FLOAT")) || dataType->value != nodeDT) {
                err_invalid_initialization(n->value, dataType->value, nodeDT, n->lineno);
                return;
            }
        }

    }

    //lookup if a variable has been declared in the currentScope or the one wrapping it.
    Symbol* lookup(const string& symbolName) {
        Scope* temp = symbolTable->currentScope; 
        while (temp != nullptr) { 
            for (int i = 0; i < temp->symbols.size(); i++) {
                if (temp->symbols[i]->name == symbolName) {
                    return temp->symbols[i];
                }
            }
            temp = temp->parent;
        }
        return nullptr;
    }

    //check if there are dublicate definiation in same scope.
    bool checkDublicateDefinition(const string& varName){
        int numdef = 0;
        for (int i = 0; i < symbolTable->currentScope->symbols.size(); i++) {
            if (symbolTable->currentScope->symbols[i]->name == varName) {
                numdef++;
            }
        }
        return numdef > 1;
    }

    //cehck if the assingment is correct.
    void assigmentCheck(const Node* node) {
        // assign must have exactly 2 children
        if (!node || node->children.size() != 2) return;

        auto it = node->children.begin();
        Node* lhs = *it++;
        Node* rhs = *it;

        //target type from LHS 
        string targetType;
        string lhsType;
        if (lhs->type == "ID") {
            Symbol* sym = lookup(lhs->value);
            if (!sym) { err_undefined_identifier(lhs->value, lhs->type, lhs->lineno); return; }
            targetType = sym->type;
        } 
        else if (lhs->type == "postfix") {
            targetType = getPostFixDatatype(lhs);
            //cout << targetType << "LEFTSIDE LINE: " << lhs->lineno << endl;
            if(targetType == "UNKOWN"){return;}
        } 
        else {
            err_invalid_lvalue(lhs->lineno);
            return;
        }

        //get RHS type 
        string rhsType = evalExprType(rhs);

        //RELOP, MATHOP
        if(rhsType == "UNKOWN"){return;}

        //compare
        if (!targetType.empty() && !rhsType.empty() && targetType != rhsType) {
            err_dataType_assignment(lhs->value,targetType, rhsType, rhs->lineno);
        }
    }

    //either we are accessing a ITEM in an array OR calling a function OR .length.
    string getPostFixDatatype(const Node* node) {

        auto it = node->children.begin();
        //get the ID at the start
        Node* base = *it; ++it;
        //get the POSTFIX could be arr_access or length or method call
        Node* post = *it;
        
        //cout << "LINE: " << base->lineno << ", BASE: " << base->type << ":" << base->value << ", POST:" << post->type << ":" << post->value << endl; 
        string baseType = base->type;

        if(base->type == "ID"){
            //get the ID symbol
            Symbol* baseSym = lookup(base->value);
            if (!baseSym) {
                err_undefined_identifier(base->value, base->type,base->lineno);
                return "UNKOWN";
            }
            baseType = baseSym->type;
        }
        else if(base->type == "call"){
            string funcType = validateFunctionCall(base);

            baseType = funcType;
            
        }
        else if(base->type == "postfix"){
            baseType = getPostFixDatatype(base);
        }
        
        string postReturnType = "";
        //this is calling a func from outside of its class
        if (post->type == "funcMethcall") {
            if(!isBuiltinType(baseType)){
                postReturnType = validateFunctionCall(post,baseType);
    
            }else{
                err_invalid_method_access(baseType, post->value, post->lineno);
                postReturnType = "UNKOWN";
            }
        }
        else if (post->type == "arr_access" && (baseType == "arr_INT" || baseType == "arr_FLOAT")) {

            auto it = post->children.begin();
            Node* arrAccessIndex = *it;
            string accessType = evalExprType(arrAccessIndex);

            
            if(accessType != "INT"){
                err_invalid_index_type(base->value, accessType, post->lineno);
                postReturnType = "UNKOWN";
            }else{
                if(baseType == "arr_INT"){
                    postReturnType = "INT";
                }else if (baseType == "arr_FLOAT"){
                    postReturnType = "FLOAT";
                }
            }
            
        }
        else if(post->type == "length"){
            Symbol* baseSym = lookup(base->value);
            if (!baseSym) {
                err_undefined_identifier(base->value, base->type,base->lineno);
                return "UNKOWN";
            }
            //check the base if its an array
            if(baseType == "arr_INT" || baseType == "arr_FLOAT"){
                postReturnType = "INT";
            }
            else{
                err_invalid_length_access(base->value, baseSym->type, post->lineno);
                postReturnType = "UNKOWN";
            }

        }
        return postReturnType;
    }

    //check the function datatype, parameters given.
    string validateFunctionCall(const Node* funcNode, string className = ""){
        /*
            main() : int {
                Obj.func() <-funcmethcall

                SPECIAL CASE 
                CLASS().func()
                NOT ALLOWED
                CLASS(arg).func()
                ALLOWED
                METHOD(arg).func()
            }

            class Obj2{
                Obj.func() <-funcmethcall
            }

            class Obj{
                func() <-call
            }
            

        */

        //get the function scope
        Scope* funcScope = nullptr;
        Symbol* funcSymbol = nullptr;  
        if(className == ""){
            //get the func scope if its called within the class.
            funcScope = getMethodScopeInSameClass(funcNode->value);
        }
        else{
            //get the func scope iif its called outside the class.
            funcScope = getFuncFromDiffScope(funcNode, className);
        }

        if(!funcScope){
            //check if class exists then it could be a class isntance CLASS().METHOD()
            Scope* classScope = getClassScope(funcNode->value);
            if(classScope){
                return classScope->name;
            }
            err_method_not_found(funcNode->value, funcNode->lineno);
            return "UNKOWN"; 
        }

        //get the functionSymbol to get its return type
        for(Symbol* sy : funcScope->parent->symbols){
            if(sy->name== funcScope->name){
                funcSymbol = sy;
            }
        }
        
        //get the arguments that the function takes
        int expectedArgs = 0;
        for(int i = 0; i < funcScope->symbols.size(); i++){
            if(funcScope->symbols[i]->kind == "param"){
                expectedArgs++;
            }
        }

        //make sure both given arguments = amount of paramets
        auto it = funcNode->children.begin();
        Node* argsNode = *it;
        int argCount = argsNode->children.size();
        if (argCount != expectedArgs) { 
            err_argument_count(funcScope->name, expectedArgs, argCount, argsNode->lineno);
            return funcSymbol->type;
        }

        //check all arguments has the correct type
        int symbolIndex = 0;

        for (Node* argument: argsNode->children) {
            string argType = argument->type;
            string paramType = funcScope->symbols[symbolIndex]->type;
            //check if its call or POSTFIX
            if(argument->type == "call"){
                argType = validateFunctionCall(argument);
            }else if (argument->type == "postfix"){
                argType = getPostFixDatatype(argument);
            }else if(argument->type == "ID"){
                Symbol* argSymbol = lookup(argument->value);
                if(argSymbol==nullptr){
                    err_undefined_identifier(argument->value,argument->type,argument->lineno);
                }
                argType = argSymbol->type;
            }

            symbolIndex++;
            if (argType != paramType && argType != "") {
                err_argument_type(funcScope->name, symbolIndex, paramType, argType, argument->lineno);
            }
        }
        //cout << "now will return " << funcSymbol->type << endl;
        return funcSymbol->type;
    }

    //get a scope for a given class or method name
    Scope* getClassScope(const string& scopeName){
        for(int i = 0; i < symbolTable->rootScope->childrenScopes.size(); i++){
            if(symbolTable->rootScope->childrenScopes[i]->name == scopeName){
                return symbolTable->rootScope->childrenScopes[i];
            }
        }
        return nullptr;
    }

    //get a scope for a given method
    Scope* getMethodScopeInSameClass(const string& methodName){
        Scope* temp = symbolTable->currentScope;
        while(temp->parent){
            for(int i = 0; i < temp->childrenScopes.size(); i++){
                if(temp->childrenScopes[i]->name == methodName){
                    return temp->childrenScopes[i];
                }
            }
            temp = temp->parent;
        }
        return nullptr;
    }

    //get out of a scope
    void leaveScope() {
        if (symbolTable->currentScope->parent != nullptr) {
            symbolTable->currentScope = symbolTable->currentScope->parent;
        }
    }

    //check if its a built in datatype
    bool isBuiltinType(const string& type) {
        return type == "INT"
        || type == "BOOL"
        || type == "FLOAT"
        || type == "arr_INT"
        || type == "arr_FLOAT";
    }

    //get return rtp on arhemetics
    string arthmetics(const Node* opNode) {
        auto it = opNode->children.begin();
        Node* left = *it++;
        Node* right = *it;
        string leftType = evalExprType(left);
        string rightType = evalExprType(right);

        if (leftType == "UNKOWN" || rightType == "UNKOWN") return "UNKOWN";
        

        if ((leftType == "INT" || leftType == "FLOAT") && (rightType == "INT" || rightType == "FLOAT")) {

            if(leftType != rightType){
                err_operand_type_mismatch(opNode->type, leftType, rightType, opNode->lineno);
                return "UNKOWN";
            }


            return leftType;

        }else{
            err_operand_type_mismatch(opNode->type, leftType, rightType, opNode->lineno);
            return "UNKOWN";
        }


    }

    //get return type on relop
    string relop(const Node* relNode){
        auto it = relNode->children.begin();
        Node* left = *it++;
        Node* right = *it;
        string leftType = evalExprType(left);
        string rightType = evalExprType(right);

        if (leftType == "UNKOWN" || rightType == "UNKOWN") return "UNKOWN";

        //check if its AND OR both sides should be bool
        if(relNode->type == "and" || relNode->type == "or"){
            if (leftType != "BOOL" && rightType != "BOOL") {
                err_operand_type_mismatch(relNode->type, leftType, rightType, relNode->lineno);
                return "UNKOWN";
            }
        }else{
            // RELOPs
            bool valid = (leftType == "INT"   && rightType == "INT") || (leftType == "FLOAT" && rightType == "FLOAT");

            if (!valid) {
                err_operand_type_mismatch(relNode->value, leftType, rightType, relNode->lineno);
                return "UNKOWN";
            }          
        }
        return "BOOL";
    }

    //get the expr type
    string evalExprType(const Node* n) {

        // Variables
        if (n->type == "ID") {
            Symbol* sym = lookup(n->value);
            if (!sym) { err_undefined_identifier(n->value, n->type, n->lineno); return "UNKOWN"; }
            return sym->type;
        }
        
        if (n->type == "INT" || n->type == "FLOAT" || n->type == "BOOL") {
            return n->type;
        }

        // arr[i], .length, obj.method()
        if (n->type == "postfix") {
            return getPostFixDatatype(n);
        }

        //validate array initiation
        if(n->type == "init_array"){
            //access the type node and the value node
            auto it = n->children.begin();
            Node* dataType = *it++;
            Node* values = *it;
            int index = 0;
            for(Node* value: values->children){
                if(dataType->value != value->type){
                    err_array_element_type(dataType->value, value->type, index, value->lineno);
                }
                index++;
            }
            if(dataType->value == "INT"){
                return "arr_INT";
            }else{
                return "arr_FLOAT";
            }
        }

        // Function call node
        if (n->type == "call") {
            return validateFunctionCall(n);
        }

        // Arithmetic ops 
        if (n->type == "PWROP" || n->type == "PLUSOP" || n->type == "MINOP" || n->type == "MULTOP" ||  n->type == "DIVOP")  return arthmetics(n);

        // relop/and/or 
        if (n->type == "and" || n->type == "or" || n->type == "relop")  return relop(n);

        if(n->type == "not"){
            auto it = n->children.begin();
            Node* child = *it;
            string childType = evalExprType(child);
            
            if(childType!="BOOL"){
                err_expected_bool_after_not(childType, n->lineno);
                return "UNKOWN";
            }

            return "BOOL";
        }

        return n->type;
    }

    //get functionf from diff class
    Scope* getFuncFromDiffScope(const Node* funcNode, string className){
        Scope* foundClassTemp = nullptr;
        for(Scope* classScope: symbolTable->rootScope->childrenScopes){
            if(classScope->name == className){
                //cout << "FOUND CLASS " << className << endl;
                foundClassTemp = classScope;
                className = classScope->name;
                //get the funcscope if the class scope was found
                for(Scope* for_funcSccope: foundClassTemp->childrenScopes){
                    //cout << "symbol " << for_funcSccope->name << endl;
                    if(for_funcSccope->name == funcNode->value){
                        return for_funcSccope;
                    }  
                } 
            }
        }
        if(!foundClassTemp){
            err_class_not_found(className, funcNode->lineno);
        }else{
            err_method_not_found(funcNode->value, funcNode->lineno);        
        }
        return nullptr; 
    }

    // ------- error functions -------
    void err_dead_code(const string& nodeType, const string& nodeValue, int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Unreachable code detected near '" << nodeType << " " << nodeValue << "'."
            << endl;
        errorCount++;
    }

    void err_duplicate_definition(const string& name, int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Duplicate definition of '" << name << "'."
            << endl;
        errorCount++;
    }

    void err_return_type_mismatch(const string& funcName,
                                const string& expectedType,
                                const string& actualType,
                                int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Return type mismatch in function '" << funcName
            << "'. Expected '" << expectedType
            << "', got '" << actualType << "'."
            << endl;
        errorCount++;
    }

    void err_void_return_value(const string& funcName, int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Cannot return a value from void function '" << funcName << "'."
            << endl;
        errorCount++;
    }

    void err_missing_return(const string& funcName, int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Missing return statement in function '" << funcName << "'."
            << endl;
        errorCount++;
    }

    void err_invalid_method_access(const string& baseType,
                                const string& methodName,
                                int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Cannot access method '" << methodName
            << "' on value of type '" << baseType << "'."
            << endl;
        errorCount++;
    }

    void err_invalid_index_type(const string& name,
                                const string& indexType,
                                int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Cannot index '" << name
            << "' with expression of type '" << indexType << "'."
            << endl;
        errorCount++;
    }

    void err_invalid_length_access(const string& varName,
                                const string& varType,
                                int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Variable '" << varName
            << "' of type '" << varType
            << "' does not support '.length'."
            << endl;
        errorCount++;
    }

    void err_method_not_found(const string& methodName, int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Method '" << methodName << "' does not exist."
            << endl;
        errorCount++;
    }

    void err_class_not_found(const string& className, int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Class '" << className << "' does not exist."
            << endl;
        errorCount++;
    }

    void err_argument_count(const string& methodName,
                            int expectedArgs,
                            int actualArgs,
                            int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Method '" << methodName
            << "' expects " << expectedArgs
            << " argument(s), got " << actualArgs << "."
            << endl;
        errorCount++;
    }

    void err_argument_type(const string& methodName,
                        int argIndex,
                        const string& expectedType,
                        const string& actualType,
                        int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Argument " << argIndex
            << " of method '" << methodName
            << "' must be of type '" << expectedType
            << "', got '" << actualType << "'."
            << endl;
        errorCount++;
    }

    void err_operand_type_mismatch(const string& opName,
                                const string& leftType,
                                const string& rightType,
                                int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Operator '" << opName
            << "' requires compatible operands, got '"
            << leftType << "' and '" << rightType << "'."
            << endl;
        errorCount++;
    }

    void err_expected_bool_after_not(const string& actualType, int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Operator '!' requires operand of type 'BOOL', got '"
            << actualType << "'."
            << endl;
        errorCount++;
    }
    
    void err_undefined_identifier(const string& name, const string& kind, int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Undefined " << kind << " '" << name << "'."
            << endl;
        errorCount++;
    }

    void err_dataType_assignment(const string& lhsName,
                                const string& lhsType,
                                const string& rhsType,
                                int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Type mismatch in assignment. Cannot assign value of type '"
            << rhsType << "' to variable '" << lhsName
            << "' of type '" << lhsType << "'."
            << endl;
        errorCount++;
    }

    void err_invalid_lvalue(int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Left-hand side of assignment is not assignable."
            << endl;
        errorCount++;
    }

    void err_undefined_datatype(const string& varName,const string& typeName,int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Variable '" << varName
            << "' declared with undefined type '" << typeName << "'."
            << endl;
        errorCount++;
    }

    void err_invalid_condition_type(const string& type, int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Condition must be of type 'BOOL', got '" << type << "'."
            << endl;
        errorCount++;
    }

    void err_invalid_initialization(const string& varName,
                                const string& varType,
                                const string& valueType,
                                int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Cannot initialize variable '" << varName
            << "' of type '" << varType
            << "' with value of type '" << valueType << "'."
            << endl;
        errorCount++;
    }

    void err_array_element_type(const string& expectedType,
                            const string& actualType,
                            int index,
                            int lineno) {
        cerr << "Semantic Error (Line " << lineno
            << "): Array element at index " << index
            << " must be of type '" << expectedType
            << "', got '" << actualType << "'."
            << endl;
        errorCount++;
    }
};

#endif
