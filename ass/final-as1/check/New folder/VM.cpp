#include "VM.h"
//tao stack

int power(int a, int b) {
	int result = 1;
	for (int i = 1; i <= b; i++) {
		result *= a;
	}
	return result;
}
float stringToFloat(string s) {
	int l = s.length();
	int i;    
	for (i = 0; i < l; i++)
		if (s[i] == '.') break;
	int soSauPhay= l - i - 1;    
	int soTruocPhay = l - 1 - soSauPhay;
	float numtruocphay = 0;
	float numsauphay = 0;
	for (int i = l - 1; i >= l-soSauPhay; i--) {
		numsauphay += (float)(s[i] - '0') * power(10, l - i - 1);
	}
	for (int i = 0; i <= soTruocPhay - 1;i++) {
		numtruocphay += (float)(s[i] - '0') * power(10, soTruocPhay-i-1);
	}
	return numtruocphay + numsauphay / power(10, soSauPhay);
}

int stringToInt(string s) {
	int l = s.length();
	int num = 0;
	for (int i = l - 1; i >= 0; i--) {
		num += (int)(s[i] - '0') * power(10, l - i - 1);
	}
	return num;
}
int getIndex(string s){
	if(s.length()==2) {
		return stringToInt(s.substr(1,1));
	}
	else // (s.length()==3){
		return stringToInt(s.substr(1,2));
	//}
}
int getAddress(string s){
	return stringToInt(s.substr(0,s.length()-1));
}
string addressOfIP(int ip){
	string address=to_string(ip);
	address+="A";
	return address;
}
int type(string s){
	if(s[0]=='R') return 0;
	if(s[s.length()-1]=='A') return 1;
	if(s=="true"||s=="false") return 2;
	for (size_t i=0; i<s.length(); i++)
		{ if (s[i]=='.') return 3; }
		return 4;
}
void Stack::push(string s){
	if(isFull()){
		//exit(EXIT_FAILURE);
		return;//sau nay quang loi StackFull
	}
	arr[++top]=s;
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
	return top==-1;
}
bool Stack::isFull(){
	return top==capacity-1;
}
void CPU::Add(string s1,string s2){
	IP++;
	if (type(s2) == 0){	
		if((type(R[getIndex(s2)]) !=3 && type(R[getIndex(s2)])!=4) || (type(R[getIndex(s1)]) !=3 && type(R[getIndex(s1)])!=4))
		throw TypeMismatch(this->IP-1);
		if (type(R[getIndex(s2)]) == 3 || type(R[getIndex(s1)]) == 3)
			R[getIndex(s1)] = to_string(stringToFloat(R[getIndex(s2)]) + stringToFloat(R[getIndex(s1)]));
		R[getIndex(s1)] = to_string(stringToInt(R[getIndex(s2)]) + stringToInt(R[getIndex(s1)]));
	}
	else{
		if((type(s2) !=3 && type(s2)!=4) || (type(R[getIndex(s1)]) !=3 && type(R[getIndex(s1)])!=4))
		throw TypeMismatch(this->IP-1);
		if (type(s2) == 3 || type(R[getIndex(s1)]) == 3){
		R[getIndex(s1)] = to_string(stringToFloat(s2) + stringToFloat(R[getIndex(s1)]));
		}
		if (type(s2) == 4 && type(R[getIndex(s1)]) == 4){
		R[getIndex(s1)] = to_string(stringToInt(s2) + stringToInt(R[getIndex(s1)]));
		}
	}
}
void CPU::Minus(string s1,string s2){
	IP++;
	if (type(s2) == 0){
		if((type(R[getIndex(s2)]) !=3 && type(R[getIndex(s2)])!=4) || (type(R[getIndex(s1)]) !=3 && type(R[getIndex(s1)])!=4))
		throw TypeMismatch(this->IP-1);
		if (type(R[getIndex(s2)]) == 3 || type(R[getIndex(s1)]) == 3)
			R[getIndex(s1)] = to_string(stringToFloat(R[getIndex(s1)]) - stringToFloat(R[getIndex(s2)]));
		R[getIndex(s1)] = to_string(stringToInt(R[getIndex(s1)]) - stringToInt(R[getIndex(s2)]));
	}
	else{
		if((type(s2) !=3 && type(s2)!=4) || (type(R[getIndex(s1)]) !=3 && type(R[getIndex(s1)])!=4))
		throw TypeMismatch(this->IP-1);
	if (type(s2) == 3 || type(R[getIndex(s1)]) == 3){
		R[getIndex(s1)] = to_string(stringToFloat(R[getIndex(s1)]) - stringToFloat(s2));
	}
	if (type(s2) == 4 && type(R[getIndex(s1)]) == 4){
		R[getIndex(s1)] = to_string(stringToInt(R[getIndex(s1)]) - stringToInt(s2));
	}
	}
}
void CPU::Mul(string s1,string s2){
	IP++;
	if (type(s2) == 0){
		if((type(R[getIndex(s2)]) !=3 && type(R[getIndex(s2)])!=4) || (type(R[getIndex(s1)]) !=3 && type(R[getIndex(s1)])!=4))
		throw TypeMismatch(this->IP-1);
		if (type(R[getIndex(s2)]) == 3 || type(R[getIndex(s1)]) == 3)
			R[getIndex(s1)] = to_string(stringToFloat(R[getIndex(s1)])*stringToFloat(R[getIndex(s2)]));
		R[getIndex(s1)] = to_string(stringToInt(R[getIndex(s1)])*stringToInt(R[getIndex(s2)]));
	}
	else{
		if((type(s2) !=3 && type(s2)!=4) || (type(R[getIndex(s1)]) !=3 && type(R[getIndex(s1)])!=4))
		throw TypeMismatch(this->IP-1);
	if (type(s2) == 3 || type(R[getIndex(s1)]) == 3){
		R[getIndex(s1)] = to_string(stringToFloat(R[getIndex(s1)])*stringToFloat(s2));
	}
	if (type(s2) == 4 && type(R[getIndex(s1)]) == 4){
		R[getIndex(s1)] = to_string(stringToInt(R[getIndex(s1)])*stringToInt(s2));
	}
	}
}
void CPU::Div(string s1,string s2){
	IP++;
	if (type(s2) == 0){	
		if(stringToInt(R[getIndex(s2)])==0 || stringToFloat(R[getIndex(s2)])==0.0) throw DivideByZero(this->IP-1);
		if((type(R[getIndex(s2)]) !=3 && type(R[getIndex(s2)])!=4) || (type(R[getIndex(s1)]) !=3 && type(R[getIndex(s1)])!=4))
		throw TypeMismatch(this->IP-1);
		if (type(R[getIndex(s2)]) == 3 || type(R[getIndex(s1)]) == 3)
			R[getIndex(s1)] = to_string(stringToFloat(R[getIndex(s1)])/stringToFloat(R[getIndex(s2)]));
		R[getIndex(s1)] = to_string(stringToInt(R[getIndex(s1)])/stringToInt(R[getIndex(s2)]));
	}
	else{
		if((type(s2) !=3 && type(s2)!=4) || (type(R[getIndex(s1)]) !=3 && type(R[getIndex(s1)])!=4))
		throw TypeMismatch(this->IP-1);
	if (type(s2) == 3 || type(R[getIndex(s1)]) == 3){
		if(stringToInt(s2)==0 || stringToFloat(s2)==0.0) throw DivideByZero(this->IP-1);
		R[getIndex(s1)] = to_string(stringToFloat(R[getIndex(s1)])/stringToFloat(s2));
	}
	if (type(s2) == 4 && type(R[getIndex(s1)]) == 4){	
		if(stringToInt(s2)==0 || stringToFloat(s2)==0.0) throw DivideByZero(this->IP-1);
		R[getIndex(s1)] = to_string(stringToInt(R[getIndex(s1)])/stringToInt(s2));
	}
	}
}
void CPU::CmpEQ(string s1, string s2){
	IP++;
	if(type(s2)==0){
		if((type(R[getIndex(s1)])==2 && type(R[getIndex(s2)])!=2) ||
		    (type(R[getIndex(s1)])!=2 && type(R[getIndex(s2)])==2) ||
			(type(R[getIndex(s1)])==1 || type(R[getIndex(s2)])==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(R[getIndex(s1)]==R[getIndex(s2)]) R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";
	}
	else{
		if((type(R[getIndex(s1)])==2 && type(s2)!=2) ||
		    (type(R[getIndex(s1)])!=2 && type(s2)==2) ||
			(type(R[getIndex(s1)])==1 || type(s2)==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(R[getIndex(s1)]==s2) R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";
	}
}
void CPU::CmpNE(string s1, string s2){
	IP++;
	if(type(s2)==0){
		if((type(R[getIndex(s1)])==2 && type(R[getIndex(s2)])!=2) ||
		    (type(R[getIndex(s1)])!=2 && type(R[getIndex(s2)])==2) ||
			(type(R[getIndex(s1)])==1 || type(R[getIndex(s2)])==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(R[getIndex(s1)]!=R[getIndex(s2)]) R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";
	}
	else{
		if((type(R[getIndex(s1)])==2 && type(s2)!=2) ||
		    (type(R[getIndex(s1)])!=2 && type(s2)==2) ||
			(type(R[getIndex(s1)])==1 || type(s2)==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(R[getIndex(s1)]!=s2) R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";
	}
	
}
void CPU::CmpLT(string s1, string s2){
	IP++;
	if(type(s2)==0){
		if((type(R[getIndex(s1)])==2 || type(R[getIndex(s2)])==2) ||
			(type(R[getIndex(s1)])==1 || type(R[getIndex(s2)])==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(stringToFloat(R[getIndex(s1)])<stringToFloat(R[getIndex(s2)]))
		R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";
	}
	else{
		if((type(R[getIndex(s1)])==2 || type(s2)==2) ||
		   (type(R[getIndex(s1)])==1 || type(s2)==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(stringToFloat(R[getIndex(s1)])<stringToFloat(s2))
		R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";	
	}
}
void CPU::CmpLE(string s1, string s2){
	IP++;
	if(type(s2)==0){
		if((type(R[getIndex(s1)])==2 || type(R[getIndex(s2)])==2) ||
			(type(R[getIndex(s1)])==1 || type(R[getIndex(s2)])==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(stringToFloat(R[getIndex(s1)])<=stringToFloat(R[getIndex(s2)]))
		R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";
	}
	else{
		if((type(R[getIndex(s1)])==2 || type(s2)==2) ||
		   (type(R[getIndex(s1)])==1 || type(s2)==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(stringToFloat(R[getIndex(s1)])<=stringToFloat(s2))
		R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";	
	}
}
void CPU::CmpGT(string s1, string s2){
	IP++;
	if(type(s2)==0){
		if((type(R[getIndex(s1)])==2 || type(R[getIndex(s2)])==2) ||
			(type(R[getIndex(s1)])==1 || type(R[getIndex(s2)])==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(stringToFloat(R[getIndex(s1)])>stringToFloat(R[getIndex(s2)]))
		R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";
	}
	else{
		if((type(R[getIndex(s1)])==2 || type(s2)==2) ||
		   (type(R[getIndex(s1)])==1 || type(s2)==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(stringToFloat(R[getIndex(s1)])>stringToFloat(s2))
		R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";	
	}
}
void CPU::CmpGE(string s1, string s2){
	IP++;
	if(type(s2)==0){
		if((type(R[getIndex(s1)])==2 || type(R[getIndex(s2)])==2) ||
			(type(R[getIndex(s1)])==1 || type(R[getIndex(s2)])==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(stringToFloat(R[getIndex(s1)])>=stringToFloat(R[getIndex(s2)]))
		R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";
	}
	else{
		if((type(R[getIndex(s1)])==2 || type(s2)==2) ||
		   (type(R[getIndex(s1)])==1 || type(s2)==1)){
				throw TypeMismatch(this->IP-1);
			}
		if(stringToFloat(R[getIndex(s1)])>=stringToFloat(s2))
		R[getIndex(s1)]="true";
		else R[getIndex(s1)]="false";	
	}
}
void CPU::Not(string s1){
	IP++;
	if(type(R[getIndex(s1)])!=2) throw TypeMismatch(this->IP-1);
	if(R[getIndex(s1)]=="true") R[getIndex(s1)]="false";
	else R[getIndex(s1)]="true";
}
void CPU::And(string s1,string s2){
	IP++;
	if(type(s2)==0){
		if(type(R[getIndex(s1)])!=2 || type(R[getIndex(s2)])!=2) throw TypeMismatch(this->IP-1);
		if(R[getIndex(s1)]=="false" || R[getIndex(s2)]=="false") R[getIndex(s1)]="false";
		if(R[getIndex(s1)]=="true" && R[getIndex(s2)]=="true") R[getIndex(s1)]="true";
	}
	else{
		if(type(R[getIndex(s1)])!=2 || type(s2)!=2) throw TypeMismatch(this->IP-1);
		if(R[getIndex(s1)]=="false" || s2=="false") R[getIndex(s1)]="false";
		if(R[getIndex(s1)]=="true" && s2=="true") R[getIndex(s1)]="true";
	}
}
void CPU::Or(string s1, string s2){
	IP++;
	if(type(s2)==0){
		if(type(R[getIndex(s1)])!=2 || type(R[getIndex(s2)])!=2) throw TypeMismatch(this->IP-1);
		if(R[getIndex(s1)]=="true" || R[getIndex(s2)]=="true") R[getIndex(s1)]="true";
		if(R[getIndex(s1)]=="false" && R[getIndex(s2)]=="false") R[getIndex(s1)]="false";
	}
	else{
		if(type(R[getIndex(s1)])!=2 || type(s2)!=2) throw TypeMismatch(this->IP-1);
		if(R[getIndex(s1)]=="true" || s2=="true") R[getIndex(s1)]="true";
		if(R[getIndex(s1)]=="false" && s2=="false") R[getIndex(s1)]="false";
	}
}
void CPU::Move(string s1,string s2){
	IP++;
	if (type(s2) == 0){
		R[getIndex(s1)] = R[getIndex(s2)];
	}
	else R[getIndex(s1)] = s2;
}
void CPU::Load(string s1,string s2){
	IP++;
	if(type(s2)==0){
		if(type(R[getIndex(s2)])!=1) throw TypeMismatch(this->IP-1);
		R[getIndex(s1)]=sMemory[getAddress(R[getIndex(s2)])];
	}
	else{
		if(type(s2)!=1) throw TypeMismatch(this->IP-1);
		R[getIndex(s1)]=sMemory[getAddress(s2)];
	}
}
void CPU::Store(string s1,string s2){
	IP++;
	if(type(R[getIndex(s1)])!=1) throw TypeMismatch(this->IP-1);
	if(type(s2)==0){
		sMemory[getAddress(R[getIndex(s1)])]=R[getIndex(s2)];
	}
	else{
		sMemory[getAddress(R[getIndex(s1)])]=s2;
	}
}
void CPU::Jump(string s2){
	IP++;
	if(type(s2)==0){
		if(type(R[getIndex(s2)])!=1) throw TypeMismatch(this->IP-1);
		IP=getAddress(R[getIndex(s2)]);
	}
	else{
		if(type(s2)!=1) throw TypeMismatch(this->IP-1);
		IP=getAddress(s2);
	}
}
void CPU::JumpIf(string s1,string s2){
	IP++;
	if(type(s2)==0){
		if(type(R[getIndex(s1)])!=2 || type(R[getIndex(s2)])!=1) throw TypeMismatch(this->IP-1);
	}
	else {
		if(type(R[getIndex(s1)])!=2 || type(s2)!=1) throw TypeMismatch(this->IP-1);
	}
	if(R[getIndex(s1)]=="true"){
		if(type(s2)==0) IP=getAddress(R[getIndex(s2)]);
		else  IP=getAddress(s2);
	}
}
void CPU::Call(string s2){
	IP++;
	if(type(s2)==0){
		if(type(R[getIndex(s2)])!=1) throw TypeMismatch(this->IP-1);
		stack.push(to_string(IP)+"A");
		IP=getAddress(R[getIndex(s2)]);
	}
	else{
		if(type(s2)!=1) throw TypeMismatch(this->IP-1);
		stack.push(to_string(IP)+"A");
		IP=getAddress(s2);
	}
}
void CPU::Return(){
	IP++;
	IP=getAddress(stack.pop());
}
void CPU::Input(string s){
	IP++;
	cin>>R[getIndex(s)];
	if(type(R[getIndex(s)])!=2 && type(R[getIndex(s)])!=3 && type(R[getIndex(s)])!=4 )
	throw TypeMismatch(this->IP-1);
}
void CPU::Output(string s){
	IP++;
	if (type(s) == 0){
		cout << R[getIndex(s)];
	}
	else {
		cout << s;
	}
}

void VM::run(string filename)
{ 
	CPU a;
	string s = "";
	int count = 0;
	unsigned int j;
	string *codeMemory=new string[65536];
	string *s1=new string[65536];
	string *s2=new string[65536];
	ifstream f1;
	f1.open(filename, ios::in);
	while (!f1.eof()){
		getline(f1, s); //s=Move R1, 0A;
		for (size_t i = 0; i < s.length(); i++){
			if (s[i] != ' ') codeMemory[count] += s[i];
			else {
				j = i + 1;
				break;
			}
		}
		if (j < s.length()){
			if(codeMemory[count]=="Return" || codeMemory[count]=="Halt") throw InvalidInstruction(count);
			for (size_t i = j; i < s.length(); i++){
				if (s[i] != ',')	s1[count] += s[i];
				if (i == s.length() - 1 || s[i] == ','){
					j = i + 2;
					break;
				}
			}
			if (j < s.length()){	
				if(codeMemory[count]=="Not" || codeMemory[count]=="Jump" || codeMemory[count]=="Call" || 
					codeMemory[count]=="Input" || codeMemory[count]=="Output") throw InvalidInstruction(count);
				for (size_t i = j; i < s.length(); i++){
					if(s[i]==',' || s[i]==' '){
						throw InvalidInstruction(count);
					}
					s2[count] += s[i];
				}
			}
		}
		count++;
	}
	for (int i = 0; i < count; i++){
		if (codeMemory[i] == "Add") a.Add(s1[i], s2[i]);
		else if (codeMemory[i] == "Minus") a.Minus(s1[i], s2[i]);
		else if (codeMemory[i] == "Mul") a.Mul(s1[i], s2[i]);
		else if (codeMemory[i] == "Div") a.Div(s1[i], s2[i]);
		else if (codeMemory[i] == "CmpEQ") a.CmpEQ(s1[i], s2[i]);
		else if (codeMemory[i] == "CmpNE") a.CmpNE(s1[i], s2[i]);
		else if (codeMemory[i] == "CmpLT") a.CmpLT(s1[i], s2[i]);
		else if (codeMemory[i] == "CmpLE") a.CmpLE(s1[i], s2[i]);
		else if (codeMemory[i] == "CmpGT") a.CmpGT(s1[i], s2[i]);
		else if (codeMemory[i] == "CmpGE") a.CmpGE(s1[i], s2[i]);
		else if (codeMemory[i] == "Not") a.Not(s1[i]);
		else if (codeMemory[i] == "And") a.And(s1[i], s2[i]);
		else if (codeMemory[i] == "Or") a.Or(s1[i], s2[i]);
		else if (codeMemory[i] == "Move") a.Move(s1[i], s2[i]);
		else if (codeMemory[i] == "Load") a.Load(s1[i], s2[i]);
		else if (codeMemory[i] == "Store") a.Store(s1[i], s2[i]);
		else if (codeMemory[i] == "Jump"){
			a.Jump(s1[i]);
			if(a.ip()>=count) throw InvalidDestination(i);
			i=a.ip()-1;
		}
		else if (codeMemory[i] == "JumpIf"){
			a.JumpIf(s1[i], s2[i]);
			if(a.ip()>=count) throw InvalidDestination(i);
			i=a.ip()-1;
		}
		else if (codeMemory[i] == "Call"){
			a.Call(s1[i]);
			if(a.ip()>=count) throw InvalidDestination(i);
			i=a.ip()-1;
		}
		else if (codeMemory[i] == "Return"){
			a.Return();
			i=a.ip()-1;
		}
		else if (codeMemory[i] == "Input") a.Input(s1[i]);
		else if (codeMemory[i] == "Output") a.Output(s1[i]);
		else if (codeMemory[i] == "Halt") i=count;
		else throw InvalidInstruction(i);
	}
}
