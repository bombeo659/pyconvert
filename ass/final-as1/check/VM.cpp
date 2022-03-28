#include "VM.h"
void VM::run(string filename)
{
	readfilename(filename);
	excec();
}
void VM:: readfilename(string filename){
	ifstream file(filename);
	string line;
	while((!file.eof())){
		getline(file,line);
		load(line);
	}
	file.close();
}
void VM :: load(string instruction){
	if( checkloaded1(instruction) == false ){
		throw InvalidInstruction(IP);
	}
	else{ 
		Instruction tmp;
		char *cstr = new char[instruction.length() +1];
		strcpy(cstr, instruction.c_str());
		char* tok = strtok(cstr, " ");
		tmp.code = string(tok);
		int i = 0;
		while (tok != NULL) {
			tok = strtok(NULL, " ,");
			if(tok != NULL){
				if(i == 0){
					tmp.opcode1 = string(tok);
				}
				else if(i == 1){
					tmp.opcode2 = string(tok);
				}
				else throw InvalidInstruction(IP);
			}
			i++;
		}
		this->codeMemory[codeloadIndex] = tmp;
		codeloadIndex++;
  	}
}
void VM:: excec(){
	for(int i=0;i<codeloadIndex;i++){
		Instruction ins = codeMemory[i];
		if(ins.code == "Move")this-> move(ins.opcode1, ins.opcode2);
		else if(ins.code == "Add")this-> add(ins.opcode1, ins.opcode2);
		else if(ins.code == "Minus")this-> minus(ins.opcode1, ins.opcode2);
		else if(ins.code == "Mul")this-> mul(ins.opcode1, ins.opcode2);
		else if(ins.code == "Div") this -> div(ins.opcode1, ins.opcode2);
		else if(ins.code == "Halt") break;
		else if(ins.code == "Output")this-> output(ins.opcode1);
		else if(codeMemory[i].code == "CmpEQ")this-> CmpEQ(ins.opcode1, ins.opcode2);
		else if(codeMemory[i].code == "CmpNE") CmpNE(ins.opcode1, ins.opcode2);
		else if(codeMemory[i].code == "CmpLT") CmpLT(ins.opcode1, ins.opcode2);
		else if(codeMemory[i].code == "CmpLE") CmpLE(ins.opcode1, ins.opcode2);
		else if(codeMemory[i].code == "CmpGT") CmpGT(ins.opcode1, ins.opcode2);
		else if(codeMemory[i].code == "CmpGE") CmpGE(ins.opcode1, ins.opcode2);
		else if(ins.code == "Not") this ->Not(ins.opcode1);
		else if(ins.code == "And") this ->And(ins.opcode1, ins.opcode2);
		else if(ins.code == "Or") this ->Or(ins.opcode1, ins.opcode2);
	}
}
bool VM :: checkloaded1(string instruction){
	// int lenght = instruction.length();
	// if(instruction[0] == ' ' || instruction[lenght-1] == ' ')
	// return false ;
	// else{
	// 	for(int i=0; i< lenght ;i++){
	// 		if(instruction[i] == ' ' && instruction[i+1] == ' ' )
	// 		return false;
	// 		else return true;
	// 	}
	// }
	// Instruction pre;
	// char *cstr = new char[instruction.length() +1];
    // strcpy(cstr, instruction.c_str());
    // char* tok = strtok(cstr, " ");
	// pre.code = tok;
	// for(int i=0; i<21;i++){
	// 	if(array[i] == pre.code){
	// 		return true;
	// 		break;
	// 	}
	// 	else return false;
	// }
	// int i=0;
	// while(tok != NULL){
	// 	if(i == 1){
	// 		pre.opcode1 = string(tok);
	// 		if(checkreg(pre.opcode1) == false)
	// 		return false;
	// 	}   
	// 	else if(i == 2){
	// 		pre.opcode2 = string(tok);
	// 		if(checkreg(pre.opcode2) == false)
	// 		return false;
	// 	}
	// 	else return false;
	// 	tok = strtok(NULL, " ,");
	// 	i++;
	// }
	return true;
}
int VM::checktypedata(string str){
	if(str == "true" || str == "false") return BOOL;
	else if(str[str.length()-1] == 'A') return ADDRESS; // address 
	else if(str[0] == 'R') return REGISTER;
	else
		for(int i = 0; i<(int)str.length(); i++)
			if(str[i] == '.') return FLOAT;
	return INT; 
}

