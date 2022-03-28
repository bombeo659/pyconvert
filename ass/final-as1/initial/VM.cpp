#include "VM.h"
//define Stack
void Stack::push(string str){
	if(isFull()){
		//exit(EXIT_FAILURE);
		return;//sau nay quang loi StackFull
	}
	arr[++top] = str;
}
string Stack::pop(){
	if(isEmpty()){
		exit(EXIT_FAILURE);
	}
	return arr[top--];
}
string Stack::peek(){
	if(!isEmpty()){
		return arr[top];
	}
	else{
		exit(EXIT_FAILURE);
	}
}
int Stack::size(){
	return top+1;
}
bool Stack::isEmpty(){
	return top == -1;
}
bool Stack::isFull(){
	return top == capacity - 1;
}
///////////////////////////
void CPU::readfile(string filename){
	int i = 0;
	ifstream file(filename);
	while(!file.eof()){
		getline(file, InstMemory[i]);
		i++;
	}
	file.close();
}

void CPU::AssignData(string *ID, string &dest, string &src){
	string str = *ID;
	if(str.find(" ") != -1){
		for (int i = str.find(" ") +1; i < str.length(); i++){
			dest += str[i];
			if(str[i+1] == ','){
				for(int j = i+3; j<str.length(); j++){
					src += str[j];
				}
				i = str.length() + 100;
			}
		}
	}
}

int CPU::CheckInst(string *ID){
	string str = *ID;
	if(str.find("Add") != -1) return ADD;
	else if(str.find("Minus") != -1) return MINUS;
	else if(str.find("Mul") != -1) return MUL;
	else if(str.find("Div") != -1) return DIV;
	else if(str.find("CmpEQ") != -1) return CMPEQ;
	else if(str.find("CmpNE") != -1) return CMPNE;
	else if(str.find("CmpLT") != -1) return CMPLT;
	else if(str.find("CmpLE") != -1) return CMPLE;
	else if(str.find("CmpGT") != -1) return CMPGT;
	else if(str.find("CmpGE") != -1) return CMPGE;
	else if(str.find("Not") != -1) {
		if(str.length() != 6) throw InvalidDestination(count); //////// errors
		else return NOT;
	}
	else if(str.find("And") != -1) return AND;
	else if(str.find("Or") != -1) return OR;
	else if(str.find("Move") != -1) return MOVE;
	else if(str.find("Load") != -1) return LOAD;
	else if(str.find("Store") != -1) return STORE;
	else if(str.find("Jump") != -1) {
		if(str.length() != 7) throw InvalidDestination(count);
		else return JUMP;
	}
	else if(str.find("JumpIf") != -1) return JUMPIF;
	else if(str.find("Call") != -1) {
		if(str.length() != 7) throw InvalidDestination(count);
		else return CALL;
	}
	else if(str.find("Return") != -1){
		if(str.find(" ") != -1) throw InvalidDestination(count);
		else return RETURN;
	}
	else if(str.find("Halt") != -1) {
		if(str.find(" ") != -1) throw InvalidDestination(count);
		else return HALT;
	}
	else if(str.find("Input") != -1) {
		if(str.length() != 8) throw InvalidDestination(count);
		else return INPUT;
	}
	else if(str.find("Output") != -1) {
		if(str.length() != 9) throw InvalidDestination(count);
		else return OUTPUT;
	}
	else throw InvalidDestination(count);
}

