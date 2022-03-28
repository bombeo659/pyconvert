#ifndef VM_H
#define VM_H

#include "main.h"
enum Datatype{NONE, INT , FLOAT, BOOL, ADDRESS, REGISTER};
struct Greg{
  int data1;
  float data2;
  bool data3;
  Datatype type;
};
struct Instruction{
  string code;
  string opcode1;
  string opcode2;
};

class VM 
{
  
private:
  int IP;
  Greg reg[15];
  Instruction codeMemory[65536];
  int codeloadIndex ;

public:

 string array[21] ={"Add","Minus","Mul","Div","CmpEQ",
 "CmpNE","CmpLT","CmpLE","CmpGT","CmpGE","Not","And",
 "Or","Move","Load","Store","Jump","JumpIf","Call","Return","Output"};
VM() { 
        this->IP = 0;
        for (int i = 0; i < 15; i++) {
            reg[i].type = NONE;
        }
        this->codeloadIndex = 0;
    }
  void datatype();
  void excec();
  void readfilename(string);
  void add(string, string);
  void minus(string, string);
  void div(string, string);
  void move(string , string);
  void mul(string, string);
  void output(string);
  void CmpEQ(string, string);
  void CmpNE(string, string);
  void CmpLT(string, string);
  void CmpLE(string, string);
  void CmpGT(string, string);
  void CmpGE(string, string);
  void Not(string);
  void And(string, string);
  void Or(string, string);
  int checktypedata(string);
  bool checkreg(string);
  bool stringtobool(string);
  bool checkloaded1(string);
  bool checkloaded2(string);
  void run(string filename);
  void load(string );
};
#endif