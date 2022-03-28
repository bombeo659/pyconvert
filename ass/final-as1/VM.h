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

enum DataType { NONE, INT, BOOL, FLOAT, ADDRESS, REGISTER};

struct GReg {
    int data1;
    bool data2;
    float data3;
    string data4;
    DataType type;
};

struct Instruction {
    string code;
    string op1;
    string op2;
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
    bool isEmpty();
    bool isFull();
};

class VM {
  private:
    int IP; // IP register
    GReg reg[15]; // General register: R1 -> R15
    Instruction codeMemory[SIZE_INSTRUCTION_MEMORY];
    int codeLoadedIndex;
    GReg StaticMemory[SIZE_STATIC_MEMORY]; // Static Memory
    Stack stack;
public:
  VM(){
    this->IP = 0;
    for (int i = 0; i < 15; i++) {
      reg[i].type = NONE;
    }
    this->codeLoadedIndex = 0;
  }

  int StrtoInt(string str);
  float StrtoFloat(string str);
  bool StrtoBool(string str);
  int CheckType(string);
  int checkTypeInput(string);

  void run(string filename);
  void readfile(string filename);
  void load(string);
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
  void Jump(string, int&);
  void JumpIf(string, string, int&);
  void Call(string, int&);
  void Return();
  //void Halt();
  // Input and Output instructions
  void Input(string);
  void Output(string);
};
#endif