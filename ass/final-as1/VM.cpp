#include "VM.h"
//define Stack
void Stack::push(string str){
	arr[++top] = str;
}
string Stack::pop(){
	return arr[top--];
}
bool Stack::isEmpty(){
	return top == -1;
}
bool Stack::isFull(){
	return top == capacity - 1;
}
///////////////////////////
bool checkIns(string str){
	if(str == "Add" || str == "Minus" || str == "Mul" || str == "Div" ||
	 str == "CmpEQ" || str == "CmpNE" || str == "CmpLT" || str == "CmpLE" || str == "CmpGT" || str == "CmpGE"
	|| str == "Not" || str == "And" || str == "Or"
	|| str == "Move" || str == "Load" || str == "Store"
	|| str == "Jump" || str == "JumpIf" || str == "Call" || str == "Return" || str == "Halt"
	|| str == "Input" || str == "Output") return true;
	else return false;
}
int VM::StrtoInt(string str){
	return stoi(str);
}
float VM::StrtoFloat(string str){
	return stof(str);	
}
bool VM::StrtoBool(string str){
	if(str == "true") return true;
    else return false;
}
int VM::CheckType(string str){
	if(str == "true" || str == "false") return BOOL;
	else if(str[str.length()-1] == 'A' && str[0] != 'R'){
		for(int i = 0; i < (int)str.length() - 1; i++){
			if((int)str[i] < 48 || (int)str[i] > 57)
                    throw InvalidOperand(IP);            
		}
        return ADDRESS; // address
    } 
	else if(str[0] == 'R'){
		for(int i = 1; i < (int)str.length() - 1; i++){
			if((int)str[i] < 48 || (int)str[i] > 57)
                    throw InvalidOperand(IP);            
		}
        if(stoi(str.substr(1)) > 0 && stoi(str.substr(1)) <= 15)
            return REGISTER;
        else throw InvalidOperand(IP);
    } 
	else{
		int a = 0;
		for(int i = 0; i<(int)str.length(); i++){
			if((int)str[i] < 48 || (int)str[i] > 57){
                if((int)str[i] != 46)
                    throw InvalidOperand(IP);
				else{
					a++;
				}
            }
		}
        if(a == 1){
            return FLOAT;
        }
        else if(a == 0){
            return INT;
        }
        else throw InvalidOperand(IP);
    }
	throw InvalidOperand(IP);
}
int VM::checkTypeInput(string str){
	if(str == "true" || str == "false") return BOOL;
	else{
        int a = 0;
		for(int i = 0; i<(int)str.length(); i++){
			if((int)str[i] < 48 || (int)str[i] > 57){
                if((int)str[i] != 46)
                    throw InvalidInput(IP);
				else{
					a++;
				}
            }
		}
        if(a == 1){
            return FLOAT;
        }
        else if(a == 0){
            return INT;
        }
        else throw InvalidInput(IP);
    }
	throw InvalidInput(IP);
}
int getAddress(string str){
	return stoi(str.substr(0,str.length()-1));
}
///////////////////////
void VM::readfile(string filename){
	string tmp;
	ifstream file(filename);
	while(!file.eof()){
		getline(file, tmp);
		load(tmp);
	}
	file.close();
}