bool VM :: stringtobool(string src){
	if(src == "true") return true;
    else return false;
}

bool VM :: checkreg(string src){
	if(stoi(src.substr(1))-1 < 1 || stoi(src.substr(1))-1 > 15 ) return false;
	else return true;
}

void VM :: move(string dest, string src){
	int Idxdest = stoi(dest.substr(1))-1;
	if(checktypedata(dest) != 5)
	throw TypeMismatch(IP);
	if(src[0]== 'R') {
		int Idxsrc = stoi(src.substr(1))-1;
		if(reg[Idxsrc].type == 1){
			reg[Idxdest].data1 = reg[Idxsrc].data1;
        	reg[Idxdest].type = reg[Idxsrc].type;
		}
		else if(reg[Idxsrc].type == 2) {
			reg[Idxdest].data2 = reg[Idxsrc].data2;
        	reg[Idxdest].type = reg[Idxsrc].type;
		}
		else if(reg[Idxsrc].type == 3){
			reg[Idxdest].data3 = reg[Idxsrc].data3;
        	reg[Idxdest].type = reg[Idxsrc].type;
		}
    }
    else {
		if(checktypedata(src) == 1){
			reg[Idxdest].data1 = stoi(src);
        	reg[Idxdest].type = INT ;
		}
		else if(checktypedata(src) == 2) {
			reg[Idxdest].data2 = stof(src);
        	reg[Idxdest].type = FLOAT ;
		}
		else if(checktypedata(src) == 3){
			reg[Idxdest].data3 = stringtobool(src);
			reg[Idxdest].type = BOOL;
		}

    }
        this -> IP ++;
}

void VM :: add(string dest, string src){
	int Idxdest = stoi(dest.substr(1))-1;
	if(checktypedata(dest) != 5 || checktypedata(src) == 3 || checktypedata(src) == 4 || reg[Idxdest].type == 3 
	|| reg[Idxdest].type == 4)
	throw TypeMismatch(IP);
	if(src[0] == 'R'){
		int Idxsrc = stoi(src.substr(1))-1;
		if(reg[Idxsrc].type == 3 || reg[Idxsrc].type ==4 )
		throw TypeMismatch(IP); 
		else if(reg[Idxsrc].type == 1 && reg[Idxdest].type == 1){
			reg[Idxdest].data1 = reg[Idxdest].data1 + reg[Idxsrc].data1;
			reg[Idxdest].type = INT;
		}
		else if(reg[Idxsrc].type == 2){
			if(reg[Idxdest].type == 1){
				reg[Idxdest].data2 =(float) reg[Idxdest].data1 +(float) reg[Idxsrc].data2;
				reg[Idxdest].type = FLOAT;
			}
			else{
				reg[Idxdest].data2 =(float) reg[Idxdest].data2 +(float) reg[Idxsrc].data2;
				reg[Idxdest].type = FLOAT;
			}
		}
		else {
			reg[Idxdest].data2 =(float) reg[Idxdest].data2 +(float) reg[Idxsrc].data1;
			reg[Idxdest].type = FLOAT;
		}
	}
	else {
		if(checktypedata(src) == 1 && reg[Idxdest].type == 1){
			reg[Idxdest].data1 = reg[Idxdest].data1 + stoi(src);
			reg[Idxdest].type = INT;
		}
		else if(checktypedata(src) == 2) {
			if(reg[Idxdest].type == 1){
				reg[Idxdest].data2 =(float) reg[Idxdest].data1 + stof(src);
				reg[Idxdest].type = FLOAT;
			}
			else{
				reg[Idxdest].data2 =(float) reg[Idxdest].data2 + stof(src);
				reg[Idxdest].type = FLOAT;
			}
		}
		else {
			reg[Idxdest].data2 = reg[Idxdest].data2 + stof(src);
			reg[Idxdest].type = FLOAT;
		}
	}

	this -> IP++;
}

