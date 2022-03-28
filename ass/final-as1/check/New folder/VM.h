#ifndef VM_H
#define VM_H

#include "main.h"


class VM
{
public:
	VM() {}
	void run(string filename);
};
class Stack{
private:
	string *arr;
	int top;
	int capacity;
public:
	Stack(){
		arr=new string[1000];
		capacity=1000;
		top=-1;
	}
	
	void push(string);
	string pop();
	string peek();
	int size();
	bool isEmpty();
	bool isFull();
};
class CPU
{
private:
	string R[15];
	string *sMemory=new string[65536];
	Stack stack;
	int IP=0;
public:
	int ip(){
		return this->IP;
	}
//lenh so hoc
	void Add(string, string);
	void Minus(string, string);
	void Mul(string, string);
	void Div(string, string);
//lenh so sanh
	void CmpEQ(string, string);
	void CmpNE(string, string);
	void CmpLT(string, string);
	void CmpLE(string, string);
	void CmpGT(string, string);
	void CmpGE(string, string);
//lenh luan ly
    void Not(string);
	void And(string, string);
	void Or(string, string);
//lenh doc va ghi
	void Move(string, string);
	void Load(string, string);
	void Store(string, string);
//lenh dieu khien
	void Jump(string);
	void JumpIf(string, string);
	void Call(string);
	void Return();
//lenh nhap xuat
	void Input(string);
	void Output(string);

};
#endif