void VM::load(string instruction){
	Instruction tmp;

    char *cstr = new char[instruction.length() + 1];
    strcpy(cstr, instruction.c_str());

    if(cstr[0] == ' ' || cstr[strlen(cstr) -1] == ' ' || cstr[0] == ',' || cstr[strlen(cstr) -1] == ',')
		throw InvalidInstruction(IP);
	
    char* tok = strtok(cstr, " ,");
    tmp.code = string(tok);
	if(!checkIns(tmp.code)) throw InvalidInstruction(IP);
	if(tmp.code != "Return" && tmp.code != "Halt" && instruction[tmp.code.length()] != ' ')
		throw InvalidInstruction(IP);
	for(size_t i = tmp.code.length(); i < instruction.length() - 1; i++){
		if((instruction[i] == ' ' && instruction[i+1] == ' ')
		|| (instruction[i] == ' ' && instruction[i+1] == ',')
		|| (instruction[i] == ',' && instruction[i+1] == ','))
			throw InvalidInstruction(IP);
	}

	int i = 0;
	while (tok != NULL) {
		tok = strtok(NULL, " ,");
		if(tok != NULL){
			if(i == 0){
				tmp.op1 = string(tok);
				if(tmp.code == "Halt" || tmp.code == "Return")
					throw InvalidInstruction(IP);
				if(tmp.code != "Output" && tmp.code != "Jump" && tmp.code != "Call"){
					if(CheckType(tmp.op1) != REGISTER) throw InvalidOperand(IP);
					if(tmp.code != "Input"){
						if(instruction[tmp.code.length() + tmp.op1.length() +1] != ',' 
						|| instruction[tmp.code.length() + tmp.op1.length() +2] != ' ')
							throw InvalidInstruction(IP);
					}
				}
			}   
			else if(i == 1){
				tmp.op2 = string(tok);
				if(tmp.code == "Output" || tmp.code == "Jump" || tmp.code == "Call" || tmp.code == "Input")
					throw InvalidInstruction(IP);
				
				CheckType(tmp.op2);
			}
			else throw InvalidInstruction(IP);
		}
		i++;
	}
	this->codeMemory[codeLoadedIndex] = tmp;
    codeLoadedIndex++;
}