void VM :: minus(string dest, string src){
	int Idxdest = stoi(dest.substr(1))-1;
	if(checktypedata(dest) != 5 || checktypedata(src) == 3 || checktypedata(src) == 4 || reg[Idxdest].type == 3 
	|| reg[Idxdest].type == 4)
		throw TypeMismatch(IP);
	if(src[0] == 'R'){
		int Idxsrc = stoi(src.substr(1))-1;
		if(reg[Idxsrc].type == 3 || reg[Idxsrc].type ==4 ) 
		throw TypeMismatch(IP);
		if(reg[Idxsrc].type == 1 && reg[Idxdest].type == 1){
			reg[Idxdest].data1 = reg[Idxdest].data1 - reg[Idxsrc].data1;
			reg[Idxdest].type = INT;
		}
		else if(reg[Idxsrc].type == 2){
			if(reg[Idxdest].type == 1){
				reg[Idxdest].data2 =(float) reg[Idxdest].data1 -(float) reg[Idxsrc].data2;
				reg[Idxdest].type = FLOAT;
			}
			else{
				reg[Idxdest].data2 =(float) reg[Idxdest].data2 -(float) reg[Idxsrc].data2;
				reg[Idxdest].type = FLOAT;
			}
		}
		else {
			reg[Idxdest].data2 =(float) reg[Idxdest].data2 - (float) reg[Idxsrc].data1;
			reg[Idxdest].type = FLOAT;
		}
	}
	else {
		if(checktypedata(src) == 1 && reg[Idxdest].type == 1){
			reg[Idxdest].data1 = reg[Idxdest].data1 - stoi(src);
			reg[Idxdest].type = INT;
		}
		else if(checktypedata(src) == 2) {
			if(reg[Idxdest].type == 1){
				reg[Idxdest].data2 =(float) reg[Idxdest].data1 - stof(src);
				reg[Idxdest].type = FLOAT;
			}
			else{
				reg[Idxdest].data2 =(float) reg[Idxdest].data2 - stof(src);
				reg[Idxdest].type = FLOAT;
			}
		}
		else {
			reg[Idxdest].data2 = reg[Idxdest].data2 - stof(src);
			reg[Idxdest].type = FLOAT;
		}
	}

	this -> IP++;
}

void VM :: mul(string dest, string src){
	int Idxdest = stoi(dest.substr(1))-1;
	if(checktypedata(dest) != 5 || checktypedata(src) == 3 || checktypedata(src) == 4)
		throw TypeMismatch(IP);
	if(src[0] == 'R'){
		int Idxsrc = stoi(src.substr(1))-1;
		if(reg[Idxsrc].type == 3 || reg[Idxsrc].type ==4 ) 
		throw TypeMismatch(IP);
		if(reg[Idxsrc].type == 1 && reg[Idxdest].type == 1){
			reg[Idxdest].data1 = reg[Idxdest].data1 * reg[Idxsrc].data1;
			reg[Idxdest].type = INT;
		}
		else if(reg[Idxsrc].type == 2){
			if(reg[Idxdest].type == 1){
				reg[Idxdest].data2 =(float) reg[Idxdest].data1 *(float) reg[Idxsrc].data2;
				reg[Idxdest].type = FLOAT;
			}
			else{
				reg[Idxdest].data2 =(float) reg[Idxdest].data2 *(float) reg[Idxsrc].data2;
				reg[Idxdest].type = FLOAT;
			}
		}
		else {
			reg[Idxdest].data2 =(float) reg[Idxdest].data2 *(float) reg[Idxsrc].data1;
			reg[Idxdest].type = FLOAT;
		}
	}
	else {
		if(checktypedata(src) == 1 && reg[Idxdest].type == 1){
			reg[Idxdest].data1 = reg[Idxdest].data1 * stoi(src);
			reg[Idxdest].type = INT;
		}
		else if(checktypedata(src) == 2) {
			if(reg[Idxdest].type == 1){
				reg[Idxdest].data2 =(float) reg[Idxdest].data1 * stof(src);
				reg[Idxdest].type = FLOAT;
			}
			else{
				reg[Idxdest].data2 =(float) reg[Idxdest].data2 * stof(src);
				reg[Idxdest].type = FLOAT;
			}
		}
		else {
			reg[Idxdest].data2 = reg[Idxdest].data2 * stof(src);
			reg[Idxdest].type = FLOAT;
		}
	}

	this -> IP++;
}

