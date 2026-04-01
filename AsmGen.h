#ifndef ASMGEN_H
#define ASMGEN_H

#include "ir.h"
#include <cctype>
#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class AsmGen {
private:
    vector<BasicBlock*> blocks;
    string fileName;
    //stack offset for integer variables
    unordered_map<string, int> intSlots;
    //stack offset for float variables
    unordered_map<string, int> fltSlots;
    //arrayName : Number - number of elements in the array
    unordered_map<string, int> arrSize;
    //arrayName : true - if the array stores floats
    unordered_map<string, bool> arrIsFloat;
    // next available stack offset, moves downward
    int nextOff = 0;

    //checks whether the string is a int literal.
    bool isIntLit(const string& s) const {
        if (s.empty()) return false;
        int i = 0;
        if (s[0] == '-') i = 1;
        if (i >= (int)s.size()) return false;
        for (; i < (int)s.size(); i++) {
            if (!isdigit((unsigned char)s[i])) return false;
        }
        return s.find('.') == string::npos;
    }
    //checks whether the string is a float literal.
    bool isFltLit(const string& s) const{
        if (s.empty() || s.find('.') == string::npos) return false;
        int i = 0;
        if (s[0] == '-') i = 1;
        if (i >= (int)s.size()) return false;
        for (; i < (int)s.size(); i++) {
            if (s[i] != '.' && !isdigit((unsigned char)s[i])) return false;
        }
        return true;
    }

    //convert the string to a double
    uint64_t parseFloatLiteral(const string& s)const {
        double value = stod(s);
        //reinterpret the same memory as a 64bit integer
        uint64_t bits = *(uint64_t*)&value;

        return bits;
    }

    //since we cant store all variables on the cpu registers we are storing them in a map in memory
    int allocIntSlot(const string& name) {
        auto it = intSlots.find(name);
        if (it != intSlots.end()) return it->second;
        nextOff -= 8;
        intSlots[name] = nextOff;
        return nextOff;
    }
    //allocate slot for floats
    int allocFloatSlot(const string& name) {
        auto it = fltSlots.find(name);
        if (it != fltSlots.end()) return it->second;

        nextOff -= 8;
        fltSlots[name] = nextOff;
        return nextOff;
    }

    //check if a float exists in the float map
    bool isFltSlot(const string& name) const {
        return fltSlots.find(name) != fltSlots.end();
    }

    //look if a value need to be treated as a float to use float operations
    bool isFltValue(const string& value) const {
        return isFltLit(value) || isFltSlot(value);
    }

    //collect all reachable blocks (from main)
    vector<BasicBlock*> collectBlocks(BasicBlock* start) {
        vector<BasicBlock*> seen;
        vector<BasicBlock*> todo;
        unordered_set<BasicBlock*> used;

        todo.push_back(start);

        while (!todo.empty()) {
            BasicBlock* now = todo.back();
            todo.pop_back();
            if (!used.insert(now).second) continue;
            seen.push_back(now);
            if (now->falseBlock) todo.push_back(now->falseBlock);
            if (now->trueBlock) todo.push_back(now->trueBlock);
        }

        return seen;
    }

    //ignore empty strings and ints and floats and prnt,return etc...
    void rememberValue(const string& value) {
        if (value.empty()) return;
        if (isIntLit(value) || isFltLit(value)) return;
        if (value == "print" || value == "return" || value == "read" || value == "param") return;
        if (!intSlots.count(value) && !fltSlots.count(value)) allocIntSlot(value);
    }

    //scan all variables to preapre assmply generation
    void scanVars(const vector<BasicBlock*>& mainBlocks) {
        for (BasicBlock* block : mainBlocks) {
            // scan all TACs and decide which variables need stack memory and what type
            for (const TAC& tac : block->TACs) {
                string name = tac.getName();
                string op   = tac.getOP();
                string a    = tac.getOperand1();
                string b    = tac.getOperand2();

                if (op == "new") {
                    if (a == "arr_INT" || a == "arr_FLOAT") {
                        allocIntSlot(name);           // array pointer
                        allocIntSlot(name + "_len");  // array length
                        arrIsFloat[name] = (a == "arr_FLOAT");

                        if (!b.empty() && isIntLit(b)) {
                            arrSize[name] = stoi(b);
                        }
                    } else if (a == "FLOAT") {
                        allocFloatSlot(name);
                    } else {
                        allocIntSlot(name);
                    }

                } else if (name == "print" || name == "read" || name == "return" || name == "param") {
                    rememberValue(a);

                } else if (name == "arr_access") {
                    rememberValue(a);   //array
                    rememberValue(b);   //value
                    if (!op.empty() && !isIntLit(op)) {
                        allocIntSlot(op);   //index variable
                    }

                } else if (op == "access") {
                    rememberValue(a);   //array
                    rememberValue(b);   //index

                    if (arrIsFloat.count(a) && arrIsFloat[a]) {
                        allocFloatSlot(name);
                    } else {
                        allocIntSlot(name);
                    }

                } else if (op == "length" || op == "!" || op == "call") {
                    rememberValue(a);
                    allocIntSlot(name);

                } else {
                    if (!name.empty()) {
                        if (isFltValue(a) || isFltValue(b)) {
                            allocFloatSlot(name);
                        } else {
                            allocIntSlot(name);
                        }
                    }

                    rememberValue(a);
                    rememberValue(b);
                }
            }

            if (!block->condition.getOperand1().empty()) {
                allocIntSlot(block->condition.getOperand1());
            }
        }
    }

    //load integer value into the register
    void loadInt(const string& value, ofstream& out) {
        if (isIntLit(value)) out << "    movq $" << value << ", %rax\n";
        else out << "    movq " << allocIntSlot(value) << "(%rbp), %rax\n";
    }
    
    //load a flaot into the XMM register
    void loadFlt(const string& value, int xmmNo, ofstream& out) {
        const char* scratch = (xmmNo == 0) ? "%rax" : "%rcx";

        if (isFltLit(value)) {
            out << "    movabsq $" << parseFloatLiteral(value) << ", " << scratch << "\n";
            out << "    movq " << scratch << ", -8(%rsp)\n";
            out << "    movsd -8(%rsp), %xmm" << xmmNo << "\n";
        } else if (isIntLit(value)) {
            out << "    movq $" << value << ", " << scratch << "\n";
            out << "    cvtsi2sd " << scratch << ", %xmm" << xmmNo << "\n";
        } else {
            out << "    movsd " << allocFloatSlot(value) << "(%rbp), %xmm" << xmmNo << "\n";        }
    }

    //print TACs based on operation type / first name.
    void generateTAC(const TAC& tac, vector<string>& pendingParams, ofstream& out) {
        string name = tac.getName();
        string op = tac.getOP();
        string a = tac.getOperand1();
        string b = tac.getOperand2();


        if (op == "new") {
            if (a == "arr_INT" || a == "arr_FLOAT") {
                int count = 0;
                if (!b.empty() && isIntLit(b)) count = stoi(b);
                arrSize[name] = count;
                out << "    movq $" << count << ", %rax\n";
                out << "    movq %rax, " << allocIntSlot(name + "_len") << "(%rbp)\n";
                out << "    movq $" << count * 8 << ", %rdi\n";
                out << "    call malloc\n";
                out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
            }
            return;
        }

        if (name == "print") {
            if (isFltValue(a)) {
                loadFlt(a, 0, out);
                out << "    leaq fmt_f(%rip), %rdi\n";
                out << "    movl $1, %eax\n";
                out << "    call printf\n";
            } else {
                loadInt(a, out);
                out << "    movq %rax, %rsi\n";
                out << "    leaq fmt_d(%rip), %rdi\n";
                out << "    xorl %eax, %eax\n";
                out << "    call printf\n";
            }
            return;
        }

        if (name == "read") {
            if (isFltSlot(a)) {
                out << "    leaq " << allocFloatSlot(a) << "(%rbp), %rsi\n";
                out << "    leaq fmt_sf(%rip), %rdi\n";
            } else {
                out << "    leaq " << allocIntSlot(a) << "(%rbp), %rsi\n";
                out << "    leaq fmt_sd(%rip), %rdi\n";
            }
            out << "    xorl %eax, %eax\n";
            out << "    call scanf\n";
            return;
        }

        if (name == "return") {
            if (isFltValue(a)) {
                loadFlt(a, 0, out);
                out << "    cvttsd2si %xmm0, %rax\n";
            } else {
                loadInt(a, out);
            }
            out << "    leave\n";
            out << "    ret\n";
            return;
        }

        if (name == "param") {
            pendingParams.push_back(a);
            return;
        }

        if (name == "arr_access") {
            loadInt(a, out);
            out << "    movq %rax, %r10\n";
            if (isIntLit(op)) out << "    movq $" << op << ", %rcx\n";
            else out << "    movq " << allocIntSlot(op) << "(%rbp), %rcx\n";

            if (arrIsFloat.count(a) && arrIsFloat[a]) {
                loadFlt(b, 0, out);
                out << "    leaq (%r10, %rcx, 8), %r11\n";
                out << "    movsd %xmm0, (%r11)\n";
            } else {
                loadInt(b, out);
                out << "    movq %rax, (%r10, %rcx, 8)\n";
            }
            return;
        }

        if (op == "access") {
            loadInt(a, out);
            out << "    movq %rax, %r10\n";
            if (isIntLit(b)) out << "    movq $" << b << ", %rcx\n";
            else out << "    movq " << allocIntSlot(b) << "(%rbp), %rcx\n";

            if (isFltSlot(name)) {
                out << "    movsd (%r10, %rcx, 8), %xmm0\n";
                out << "    movsd %xmm0, " << allocFloatSlot(name) << "(%rbp)\n";
            }else {
                out << "    movq (%r10, %rcx, 8), %rax\n";
                out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
            }
            return;
        }

        if (op == "length") {
            if (arrSize.count(a)) out << "    movq $" << arrSize[a] << ", %rax\n";
            else out << "    movq " << allocIntSlot(a + "_len") << "(%rbp), %rax\n";
            out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
            return;
        }

        if (op == "call") {
            int ireg = 0;
            int freg = 0;

            for (const string& arg : pendingParams) {
                if (isFltValue(arg)) {
                    loadFlt(arg, 0, out);
                    if (freg == 1) out << "    movsd %xmm0, %xmm1\n";
                    else if (freg == 2) out << "    movsd %xmm0, %xmm2\n";
                    else if (freg == 3) out << "    movsd %xmm0, %xmm3\n";
                    else if (freg == 4) out << "    movsd %xmm0, %xmm4\n";
                    else if (freg == 5) out << "    movsd %xmm0, %xmm5\n";
                    freg++;
                } else {
                    loadInt(arg, out);
                    if (ireg == 0) out << "    movq %rax, %rdi\n";
                    else if (ireg == 1) out << "    movq %rax, %rsi\n";
                    else if (ireg == 2) out << "    movq %rax, %rdx\n";
                    else if (ireg == 3) out << "    movq %rax, %rcx\n";
                    else if (ireg == 4) out << "    movq %rax, %r8\n";
                    else if (ireg == 5) out << "    movq %rax, %r9\n";
                    ireg++;
                }
            }

            pendingParams.clear();
            out << "    xorl %eax, %eax\n";
            out << "    call " << a << "\n";
            if (!name.empty()) out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
            return;
        }

        if (op == "!") {
            loadInt(a, out);
            out << "    testq %rax, %rax\n";
            out << "    sete %al\n";
            out << "    movzbq %al, %rax\n";
            out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
            return;
        }

        if (!op.empty() && !b.empty()) {
            if (isFltValue(a) || isFltValue(b)) {
                loadFlt(a, 0, out);
                loadFlt(b, 1, out);

                if (op == "+") out << "    addsd %xmm1, %xmm0\n";
                else if (op == "-") out << "    subsd %xmm1, %xmm0\n";
                else if (op == "*") out << "    mulsd %xmm1, %xmm0\n";
                else if (op == "/") out << "    divsd %xmm1, %xmm0\n";
                else {
                    out << "    ucomisd %xmm1, %xmm0\n";
                    if (op == "<") out << "    setb %al\n";
                    else if (op == "<=") out << "    setbe %al\n";
                    else if (op == ">") out << "    seta %al\n";
                    else if (op == ">=") out << "    setae %al\n";
                    else if (op == "=") out << "    sete %al\n";
                    else if (op == "!=") out << "    setne %al\n";
                    out << "    movzbq %al, %rax\n";
                    out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
                    return;
                }

                if (isFltSlot(name)) {
                    out << "    movsd %xmm0, " << allocFloatSlot(name) << "(%rbp)\n";
                } else {
                    out << "    cvttsd2si %xmm0, %rax\n";
                    out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
                }
                return;
            }

            if (op == "^") {
                loadInt(a, out);
                out << "    movq %rax, %r8\n";
                loadInt(b, out);
                out << "    movq %rax, %rcx\n";
                out << "    movq $1, %rax\n";
                out << ".Lpow_" << (-allocIntSlot(name)) << ":\n";
                out << "    cmpq $0, %rcx\n";
                out << "    je .Lpow_done_" << (-allocIntSlot(name)) << "\n";
                out << "    imulq %r8, %rax\n";
                out << "    subq $1, %rcx\n";
                out << "    jmp .Lpow_" << (-allocIntSlot(name)) << "\n";
                out << ".Lpow_done_" << (-allocIntSlot(name)) << ":\n";
                out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
                return;
            }

            loadInt(a, out);
            out << "    movq %rax, %r8\n";
            loadInt(b, out);
            out << "    movq %rax, %rcx\n";
            out << "    movq %r8, %rax\n";

            if (op == "+") out << "    addq %rcx, %rax\n";
            else if (op == "-") out << "    subq %rcx, %rax\n";
            else if (op == "*") out << "    imulq %rcx, %rax\n";
            else if (op == "/") out << "    cqto\n    idivq %rcx\n";
            else if (op == "&&") out << "    andq %rcx, %rax\n    setne %al\n    movzbq %al, %rax\n";
            else if (op == "||") out << "    orq %rcx, %rax\n    setne %al\n    movzbq %al, %rax\n";
            else {
                out << "    cmpq %rcx, %rax\n";
                if (op == "<") out << "    setl %al\n";
                else if (op == "<=") out << "    setle %al\n";
                else if (op == ">") out << "    setg %al\n";
                else if (op == ">=") out << "    setge %al\n";
                else if (op == "=") out << "    sete %al\n";
                else if (op == "!=") out << "    setne %al\n";
                out << "    movzbq %al, %rax\n";
            }

            out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
            return;
        }

        if (isFltSlot(name)) {
            loadFlt(a, 0, out);
            out << "    movsd %xmm0, " << allocFloatSlot(name) << "(%rbp)\n";
        } else {
            loadInt(a, out);
            out << "    movq %rax, " << allocIntSlot(name) << "(%rbp)\n";
        }
    }