//  Arithmetic instructions
void VM::Add(string dest, string src){
	if(src[0] == 'R'){
		if(reg[stoi(dest.substr(1)) - 1].type == INT && reg[stoi(src.substr(1)) - 1].type == INT)
			reg[stoi(dest.substr(1)) - 1].data1 += reg[stoi(src.substr(1)) - 1].data1;

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && reg[stoi(src.substr(1)) - 1].type == INT)
			reg[stoi(dest.substr(1)) - 1].data3 += (float)reg[stoi(src.substr(1)) - 1].data1;

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && reg[stoi(src.substr(1)) - 1].type == FLOAT)
			reg[stoi(dest.substr(1)) - 1].data3 += reg[stoi(src.substr(1)) - 1].data3;

		else if(reg[stoi(dest.substr(1)) - 1].type == INT && reg[stoi(src.substr(1)) - 1].type == FLOAT){
			reg[stoi(dest.substr(1)) - 1].data3 = (float)reg[stoi(dest.substr(1)) - 1].data1 
												+ reg[stoi(src.substr(1)) - 1].data3;
			reg[stoi(dest.substr(1)) - 1].type = FLOAT;
		}
		else throw TypeMismatch(IP);
	}
	else{
		if(reg[stoi(dest.substr(1)) - 1].type == INT && CheckType(src) == INT)
			reg[stoi(dest.substr(1)) - 1].data1 += stoi(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && CheckType(src) == INT)
			reg[stoi(dest.substr(1)) - 1].data3 += (float)stoi(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && CheckType(src) == FLOAT)
			reg[stoi(dest.substr(1)) - 1].data3 += stof(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == INT && CheckType(src) == FLOAT){
			reg[stoi(dest.substr(1)) - 1].data3 = (float)reg[stoi(dest.substr(1)) - 1].data1 + stof(src);
			reg[stoi(dest.substr(1)) - 1].type = FLOAT;
		}
		else throw TypeMismatch(IP);
	}
	this->IP++;
}
void VM::Minus(string dest, string src){
	if(src[0] == 'R'){
		if(reg[stoi(dest.substr(1)) - 1].type == INT && reg[stoi(src.substr(1)) - 1].type == INT)
			reg[stoi(dest.substr(1)) - 1].data1 -= reg[stoi(src.substr(1)) - 1].data1;

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && reg[stoi(src.substr(1)) - 1].type == INT)
			reg[stoi(dest.substr(1)) - 1].data3 -= (float)reg[stoi(src.substr(1)) - 1].data1;

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && reg[stoi(src.substr(1)) - 1].type == FLOAT)
			reg[stoi(dest.substr(1)) - 1].data3 -= reg[stoi(src.substr(1)) - 1].data3;

		else if(reg[stoi(dest.substr(1)) - 1].type == INT && reg[stoi(src.substr(1)) - 1].type == FLOAT){
			reg[stoi(dest.substr(1)) - 1].data3 = (float)reg[stoi(dest.substr(1)) - 1].data1 
												- reg[stoi(src.substr(1)) - 1].data3;
			reg[stoi(dest.substr(1)) - 1].type = FLOAT;
		}
		else throw TypeMismatch(IP);
	}
	else{
		if(reg[stoi(dest.substr(1)) - 1].type == INT && CheckType(src) == INT)
			reg[stoi(dest.substr(1)) - 1].data1 -= stoi(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && CheckType(src) == INT)
			reg[stoi(dest.substr(1)) - 1].data3 -= (float)stoi(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && CheckType(src) == FLOAT)
			reg[stoi(dest.substr(1)) - 1].data3 -= stof(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == INT && CheckType(src) == FLOAT){
			reg[stoi(dest.substr(1)) - 1].data3 = (float)reg[stoi(dest.substr(1)) - 1].data1 - stof(src);
			reg[stoi(dest.substr(1)) - 1].type = FLOAT;
		}
		else throw TypeMismatch(IP);
	}
	this->IP++;
}
void VM::Mul(string dest, string src){
	if(src[0] == 'R'){
		if(reg[stoi(dest.substr(1)) - 1].type == INT && reg[stoi(src.substr(1)) - 1].type == INT)
			reg[stoi(dest.substr(1)) - 1].data1 *= reg[stoi(src.substr(1)) - 1].data1;

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && reg[stoi(src.substr(1)) - 1].type == INT)
			reg[stoi(dest.substr(1)) - 1].data3 *= (float)reg[stoi(src.substr(1)) - 1].data1;

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && reg[stoi(src.substr(1)) - 1].type == FLOAT)
			reg[stoi(dest.substr(1)) - 1].data3 *= reg[stoi(src.substr(1)) - 1].data3;

		else if(reg[stoi(dest.substr(1)) - 1].type == INT && reg[stoi(src.substr(1)) - 1].type == FLOAT){
			reg[stoi(dest.substr(1)) - 1].data3 = (float)reg[stoi(dest.substr(1)) - 1].data1 
												* reg[stoi(src.substr(1)) - 1].data3;
			reg[stoi(dest.substr(1)) - 1].type = FLOAT;
		}
		else throw TypeMismatch(IP);
	}
	else{
		if(reg[stoi(dest.substr(1)) - 1].type == INT && CheckType(src) == INT)
			reg[stoi(dest.substr(1)) - 1].data1 *= stoi(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && CheckType(src) == INT)
			reg[stoi(dest.substr(1)) - 1].data3 *= (float)stoi(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && CheckType(src) == FLOAT)
			reg[stoi(dest.substr(1)) - 1].data3 *= stof(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == INT && CheckType(src) == FLOAT){
			reg[stoi(dest.substr(1)) - 1].data3 = (float)reg[stoi(dest.substr(1)) - 1].data1 * stof(src);
			reg[stoi(dest.substr(1)) - 1].type = FLOAT;
		}
		else throw TypeMismatch(IP);
	}
	this->IP++;
}
void VM::Div(string dest, string src){
	if(src[0] == 'R'){
		if(reg[stoi(dest.substr(1)) - 1].type == INT && reg[stoi(src.substr(1)) - 1].type == INT){
			if(reg[stoi(src.substr(1)) - 1].data1 == 0) throw DivideByZero(IP);
			else
				reg[stoi(dest.substr(1)) - 1].data1 /= reg[stoi(src.substr(1)) - 1].data1;
		}
		
		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && reg[stoi(src.substr(1)) - 1].type == INT){
			if(reg[stoi(src.substr(1)) - 1].data1 == 0) throw DivideByZero(IP);
			else
				reg[stoi(dest.substr(1)) - 1].data3 /= (float)reg[stoi(src.substr(1)) - 1].data1;
		}
		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && reg[stoi(src.substr(1)) - 1].type == FLOAT){
			if(reg[stoi(src.substr(1)) - 1].data3 == 0.0) throw DivideByZero(IP);
			else
				reg[stoi(dest.substr(1)) - 1].data3 /= reg[stoi(src.substr(1)) - 1].data3;
		}
		else if(reg[stoi(dest.substr(1)) - 1].type == INT && reg[stoi(src.substr(1)) - 1].type == FLOAT){
			if(reg[stoi(src.substr(1)) - 1].data3 == 0.0) throw DivideByZero(IP);
			else{
				reg[stoi(dest.substr(1)) - 1].data3 = (float)reg[stoi(dest.substr(1)) - 1].data1 
													/ reg[stoi(src.substr(1)) - 1].data3;
				reg[stoi(dest.substr(1)) - 1].type = FLOAT;
			}
		}
		else throw TypeMismatch(IP);
	}
	else{
		if(reg[stoi(dest.substr(1)) - 1].type == INT && CheckType(src) == INT)
			stoi(src) == 0 ? throw DivideByZero(IP) : reg[stoi(dest.substr(1)) - 1].data1 /= stoi(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && CheckType(src) == INT)
			stoi(src) == 0 ? throw DivideByZero(IP) : reg[stoi(dest.substr(1)) - 1].data3 /= (float)stoi(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == FLOAT && CheckType(src) == FLOAT)
			stof(src) == 0.0 ? throw DivideByZero(IP) : reg[stoi(dest.substr(1)) - 1].data3 /= stof(src);

		else if(reg[stoi(dest.substr(1)) - 1].type == INT && CheckType(src) == FLOAT){
			if(stoi(src) == 0) throw DivideByZero(IP);
			else{
			reg[stoi(dest.substr(1)) - 1].data3 = (float)reg[stoi(dest.substr(1)) - 1].data1 / stof(src);
			reg[stoi(dest.substr(1)) - 1].type = FLOAT;
			}
		}
		else throw TypeMismatch(IP);
	}
	this->IP++;
}
// Comparison instructions
void VM::CmpEQ(string dest, string src){
	if(src[0] == 'R'){
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL && reg[stoi(src.substr(1)) - 1].type != BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type != BOOL && reg[stoi(src.substr(1)) - 1].type == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || reg[stoi(src.substr(1)) - 1].type == ADDRESS)){
			throw TypeMismatch(IP);
		}
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case BOOL:{
					if(reg[stoi(dest.substr(1)) - 1].data2 == reg[stoi(src.substr(1)) - 1].data2)
						reg[stoi(dest.substr(1)) - 1].data2 = true;
					else reg[stoi(dest.substr(1)) - 1].data2 = false;
				}
					break;
				case INT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 == reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 == reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 == (float)reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 == reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	else{
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL && CheckType(src) != BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type != BOOL && CheckType(src) == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || CheckType(src) == ADDRESS))
			throw TypeMismatch(IP);
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case BOOL:{
					if(reg[stoi(dest.substr(1)) - 1].data2 == StrtoBool(src))
						reg[stoi(dest.substr(1)) - 1].data2 = true;
					else reg[stoi(dest.substr(1)) - 1].data2 = false;
				}
					break;
				case INT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 == stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 == stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 == (float)stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 == stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	this->IP++;
}
void VM::CmpNE(string dest, string src){
	if(src[0] == 'R'){
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL && reg[stoi(src.substr(1)) - 1].type != BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type != BOOL && reg[stoi(src.substr(1)) - 1].type == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || reg[stoi(src.substr(1)) - 1].type == ADDRESS)){
			throw TypeMismatch(IP);
		}
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case BOOL:{
					if(reg[stoi(dest.substr(1)) - 1].data2 != reg[stoi(src.substr(1)) - 1].data2)
						reg[stoi(dest.substr(1)) - 1].data2 = true;
					else reg[stoi(dest.substr(1)) - 1].data2 = false;
				}
					break;
				case INT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 != reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 != reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 != (float)reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 != reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	else{
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL && CheckType(src) != BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type != BOOL && CheckType(src) == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || CheckType(src) == ADDRESS))
			throw TypeMismatch(IP);
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case BOOL:{
					if(reg[stoi(dest.substr(1)) - 1].data2 != StrtoBool(src))
						reg[stoi(dest.substr(1)) - 1].data2 = true;
					else reg[stoi(dest.substr(1)) - 1].data2 = false;
				}
					break;
				case INT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 != stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 != stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 != (float)stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 != stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	this->IP++;
}
void VM::CmpLT(string dest, string src){
	if(src[0] == 'R'){
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL || reg[stoi(src.substr(1)) - 1].type == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || reg[stoi(src.substr(1)) - 1].type == ADDRESS)){
			throw TypeMismatch(IP);
		}
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case INT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 < reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 < reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 < (float)reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 < reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	else{
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL || CheckType(src) == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || CheckType(src) == ADDRESS))
			throw TypeMismatch(IP);
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case INT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 < stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 < stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 < (float)stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 < stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	this->IP++;
}
void VM::CmpLE(string dest, string src){
	if(src[0] == 'R'){
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL || reg[stoi(src.substr(1)) - 1].type == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || reg[stoi(src.substr(1)) - 1].type == ADDRESS)){
			throw TypeMismatch(IP);
		}
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case INT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 <= reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 <= reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 <= (float)reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 <= reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	else{
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL || CheckType(src) == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || CheckType(src) == ADDRESS))
			throw TypeMismatch(IP);
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case INT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 <= stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 <= stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 <= (float)stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 <= stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	this->IP++;
}
void VM::CmpGT(string dest, string src){
	if(src[0] == 'R'){
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL || reg[stoi(src.substr(1)) - 1].type == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || reg[stoi(src.substr(1)) - 1].type == ADDRESS)){
			throw TypeMismatch(IP);
		}
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case INT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 > reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 > reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 > (float)reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 > reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	else{
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL || CheckType(src) == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || CheckType(src) == ADDRESS))
			throw TypeMismatch(IP);
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case INT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 > stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 > stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 > (float)stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 > stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	this->IP++;
}
void VM::CmpGE(string dest, string src){
	if(src[0] == 'R'){
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL || reg[stoi(src.substr(1)) - 1].type == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || reg[stoi(src.substr(1)) - 1].type == ADDRESS)){
			throw TypeMismatch(IP);
		}
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case INT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 >= reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 >= reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(reg[stoi(src.substr(1)) - 1].type == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 >= (float)reg[stoi(src.substr(1)) - 1].data1)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 >= reg[stoi(src.substr(1)) - 1].data3)
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	else{
		if((reg[stoi(dest.substr(1)) - 1].type == BOOL || CheckType(src) == BOOL)
		|| (reg[stoi(dest.substr(1)) - 1].type == ADDRESS || CheckType(src) == ADDRESS))
			throw TypeMismatch(IP);
		else{
			switch (reg[stoi(dest.substr(1)) - 1].type){
				case INT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data1 >= stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if((float)reg[stoi(dest.substr(1)) - 1].data1 >= stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				case FLOAT:{
					if(CheckType(src) == INT){
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 >= (float)stoi(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
					else{
						reg[stoi(dest.substr(1)) - 1].type = BOOL;
						if(reg[stoi(dest.substr(1)) - 1].data3 >= stof(src))
							reg[stoi(dest.substr(1)) - 1].data2 = true;
						else reg[stoi(dest.substr(1)) - 1].data2 = false;
					}
				}
					break;
				default:
					break;
			}
		}
	}
	this->IP++;
}
// Logical instructions
void VM::Not(string dest){
	if(reg[stoi(dest.substr(1)) - 1].type != BOOL) throw TypeMismatch(IP);
	else{
		if(reg[stoi(dest.substr(1)) - 1].data2) reg[stoi(dest.substr(1)) - 1].data2 = false;
		else reg[stoi(dest.substr(1)) - 1].data2 = true;
	}
}
void VM::And(string dest, string src){
	if(src[0] == 'R'){
		if(reg[stoi(dest.substr(1)) - 1].type != BOOL || reg[stoi(src.substr(1)) - 1].type != BOOL)
			throw TypeMismatch(IP);
		else{
			if(reg[stoi(dest.substr(1)) - 1].data2 == true && reg[stoi(src.substr(1)) - 1].data2 == true)
				reg[stoi(dest.substr(1)) - 1].data2 = true;
			else reg[stoi(dest.substr(1)) - 1].data2 = false;
		}
	}
	else{
		if(reg[stoi(dest.substr(1)) - 1].type != BOOL || CheckType(src) != BOOL)
			throw TypeMismatch(IP);
		else{
			if(reg[stoi(dest.substr(1)) - 1].data2 == true && src == "true")
				reg[stoi(dest.substr(1)) - 1].data2 = true;
			else reg[stoi(dest.substr(1)) - 1].data2 = false;
		}
	}
	this->IP++;
}
void VM::Or(string dest, string src){
	if(src[0] == 'R'){
		if(reg[stoi(dest.substr(1)) - 1].type != BOOL || reg[stoi(src.substr(1)) - 1].type != BOOL)
			throw TypeMismatch(IP);
		else{
			if(reg[stoi(dest.substr(1)) - 1].data2 == false && reg[stoi(src.substr(1)) - 1].data2 == false)
				reg[stoi(dest.substr(1)) - 1].data2 = false;
			else reg[stoi(dest.substr(1)) - 1].data2 = true;
		}
	}
	else{
		if(reg[stoi(dest.substr(1)) - 1].type != BOOL || CheckType(src) != BOOL)
			throw TypeMismatch(IP);
		else{
			if(reg[stoi(dest.substr(1)) - 1].data2 == false && src == "false")
				reg[stoi(dest.substr(1)) - 1].data2 = false;
			else reg[stoi(dest.substr(1)) - 1].data2 = true;
		}
	}
	this->IP++;
}
// Load and store instructions
void VM::Move(string dest, string src){

	if(src[0] == 'R'){
		reg[stoi(dest.substr(1)) - 1].type = reg[stoi(src.substr(1)) - 1].type;
		switch (reg[stoi(src.substr(1)) - 1].type){
			case INT:
				reg[stoi(dest.substr(1)) - 1].data1 = reg[stoi(src.substr(1)) - 1].data1;
				break;
			case BOOL:
				reg[stoi(dest.substr(1)) - 1].data2 = reg[stoi(src.substr(1)) - 1].data2;
				break;
			case FLOAT:
				reg[stoi(dest.substr(1)) - 1].data3 = reg[stoi(src.substr(1)) - 1].data3;
				break;
			case ADDRESS:
				reg[stoi(dest.substr(1)) - 1].data4 = reg[stoi(src.substr(1)) - 1].data4;
				break;
			default:
				break;
		}
	}
	else{
		switch (CheckType(src))
		{
			case INT:{
				reg[stoi(dest.substr(1)) - 1].data1 = stoi(src);
				reg[stoi(dest.substr(1)) - 1].type = INT;
			}
				break;
			case BOOL:{
				reg[stoi(dest.substr(1)) - 1].data2 = StrtoBool(src);
				reg[stoi(dest.substr(1)) - 1].type = BOOL;
			}
				break;
			case FLOAT:{
				reg[stoi(dest.substr(1)) - 1].data3 = stof(src);
				reg[stoi(dest.substr(1)) - 1].type = FLOAT;
			}
				break;
			case ADDRESS:{
				reg[stoi(dest.substr(1)) - 1].data4 = src;
				reg[stoi(dest.substr(1)) - 1].type = ADDRESS;
			}
				break;
			default:
				break;
		}
	}
	this->IP++;
}
void VM::Load(string dest, string src){
	int a = 0;
	if(src[0] == 'R'){
		if(reg[stoi(src.substr(1)) - 1].type != ADDRESS)
			throw TypeMismatch(IP);
		else
			a = getAddress(reg[stoi(src.substr(1)) - 1].data4);
	}
	else{
		if(CheckType(src) != ADDRESS)
			throw TypeMismatch(IP);
		else
			a = getAddress(src);
	}
	switch (StaticMemory[a].type)
	{
		case INT:{
			reg[stoi(dest.substr(1)) - 1].type = INT;
			reg[stoi(dest.substr(1)) - 1].data1 = StaticMemory[a].data1;
		}
			break;
		case BOOL:{
			reg[stoi(dest.substr(1)) - 1].type = BOOL;
			reg[stoi(dest.substr(1)) - 1].data2 = StaticMemory[a].data2;
		}
			break;
		case FLOAT:{
			reg[stoi(dest.substr(1)) - 1].type = FLOAT;
			reg[stoi(dest.substr(1)) - 1].data3 = StaticMemory[a].data3;
		}
			break;
		default:
			break;
	}
	this->IP++;
}
void VM::Store(string dest, string src){
	int a = 0;
	if(reg[stoi(dest.substr(1)) - 1].type != ADDRESS)
			throw TypeMismatch(IP);
	a = getAddress(reg[stoi(dest.substr(1)) - 1].data4);

	if(src[0] == 'R'){
		switch (reg[stoi(src.substr(1)) - 1].type)
		{
			case INT:{
				StaticMemory[a].type = INT;
				StaticMemory[a].data1 = reg[stoi(src.substr(1)) - 1].data1;
			}
				break;
			case BOOL:{
				StaticMemory[a].type = BOOL;
				StaticMemory[a].data2 = reg[stoi(src.substr(1)) - 1].data2;
			}
				break;
			case FLOAT:{
				StaticMemory[a].type = FLOAT;
				StaticMemory[a].data3 = reg[stoi(src.substr(1)) - 1].data3;
			}
				break;
			default:
				break;
		}
	}
	else{
		switch (CheckType(src))
		{
			case INT:{
				StaticMemory[a].type = INT;
				StaticMemory[a].data1 = stoi(src);
			}
				break;
			case BOOL:{
				StaticMemory[a].type = BOOL;
				StaticMemory[a].data2 = StrtoBool(src);
			}
				break;
			case FLOAT:{
				StaticMemory[a].type = FLOAT;
				StaticMemory[a].data3 = stof(src);
			}
				break;
			default:
				break;
		}
	}
	this->IP++;
}
// Sequence control instructions
void VM::Jump(string src, int &i){
	if(src[0] == 'R'){
		if(reg[stoi(src.substr(1)) - 1].type != ADDRESS)
			throw TypeMismatch(IP);
		else
			i = getAddress(reg[stoi(src.substr(1)) - 1].data4) - 1;
	}
	else{
		if(CheckType(src) != ADDRESS)
			throw TypeMismatch(IP);
		else
			i = getAddress(src) - 1;
	}
	if(i >= codeLoadedIndex - 1)
		throw InvalidDestination(IP);
	this->IP = i;
	this->IP++;
}
void VM::JumpIf(string dest, string src, int &i){
	if(src[0] == 'R'){
		if(reg[stoi(dest.substr(1)) - 1].type != BOOL || reg[stoi(src.substr(1)) - 1].type != ADDRESS)
			throw TypeMismatch(IP);
		else{
			if(reg[stoi(dest.substr(1)) - 1].data2)
				i = getAddress(reg[stoi(src.substr(1)) - 1].data4) - 1;
		}
	}
	else{
		if(reg[stoi(dest.substr(1)) - 1].type != BOOL || CheckType(src) != ADDRESS)
		 	throw TypeMismatch(IP);
		else{
			if(reg[stoi(dest.substr(1)) - 1].data2)
				i = getAddress(src) - 1;
		}
	}
	if(i >= codeLoadedIndex - 1)
		throw InvalidDestination(IP);
	this->IP = i;
	this->IP++;
}
void VM::Call(string src, int &i){
	if(src[0] == 'R'){
		if(reg[stoi(src.substr(1)) - 1].type != ADDRESS)
			throw TypeMismatch(IP);
		else{
			if(stack.isFull())
				throw StackFull(IP);
			else
				stack.push(reg[stoi(src.substr(1)) - 1].data4);
			i = getAddress(reg[stoi(src.substr(1)) - 1].data4) - 1;
		}
	}
	else{
		if(CheckType(src) != ADDRESS)
			throw TypeMismatch(IP);
		else{
			if(stack.isFull())
				throw StackFull(IP);
			else
				stack.push(src);
			i = getAddress(src) - 1;
		}
	}
	if(i >= codeLoadedIndex - 1)
		throw InvalidDestination(IP);
	this->IP = i;
	this->IP++;
}
void VM::Return(){
	if(!(stack.isEmpty())){
		reg[IP].data4 = stack.pop();
		reg[IP].type = ADDRESS;
	}
	this->IP++;
}
// Input and Output instructions
void VM::Input(string dest){
	string tmp;
	cin >> tmp;
	switch (checkTypeInput(tmp))
	{
		case INT:{
			reg[stoi(dest.substr(1)) - 1].type = INT;
			reg[stoi(dest.substr(1)) - 1].data1 = stoi(tmp);
		}
			break;
		case BOOL:{
			reg[stoi(dest.substr(1)) - 1].type = BOOL;
			reg[stoi(dest.substr(1)) - 1].data2 = StrtoBool(tmp);
		}
			break;
		case FLOAT:{
			reg[stoi(dest.substr(1)) - 1].type = FLOAT;
			reg[stoi(dest.substr(1)) - 1].data3 = stof(tmp);
		}
			break;
		default:
			break;
	}
	this->IP++;
}
void VM::Output(string src){
	if(src[0] == 'R'){
		switch (reg[stoi(src.substr(1)) - 1].type)
		{
			case INT:
				cout << reg[stoi(src.substr(1)) - 1].data1;
				break;
			case BOOL:{
				if(reg[stoi(src.substr(1)) - 1].data2)
					cout << "true";
				else cout << "false";
			}
				break;
			case FLOAT:
				cout << reg[stoi(src.substr(1)) - 1].data3;
				break;
			case ADDRESS:
				cout << reg[stoi(src.substr(1)) - 1].data4;
				break;
			default:
				break;
		}
	}
	else cout << src;
    this->IP++;
}

void VM::run(string filename)
{
	readfile(filename);
	for(int i = 0; i < codeLoadedIndex; i++){
        Instruction ins = codeMemory[IP];
		//cout << ins.code << ".." << ins.op1 << ".." << ins.op2 << endl;
		if(ins.code == "Add") this->Add(ins.op1, ins.op2);
		else if(ins.code == "Minus") this->Minus(ins.op1, ins.op2);
		else if(ins.code == "Mul") this->Mul(ins.op1, ins.op2);
		else if(ins.code == "Div") this->Div(ins.op1, ins.op2);
		else if(ins.code == "CmpEQ") this->CmpEQ(ins.op1, ins.op2);
		else if(ins.code == "CmpNE") this->CmpNE(ins.op1, ins.op2);
		else if(ins.code == "CmpLT") this->CmpLT(ins.op1, ins.op2);
		else if(ins.code == "CmpLE") this->CmpLE(ins.op1, ins.op2);
		else if(ins.code == "CmpGT") this->CmpGT(ins.op1, ins.op2);
		else if(ins.code == "CmpGE") this->CmpGE(ins.op1, ins.op2);
		else if(ins.code == "Not") this->Not(ins.op1);
        else if(ins.code == "And") this->And(ins.op1, ins.op2);
		else if(ins.code == "Or") this->Or(ins.op1, ins.op2);
		else if(ins.code == "Move") this->Move(ins.op1, ins.op2);
        else if(ins.code == "Load") this->Load(ins.op1, ins.op2);
		else if(ins.code == "Store") this->Store(ins.op1, ins.op2);
        else if(ins.code == "Jump") this->Jump(ins.op1, i);
        else if(ins.code == "JumpIf") this->JumpIf(ins.op1, ins.op2, i);
		else if(ins.code == "Call") this->Call(ins.op1, i);
        else if(ins.code == "Return") this->Return();
		else if(ins.code == "Halt") break;
		else if(ins.code == "Input") this->Input(ins.op1);
        else if(ins.code == "Output") this->Output(ins.op1);
	}
}