void VM :: div(string dest, string src){
	int Idxdest = stoi(dest.substr(1))-1;
	if(checktypedata(dest) != 5 || checktypedata(src) == 3 || checktypedata(src) == 4)
		throw TypeMismatch(IP);
	if(src[0] == 'R'){
		int Idxsrc = stoi(src.substr(1))-1;
		if(reg[Idxsrc].type == 3 || reg[Idxsrc].type ==4 ) 
		throw TypeMismatch(IP);
		if(reg[Idxsrc].type == 1 && reg[Idxdest].type == 1){
			reg[Idxdest].data1 = reg[Idxdest].data1 / reg[Idxsrc].data1;
			reg[Idxdest].type = INT;
		}
		else if(reg[Idxsrc].type == 2){
			if(reg[Idxdest].type == 1){
				reg[Idxdest].data2 =(float) reg[Idxdest].data1 / (float) reg[Idxsrc].data2;
				reg[Idxdest].type = FLOAT;
			}
			else{
				reg[Idxdest].data2 =(float) reg[Idxdest].data2 / (float) reg[Idxsrc].data2;
				reg[Idxdest].type = FLOAT;
			}
		}
		else {
			reg[Idxdest].data2 =(float) reg[Idxdest].data2 / (float) reg[Idxsrc].data1;
			reg[Idxdest].type = FLOAT;
		}
	}
	else {
		if(checktypedata(src) == 1 && reg[Idxdest].type == 1){
			reg[Idxdest].data1 = reg[Idxdest].data1 / stoi(src);
			reg[Idxdest].type = INT;
		}
		else if(checktypedata(src) == 2) {
			if(reg[Idxdest].type == 1){
				reg[Idxdest].data2 =(float) reg[Idxdest].data1 / stof(src);
				reg[Idxdest].type = FLOAT;
			}
			else{
				reg[Idxdest].data2 =(float) reg[Idxdest].data2 / stof(src);
				reg[Idxdest].type = FLOAT;
			}
		}
		else {
			reg[Idxdest].data2 = reg[Idxdest].data2 / stof(src);
			reg[Idxdest].type = FLOAT;
		}
	}

	this -> IP++;
}

