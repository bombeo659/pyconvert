#ifndef VM_H
#define VM_H

#include "main.h"

const int SIZE_INSTRUCTION_MEMORY = 65535;
const int SIZE_STATIC_MEMORY = 65535;
const int SIZE_STACK = 1000;

enum Instructions{
  ADD, MINUS, MUL, DIV,
  CMPEQ, CMPNE, CMPLT, CMPLE, CMPGT, CMPGE,
  NOT, AND, OR,
  MOVE, LOAD, STORE,
  JUMP, JUMPIF, CALL, RETURN, HALT,
  INPUT, OUTPUT
};

enum dataType{
  INT, FLOAT, BOOL, ADDRESS, REGISTER
};

enum Register{
  R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15
};

class Stack{
  private:
    int top;
    int capacity;
    string *arr;
  public:
    Stack(){
      arr = new string[SIZE_STACK];
		  capacity = SIZE_STACK;
		  top = -1;
    }
    void push(string);
    string pop();
    string peek();
    bool isEmpty();
    bool isFull();
    int size();
};
class VM{
  public:
	  VM() {}
	  void run(string filename);
};
class CPU{
  private:
    string InstMemory[SIZE_INSTRUCTION_MEMORY];
    string *StaticMemory;
    string reg[15];
    Stack stack;
    string *reg_ID = new string;
    string dest, src;
    int count;
  public:
    CPU(){
      reg_ID = &InstMemory[0];
      StaticMemory = new string[SIZE_STATIC_MEMORY];
      dest = "";
      src = "";
      count = 0;
    }
    void doit(string);

    void readfile(string filename);
    int CheckInst(string *ID);
    void AssignData(string *ID, string &dest, string &src);

    int StrtoInt(string str);
    float StrtoFloat(string str);
    bool StrtoBool(string str);

    int getAddress(string str);
    int getIndex(string str);

    int CheckType(string str);
    int CheckRegister(string str);
    //void SetValue(string str);
    //  Arithmetic instructions
    void Add(string, string);
	  void Minus(string, string);
	  void Mul(string, string);
	  void Div(string, string);
    // Comparison instructions
    void CmpEQ(string, string);
    void CmpNE(string, string);
    void CmpLT(string, string);
    void CmpLE(string, string);
    void CmpGT(string, string);
    void CmpGE(string, string);
    // Logical instructions
    void Not(string);
    void And(string, string);
    void Or(string, string);
    // Load and store instructions
    void Move(string, string);
    void Load(string, string);
    void Store(string, string);
    // Sequence control instructions
    void Jump(string);
    void JumpIf(string, string);
    void Call(string);
    void Return();
    void Halt();
    // Input and Output instructions
    void Input(string);
    void Output(string);
};
#endif