/////////////////////////////////
int CPU::StrtoInt(string str){
	return stoi(str);
}
float CPU::StrtoFloat(string str){
	return stof(str);	
}
bool CPU::StrtoBool(string str){
	if(str.find("true") != -1) return true;
    else return false;
}
////////////////////
int CPU::getAddress(string str){
	return StrtoInt(str.substr(0,str.length()-1));
}
int CPU::getIndex(string str){
	if(str.length()==2) {
		return StrtoInt(str.substr(1,1)) - 1;
	}
	else
		return StrtoInt(str.substr(1,2)) - 1;	
}
////////////////////////////////////
int CPU::CheckType(string str){
	if(str.find(".") != -1) return FLOAT;
	else if(str.find("true") != -1 || str.find("false") != -1) return BOOL;
	else if(str.find("A") != -1) return ADDRESS; // address 
	else if(str.find("R") != -1) return REGISTER;
	else return INT; 
}
int CPU::CheckRegister(string str){
	if(str == "R1") return R1;
	else if(str == "R2") return R2;
	else if(str == "R3") return R3;
	else if(str == "R4") return R4;
	else if(str == "R5") return R5;
	else if(str == "R6") return R6;
	else if(str == "R7") return R7;
	else if(str == "R8") return R8;
	else if(str == "R9") return R9;
	else if(str == "R10") return R10;
	else if(str == "R11") return R11;
	else if(str == "R12") return R12;
	else if(str == "R13") return R13;
	else if(str == "R14") return R14;
	else if(str == "R15") return R15;
	else throw InvalidInstruction(count);
}
//  Arithmetic instructions
void CPU::Add(string dest, string src){
	if(CheckType(src) == REGISTER){
		if(CheckType(reg[getIndex(src)]) != INT && CheckType(reg[getIndex(src)]) != FLOAT || CheckType(reg[CheckRegister(dest)]) != INT && CheckType(reg[CheckRegister(dest)]) != FLOAT)
			throw TypeMismatch(count);
		else{
			if(CheckType(reg[getIndex(src)]) == INT && CheckType(reg[CheckRegister(dest)]) == INT)
				reg[CheckRegister(dest)] = to_string(StrtoInt(reg[getIndex(src)]) + StrtoInt(reg[CheckRegister(dest)]));
			else
				reg[CheckRegister(dest)] = to_string(StrtoFloat(reg[getIndex(src)]) + StrtoFloat(reg[CheckRegister(dest)]));
		}
	}
	else{
		if(CheckType(src) != INT && CheckType(src) != FLOAT || CheckType(reg[CheckRegister(dest)]) != INT && CheckType(reg[CheckRegister(dest)]) != FLOAT)
			throw TypeMismatch(count);
		else {
			if(CheckType(src) == INT && CheckType(reg[CheckRegister(dest)]) == INT)
				reg[CheckRegister(dest)] = to_string(StrtoInt(src) + StrtoInt(reg[CheckRegister(dest)]));
			else
				reg[CheckRegister(dest)] = to_string(StrtoFloat(src) + StrtoFloat(reg[CheckRegister(dest)]));
		}
	}
}
void CPU::Minus(string dest, string src){
	if(CheckType(src) == REGISTER){
		if(CheckType(reg[getIndex(src)]) != INT && CheckType(reg[getIndex(src)]) != FLOAT || CheckType(reg[CheckRegister(dest)]) != INT && CheckType(reg[CheckRegister(dest)]) != FLOAT)
			throw TypeMismatch(count);
		else{
			if(CheckType(reg[getIndex(src)]) == INT && CheckType(reg[CheckRegister(dest)]) == INT)
				reg[CheckRegister(dest)] = to_string(- StrtoInt(reg[getIndex(src)]) + StrtoInt(reg[CheckRegister(dest)]));
			else
				reg[CheckRegister(dest)] = to_string(- StrtoFloat(reg[getIndex(src)]) + StrtoFloat(reg[CheckRegister(dest)]));
		}
	}
	else{
		if(CheckType(src) != INT && CheckType(src) != FLOAT || CheckType(reg[CheckRegister(dest)]) != INT && CheckType(reg[CheckRegister(dest)]) != FLOAT)
			throw TypeMismatch(count);
		else {
			if(CheckType(src) == INT && CheckType(reg[CheckRegister(dest)]) == INT)
				reg[CheckRegister(dest)] = to_string(- StrtoInt(src) + StrtoInt(reg[CheckRegister(dest)]));
			else
				reg[CheckRegister(dest)] = to_string(- StrtoFloat(src) + StrtoFloat(reg[CheckRegister(dest)]));
		}
	}
}
void CPU::Mul(string dest, string src){
	if(CheckType(src) == REGISTER){
		if(CheckType(reg[getIndex(src)]) != INT && CheckType(reg[getIndex(src)]) != FLOAT || CheckType(reg[CheckRegister(dest)]) != INT && CheckType(reg[CheckRegister(dest)]) != FLOAT)
			throw TypeMismatch(count);
		else{
			if(CheckType(reg[getIndex(src)]) == INT && CheckType(reg[CheckRegister(dest)]) == INT)
				reg[CheckRegister(dest)] = to_string(StrtoInt(reg[getIndex(src)]) * StrtoInt(reg[CheckRegister(dest)]));
			else
				reg[CheckRegister(dest)] = to_string(StrtoFloat(reg[getIndex(src)]) * StrtoFloat(reg[CheckRegister(dest)]));
		}
	}
	else{
		if(CheckType(src) != INT && CheckType(src) != FLOAT || CheckType(reg[CheckRegister(dest)]) != INT && CheckType(reg[CheckRegister(dest)]) != FLOAT)
			throw TypeMismatch(count);
		else {
			if(CheckType(src) == INT && CheckType(reg[CheckRegister(dest)]) == INT)
				reg[CheckRegister(dest)] = to_string(StrtoInt(src) * StrtoInt(reg[CheckRegister(dest)]));
			else
				reg[CheckRegister(dest)] = to_string(StrtoFloat(src) * StrtoFloat(reg[CheckRegister(dest)]));
		}
	}
}
void CPU::Div(string dest, string scr){
	if(CheckType(src) == REGISTER){
		
		if(CheckType(reg[getIndex(src)]) != INT && CheckType(reg[getIndex(src)]) != FLOAT || CheckType(reg[CheckRegister(dest)]) != INT && CheckType(reg[CheckRegister(dest)]) != FLOAT)
			throw TypeMismatch(count);
		else if(StrtoInt(reg[getIndex(src)]) == 0 || StrtoFloat(reg[getIndex(src)]) == 0.0)
			throw DivideByZero(count);
		else{
			if(CheckType(reg[getIndex(src)]) == INT && CheckType(reg[CheckRegister(dest)]) == INT)
				reg[CheckRegister(dest)] = to_string(StrtoInt(reg[CheckRegister(dest)])/StrtoInt(reg[getIndex(src)]));
			else
				reg[CheckRegister(dest)] = to_string(StrtoFloat(reg[CheckRegister(dest)])/StrtoFloat(reg[getIndex(src)]));
		}
	}
	else{
		
		if(CheckType(src) != INT && CheckType(src) != FLOAT || CheckType(reg[CheckRegister(dest)]) != INT && CheckType(reg[CheckRegister(dest)]) != FLOAT)
			throw TypeMismatch(count);
		else if(StrtoInt(reg[getIndex(src)]) == 0 || StrtoFloat(reg[getIndex(src)]) == 0.0)
			throw DivideByZero(count);
		else {
			if(CheckType(src) == INT && CheckType(reg[CheckRegister(dest)]) == INT)
				reg[CheckRegister(dest)] = to_string(StrtoInt(reg[CheckRegister(dest)])/StrtoInt(src));
			else
				reg[CheckRegister(dest)] = to_string(StrtoFloat(reg[CheckRegister(dest)])/StrtoFloat(src));
		}
	}
}
///////////////////////////////
// Comparison instructions
void CPU::CmpEQ(string dest, string src){

}
void CPU::CmpNE(string dest, string src){

}
void CPU::CmpLT(string dest, string src){

}
void CPU::CmpLE(string dest, string src){

}
void CPU::CmpGT(string dest, string src){

}
void CPU::CmpGE(string dest, string src){

}
////////////////////////////////
// Logical instructions
void CPU::Not(string dest){
	if(CheckType(reg[CheckRegister(dest)]) != BOOL) throw TypeMismatch(count);
	else {
		if(reg[CheckRegister(dest)] == "true") reg[CheckRegister(dest)] = "false";
		else reg[CheckRegister(dest)] = "true";
	}
}
void CPU::And(string dest, string src){
	if(CheckType(src) == REGISTER){
		if(CheckType(reg[getIndex(src)]) != BOOL || CheckType(reg[CheckRegister(dest)]) != BOOL)
			throw TypeMismatch(count);
		else{
			if(reg[CheckRegister(dest)] == "true" && reg[getIndex(src)] == "true")
				reg[CheckRegister(dest)] = "true";
			else 
				reg[CheckRegister(dest)] = "false";
		}
	}
	else{
		if(CheckType(src) != BOOL || CheckType(reg[CheckRegister(dest)]) != BOOL)
			throw TypeMismatch(count);
		else{
			if(reg[CheckRegister(dest)] == "true" && src == "true")
				reg[CheckRegister(dest)] = "true";
			else 
				reg[CheckRegister(dest)] = "false";
		}
	}
}
void CPU::Or(string dest, string src){
	if(CheckType(src) == REGISTER){
		if(CheckType(reg[getIndex(src)]) != BOOL || CheckType(reg[CheckRegister(dest)]) != BOOL)
			throw TypeMismatch(count);
		else{
			if(reg[CheckRegister(dest)] == "false" && reg[getIndex(src)] == "false")
				reg[CheckRegister(dest)] = "false";
			else 
				reg[CheckRegister(dest)] = "true";
		}
	}
	else{
		if(CheckType(src) != BOOL || CheckType(reg[CheckRegister(dest)]) != BOOL)
			throw TypeMismatch(count);
		else{
			if(reg[CheckRegister(dest)] == "false" && src == "false")
				reg[CheckRegister(dest)] = "false";
			else 
				reg[CheckRegister(dest)] = "true";
		}
	}
}
/////////////////////////////////
// Load and store instructions
void CPU::Move(string dest, string src){
	if(CheckType(src) == REGISTER){
		reg[CheckRegister(dest)] = reg[getIndex(src)];
	}
	else 
		reg[CheckRegister(dest)] = src;
}
void CPU::Load(string dest, string src){
	if(CheckType(src) == REGISTER){
		if(CheckType(reg[getIndex(src)]) != ADDRESS)
			throw TypeMismatch(count);
		else{
			reg[CheckRegister(dest)] = StaticMemory[getAddress(reg[getIndex(src)])];
		}
	}
	else{
		if(CheckType(src) != ADDRESS)
			throw TypeMismatch(count);
		else{
			reg[CheckRegister(dest)] = StaticMemory[getAddress(src)];
		}
	}	
}
void CPU::Store(string dest, string src){
	if(CheckType(reg[CheckRegister(dest)]) != ADDRESS)
		throw TypeMismatch(count);
	else{
		CheckType(src) == REGISTER ? 
			StaticMemory[getAddress(reg[CheckRegister(dest)])] = reg[getIndex(src)] : 
			StaticMemory[getAddress(reg[CheckRegister(dest)])] = src;
	}
}
////////////////////////////////
// Sequence control instructions
void CPU::Jump(string src){
	if(CheckType(src) == REGISTER){
		if(CheckType(reg[getIndex(src)]) != ADDRESS)
			throw TypeMismatch(count);
		else
			reg_ID = &InstMemory[getAddress(reg[getIndex(src)])];
	}
	else
		if(CheckType(src) != ADDRESS)
			throw TypeMismatch(count);
		else
			reg_ID = &InstMemory[getAddress(src)];
}
void CPU::JumpIf(string dest, string src){
	if(CheckType(src) == REGISTER){
		if(CheckType(reg[getIndex(src)]) != ADDRESS || CheckType(reg[CheckRegister(dest)]) != BOOL)
			throw TypeMismatch(count);
		else{
			if(reg[CheckRegister(dest)] == "true")
				reg_ID = &InstMemory[getAddress(reg[getIndex(src)])];
			else return;
		}
	}
	else{
		if(CheckType(src) != ADDRESS || CheckType(reg[CheckRegister(dest)]) != BOOL)
			throw TypeMismatch(count);
		else{
			if(reg[CheckRegister(dest)] == "true")
				reg_ID = &InstMemory[getAddress(src)];
			else return;
		}
	}
}
void CPU::Call(string src){

}
void CPU::Return(){

}
void CPU::Halt(){
	*reg_ID = "\0";
}
///////////////////////////////
// Input and Output instructions
void CPU::Input(string dest){
	if(CheckType(dest) != INT && CheckType(dest) != FLOAT && CheckType(dest) != BOOL)
		throw TypeMismatch(count);
	else cin >> reg[CheckRegister(dest)];
}
void CPU::Output(string src){
	if(CheckType(src) == REGISTER){
		cout << reg[CheckRegister(src)];
	}
	else 
		cout << src;
}
///////////////////////////////