void VM :: CmpEQ(string dest, string src) {
	int Idxdest = stoi(dest.substr(1))-1;
	if(checktypedata(dest) != 5 || checktypedata(src) == 4 || reg[Idxdest].type == 4)
		throw TypeMismatch(IP);
	if(src[0] == 'R'){
		int Idxsrc = stoi(src.substr(1))-1;
		if(reg[Idxsrc].type == 4 || (reg[Idxdest].type == 3 && reg[Idxsrc].type != 3)
		|| (reg[Idxdest].type != 3 && reg[Idxsrc].type == 3)) 
		throw TypeMismatch(IP);
		if(reg[Idxdest].type == 1){
			if(reg[Idxsrc].type == 1){
				reg[Idxdest].type = BOOL;
				if(reg[Idxdest].data1 == reg[Idxsrc].data1 )
				reg[Idxdest].data3 = true;
				else
				reg[Idxdest].data3 = false;
			}
			else {
				reg[Idxdest].type = BOOL;
				if(reg[Idxdest].data1 == reg[Idxsrc].data2 )
				reg[Idxdest].data3 = true;
				else
				reg[Idxdest].data3 = false;
			}
		}
		else if(reg[Idxdest].type == 2){
			if(reg[Idxsrc].type == 1){
				reg[Idxdest].type = BOOL;
				if(reg[Idxdest].data2 == reg[Idxsrc].data1 )
				reg[Idxdest].data3 = true;
				else
				reg[Idxdest].data3 = false;
			}
			else if(reg[Idxsrc].type == 2){
				reg[Idxdest].type = BOOL;
				if(reg[Idxdest].data2 == reg[Idxsrc].data2 )
				reg[Idxdest].data3 = true;
				else
				reg[Idxdest].data3 = false;
			}	
		}
		else if(reg[Idxdest].type == 3 && reg[Idxsrc].type == 3){
			reg[Idxdest].type = BOOL;
			if(reg[Idxdest].data3 == reg[Idxsrc].data3 )
			reg[Idxdest].data3 = true;
			else 
			reg[Idxdest].data3 = false;
		}
	}
	else {
		if((reg[Idxdest].type == 3 && checktypedata(src) != 3) || (reg[Idxdest].type != 3 && checktypedata(src) == 3) )
		throw TypeMismatch(IP);
		if(reg[Idxdest].type == 1){
			if(checktypedata(src) == 1){
				reg[Idxdest].type = BOOL;
				if(reg[Idxdest].data1 == stoi(src))
				reg[Idxdest].data3 = true;
				else 
				reg[Idxdest].data3 = false;
			}
			else if(checktypedata(src) == 2){
				reg[Idxdest].type = BOOL;
				if(reg[Idxdest].data1 == stof(src))
				reg[Idxdest].data3 = true;
				else 
				reg[Idxdest].data3 = false;
			}
		}
		else if(reg[Idxdest].type == 2){
			if(checktypedata(src) == 1){
				reg[Idxdest].type = BOOL;
				if(reg[Idxdest].data2 == stoi(src))
				reg[Idxdest].data3 = true;
				else 
				reg[Idxdest].data3 = false;
			}
			else if(checktypedata(src) == 2){
				reg[Idxdest].type = BOOL;
				if(reg[Idxdest].data2 == stof(src))
				reg[Idxdest].data3 = true;
				else 
				reg[Idxdest].data3 = false;
			}
		}
		else if(reg[Idxdest].type == 3 && checktypedata(src) == 3){
			reg[Idxdest].type = BOOL;
			if(reg[Idxdest].data3 == stringtobool(src))
			reg[Idxdest].data3 = true;
			else 
			reg[Idxdest].data3 = false;
		}
	}
}

void VM :: CmpNE(string dest, string src) {

}

void VM :: CmpLT(string dest, string src) {

}

void VM :: CmpLE(string dest, string src) {

}

void VM :: CmpGT(string dest, string src) {

}

void VM :: CmpGE(string dest, string src) {

}

void VM :: Not(string dest){
	int Idxdest = stoi(dest.substr(1))-1;
	if(checktypedata(dest) != 5 || reg[Idxdest].type !=3 )
	throw TypeMismatch(IP);
	reg[Idxdest].data3 = !(reg[Idxdest].data3);
}

void VM :: And(string dest, string src){

}

void VM :: Or(string dest, string src){

}

void VM :: output(string dest) {
    int Idxdest =stoi(dest.substr(1))-1;
	if(reg[Idxdest].type == 1) cout << reg[Idxdest].data1;
	else if(reg[Idxdest].type == 2) cout << reg[Idxdest].data2;
	else if(reg[Idxdest].type == 3) cout << reg[Idxdest].data3;
    this -> IP++;
}