public:
    AsmGen(const vector<BasicBlock*>& blocks, const string& fileName = "out.s"){
        this->blocks = blocks; 
        this->fileName = fileName; 
        generate();
    }

        //starter
        void generate() {
            ofstream out(fileName);

            out << ".section .rodata\n";
            out << "fmt_d:  .string \"%ld\\n\"\n";
            out << "fmt_f:  .string \"%g\\n\"\n";
            out << "fmt_sd: .string \"%ld\"\n";
            out << "fmt_sf: .string \"%lf\"\n\n";

            out << ".text\n";
            out << ".extern printf\n";
            out << ".extern scanf\n";
            out << ".extern malloc\n\n";
            out << ".globl main\n\n";

            BasicBlock* entry = blocks[0];
            //get all blocks reachable from main
            vector<BasicBlock*> mainBlocks = collectBlocks(entry);
            //scan all the variables
            scanVars(mainBlocks);

            unordered_map<BasicBlock*, string> labels;
            int labelNo = 0;
            //get all labels
            for (BasicBlock* b : mainBlocks) {
                if (b == entry) labels[b] = "main";
                else labels[b] = ".Lmain_" + to_string(labelNo++);
            }

            //calculate needed space for all variiables, stack myst be 16 bytes aligned for function calls 
            int frameSize = ((intSlots.size() + fltSlots.size()) * 8 + 47) & ~15;
            if (frameSize < 64) frameSize = 64;

            vector<string> pendingParams;


            for (BasicBlock* block : mainBlocks) {
                //print the lable
                out << labels[block] << ":\n";

                //print the main function prologue
                if (block == entry) {
                    out << "    pushq %rbp\n";
                    out << "    movq %rsp, %rbp\n";
                    out << "    subq $" << frameSize << ", %rsp\n";
                }
                //start generatng TAC inistrcutions
                for (const TAC& tac : block->TACs) {
                    generateTAC(tac, pendingParams, out);
                }
                //generate control flow jumps
                if (block->condition.getName() == "ifFalse") {
                    out << "    movq " << allocIntSlot(block->condition.getOperand1()) << "(%rbp), %rax\n";
                    out << "    testq %rax, %rax\n";
                    if (block->trueBlock) out << "    je " << labels[block->trueBlock] << "\n";
                    if (block->falseBlock) out << "    jmp " << labels[block->falseBlock] << "\n";
                } else if (block->trueBlock) {
                    //unconditional jumps
                    out << "    jmp " << labels[block->trueBlock] << "\n";
                }

                out << "\n";
            }
        }
};

#endif