void CPU::doit(string filename){
	readfile(filename);
	while (*reg_ID!="\0"){
		AssignData(reg_ID, dest, src);
		switch (CheckInst(reg_ID)){
			case 0: this->Add(dest, src);
				break;
			case 1: this->Minus(dest, src);
				break;
			case 2: this->Mul(dest, src);
				break;
			case 3: this->Div(dest, src);
				break;
			case 4: this->CmpEQ(dest, src);
				break;
			case 5: this->CmpNE(dest, src);
				break;
			case 6: this->CmpLT(dest, src);
				break;
			case 7: this->CmpLE(dest, src);
				break;
			case 8: this->CmpGT(dest, src);
				break;
			case 9: this->CmpGE(dest, src);
				break;
			case 10: this->Not(dest);
				break;
			case 11: this->And(dest, src);
				break;
			case 12: this->Or(dest, src);
				break;
			case 13: this->Move(dest, src);
				break;
			case 14: this->Load(dest, src);
				break;
			case 15: this->Store(dest, src);
				break;
			case 16: this->Jump(dest);
				break;
			case 17: this->JumpIf(dest, src);
				break;
			case 18: this->Call(dest);
				break;
			case 19: this->Return();
				break;
			case 20: this->Halt();
				break;
			case 21: this->Input(dest);
				break;
			case 22: this->Output(dest);
				break;
			default:
				break;
		}
		dest.erase();
		src.erase();
		reg_ID ++;
		count ++;
	}
}
///////////////////////////////

void VM::run(string filename){
	CPU test;
	test.doit(filename);	
}