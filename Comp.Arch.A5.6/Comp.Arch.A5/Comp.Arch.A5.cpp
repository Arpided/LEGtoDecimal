#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct instruct {
	string section;
	string instructIn;
	string destination;
	string source1;
	string source2;
	int pc;						//aimed to help with branch instructions...pending full use
};

struct ruction {				//global struct to assist printing
	string instruction;
	int destination;
	int source1;
	int source2;
};

struct forMem {
	int opcode;
	int destination;
	int source1;
	int source2;
};

vector<instruct> instructions;
vector<forMem> IMEM;
const int val = 10;
const int val2 = 32;
int DMEMStart[val];
int DMEM[val];
int registerStart[val2];
string inputFile = "Assignment5Code3";

ofstream write("WriteTo.txt");

int OP_INT[23] = { 1112, 1624, 1104, 1360, 1872, 1691, 1690, 1368, 1880, 580, 836, 584, 840, 420, 708, 964, 1986, 1984, 180, 181, 84, 5, 37 };

//ofstream write("WriteTo.txt");

void fillInput(int linenum) {
	bool key;				//variable to recognize section tile, e.g. "Loop:"
	string temp, temp2;
	ifstream code; // reader variable 'code'
	code.open("Assignment5Code3.txt"); // 'code' reads from code1.txt 
	instruct fileIn;
	while (code) {				//loop changed to handle read in variance
		code >> temp >> temp2;
		key = temp2 < "X0";
		if ((temp == "B") || (temp.substr(0, 2) == "B.")) {
			fileIn.section = "";
			fileIn.instructIn = temp;
			fileIn.destination = temp2;
			fileIn.source1 = "";
			fileIn.source2 = "";
			fileIn.pc = linenum;
			instructions.push_back(fileIn);
			linenum++;
			//cout << "if \n";
			continue;
		}
		if (temp.substr(0, 2) == "CB") {
			fileIn.section = "";
			fileIn.instructIn = temp;
			fileIn.source1 = temp2;
			code  >> fileIn.source1;
			fileIn.source2 = "";
			fileIn.pc = linenum;
			instructions.push_back(fileIn);
			linenum++;
			//cout << "if \n";
			continue;
		}
		else {
			if (key) {
				temp.erase(remove(temp.begin(), temp.end(), ':'), temp.end());
				fileIn.section = temp;
				fileIn.instructIn = temp2;
				code >> fileIn.destination;
				code >> fileIn.source1 >> fileIn.source2;
				//cout << "else if \n";
			}
			else {
				fileIn.section = "";
				fileIn.instructIn = temp;
				fileIn.destination = temp2;
				code >> fileIn.source1 >> fileIn.source2;
				//cout << "else \n";
			}


			fileIn.pc = linenum;

			instructions.push_back(fileIn);

			linenum++;
		}
	}
	cout << "File read \n";
}

int calcJump(string text, int linenum) {
	signed int jump = 0;
	for (vector<instruct>::iterator i = instructions.begin(); i != instructions.end(); i++) {
		if (text == i->section) {
			jump = i->pc - linenum;
		}
	}
	return jump;
}

void printAll() {
	for (vector<instruct>::iterator i = instructions.begin(); i != instructions.end(); i++) {
		cout << i->instructIn << " " << i->destination << " " << i->source1 << " " << i->source2 << endl;
	}
}

void rFormat(string opcode, string initialDest, string initialS1, string initialS2, int opToDec) { //op | Rm  | shamt | Rn | Rd
	cout << endl << "rformat \n";
	ruction final;
	forMem memory;
	initialDest.erase(remove(initialDest.begin(), initialDest.end(), 'X'), initialDest.end());
	initialDest.erase(remove(initialDest.begin(), initialDest.end(), ','), initialDest.end());
	initialS1.erase(remove(initialS1.begin(), initialS1.end(), 'X'), initialS1.end());
	initialS1.erase(remove(initialS1.begin(), initialS1.end(), ','), initialS1.end());
	initialS2.erase(remove(initialS2.begin(), initialS2.end(), 'X'), initialS2.end());
	if (initialS1 == "ZR") initialS1 = "0";
	if (initialS2 == "ZR") initialS2 = "0";
	int shamt = 0;
	final.destination = stoi(initialDest);
	final.source1 = stoi(initialS1);
	final.source2 = stoi(initialS2);

	memory.opcode = opToDec;
	memory.destination = final.destination;
	memory.source1 = final.source1;
	memory.source2 = final.source2;
	IMEM.push_back(memory);
	//write << opToDec << " " << final.destination << " " << shamt << " " << final.source1 << " " << final.source2 << endl;
}



void rFormatShift(string opcode, string initialDest, string initialS1, string initialS2, int opToDec) { //op | Rm | shamt | Rn | Rd
	cout << endl << "rformatshift \n";
	ruction final;
	forMem memory;
	initialDest.erase(remove(initialDest.begin(), initialDest.end(), 'X'), initialDest.end());
	initialDest.erase(remove(initialDest.begin(), initialDest.end(), ','), initialDest.end());
	initialS1.erase(remove(initialS1.begin(), initialS1.end(), 'X'), initialS1.end());
	initialS1.erase(remove(initialS1.begin(), initialS1.end(), ','), initialS1.end());
	initialS2.erase(remove(initialS2.begin(), initialS2.end(), '#'), initialS2.end());
	if (initialS1 == "ZR") initialS1 = "0";
	if (initialS2 == "ZR") initialS2 = "0";
	if (opcode == "LSL") {
		int RdHold = 0;
		final.destination = stoi(initialDest);
		final.source1 = stoi(initialS1);
		final.source2 = stoi(initialS2);

		memory.opcode = opToDec;
		memory.destination = final.destination;
		memory.source1 = final.source1;
		memory.source2 = final.source2;
		IMEM.push_back(memory);
		//write << opToDec << " " << final.destination << " " << final.source2 << " " << final.source1 << " " << RdHold << endl;
	}
	else if (opcode == "LSR") {
		int RdHold = 0;
		final.destination = stoi(initialDest);
		final.source1 = stoi(initialS1);
		final.source2 = stoi(initialS2);

		memory.opcode = opToDec;
		memory.destination = final.destination;
		memory.source1 = final.source1;
		memory.source2 = final.source2;
		IMEM.push_back(memory);
		//write << opToDec << " " << final.destination << " " << final.source2 << " " << final.source1 << " " << RdHold << endl;
	}
}

void bFormat(string opcode, string initialDest, string initialS1, string initialS2, int opToDec, int NO1, int NO2) { //op | addr
	forMem memory;
	cout << endl << "bformat \n";
	if (opcode.substr(0, 2) == "B.") {
		int jump = NO2;

		memory.opcode = opToDec;
		memory.source1 = jump;
		IMEM.push_back(memory);
		//write << opToDec << " " << jump << endl;
	}
	else if (opcode == "B") {
		int jump = NO2;

		memory.opcode = opToDec;
		memory.source1 = jump;
		IMEM.push_back(memory);
		//write << opToDec << " " << jump << endl;
	}
	else if (opcode.substr(0,2) == "CB") {
		int jump = NO2;

		memory.opcode = opToDec;
		memory.source1 = jump;
		IMEM.push_back(memory);

	}
}

void cbFormat(string opCode, string initialDest, string initialS1, string initialS2, int opToDec, int NO2) {//op | addr | Rt
	cout << endl << "cbformat \n";
	forMem memory;
	ruction final;
	cout << "initialDest = " << initialDest << "\nInitialS1 = " << initialS1 << "\nInitialS2 = " << initialS2 << endl;
	initialDest.erase(remove(initialDest.begin(), initialDest.end(), 'X'), initialDest.end());
	initialDest.erase(remove(initialDest.begin(), initialDest.end(), ','), initialDest.end());
	initialS1.erase(remove(initialS1.begin(), initialS1.end(), 'X'), initialS1.end());
	initialS1.erase(remove(initialS1.begin(), initialS1.end(), ','), initialS1.end());
	initialS2.erase(remove(initialS2.begin(), initialS2.end(), '#'), initialS2.end());
	if (initialS1 == "ZR") initialS1 = "0";
	if (initialS2 == "ZR") initialS2 = "0";
	cout << "finalDest = " << initialDest << endl;
	final.destination = stoi(initialDest);
	final.source1 = NO2;


	memory.opcode = opToDec;
	memory.destination = final.destination;
	memory.source1 = final.source1;
	IMEM.push_back(memory);
}

void iFormat(string opcode, string initialDest, string initialS1, string initialS2, int opToDec) { //op | Imm | Rn | Rd
	cout << endl << "iformat \n";
	ruction final;
	forMem memory;
	initialDest.erase(remove(initialDest.begin(), initialDest.end(), 'X'), initialDest.end());
	initialDest.erase(remove(initialDest.begin(), initialDest.end(), ','), initialDest.end());
	initialS1.erase(remove(initialS1.begin(), initialS1.end(), '['), initialS1.end());
	initialS1.erase(remove(initialS1.begin(), initialS1.end(), 'X'), initialS1.end());
	initialS1.erase(remove(initialS1.begin(), initialS1.end(), ','), initialS1.end());
	initialS2.erase(remove(initialS2.begin(), initialS2.end(), '#'), initialS2.end());
	initialS2.erase(remove(initialS2.begin(), initialS2.end(), ']'), initialS2.end());
	if (initialDest == "ZR") initialDest = "0";
	if (initialS1 == "ZR") initialS1 = "0";
	if (initialS2 == "ZR") initialS2 = "0";
	cout << initialS1 << "	" << initialS2 << endl;
	if (initialDest == "0") {
		int zeroReg = 0;
		final.destination = stoi(initialDest);
		final.source1 = stoi(initialS1);
		final.source2 = stoi(initialS2);

		memory.opcode = opToDec;
		memory.destination = final.destination;
		memory.source1 = final.source1;
		memory.source2 = final.source2;
		IMEM.push_back(memory);
		//write << opToDec << " " << final.source1 << " " << final.source2 << " " << zeroReg << endl;
	}
	else {
		final.destination = stoi(initialDest);
		final.source1 = stoi(initialS1);
		final.source2 = stoi(initialS2);

		memory.opcode = opToDec;
		memory.destination = final.destination;
		memory.source1 = final.source1;
		memory.source2 = final.source2;
		IMEM.push_back(memory);
		//write << opToDec << " " << final.source2 << " " << final.source1 << " " << final.destination << endl;
	}
}


void assemble()
{
	//ifstream code; // reader variable 'code'
	//code.open("code1.txt"); // 'code' reads from code1.txt 
	//I put the txt file in the program folder and I'm hoping the code will see it
	//If not will have to put in full path

	//vector<string> Instructions;

	if (inputFile == "Assignment5Code2") {
		DMEMStart[0] = 10;
		DMEMStart[1] = 13;
		DMEM[0] = 10;
		DMEM[1] = 13;
	}

	//this fills the entire struct in a doubly linked list, using a list to make it easier to navigate if needed
	fillInput(0);
	//	0		1	 2		3		4		5	6		7		8		9		10		11		12		13		14		15		16		17		18		19		20			21	22
	string op_str[23] = { "ADD", "SUB", "AND", "ORR", "EOR", "LSL", "LSR", "ADDS", "SUBS", "ADDI", "SUBI", "ANDI", "ORRI", "EORI", "ADDIS", "SUBIS", "LDUR", "STUR", "CBZ", "CBNZ", "B.cond", "B", "BL" };

	//	1	2		3		4	5		6	7		8	9	 10	  11	12	13	  14	15	16	17		18	19	 20	  21` 22  23
	//printAll();

	string opcode;
	int opcodeToDecimal;
	/*int rd;
	int shamt = 0;
	int rm = 0;
	int rn;
	int format;
	*/
	int lineNO, lineNO2;

	string destHold, s1Hold, s2Hold;

	cout << endl;


	for (vector<instruct>::iterator i = instructions.begin(); i != instructions.end(); i++) {
		int j;
		for (j = 0; j < 23; j++) {
			cout << i->instructIn << endl;
			if (i->instructIn.substr(0, 2) == "B.") {
				j = 22;
				break;
			}
			else if (i->instructIn == op_str[j])
				break;
		}
		cout << "j = " << j << endl;
		opcode = op_str[j];
		destHold = i->destination;
		s1Hold = i->source1;
		s2Hold = i->source2;
		if (j >= 18) {
			if ((j >= 20) && (j<23)) {
				lineNO = i->pc;
				lineNO2 = calcJump(i->destination, lineNO);
				opcodeToDecimal = OP_INT[j];
				bFormat(opcode, destHold, s1Hold, s2Hold, opcodeToDecimal, lineNO, lineNO2);
			}
			else {
				lineNO = i->pc;
				lineNO2 = calcJump(i->destination, lineNO);
				cbFormat(opcode, destHold, s1Hold, s2Hold, opcodeToDecimal, lineNO2);
			}
		}
		else if (j < 9) {
			opcode = op_str[j];;
			destHold = i->destination;
			s1Hold = i->source1;
			s2Hold = i->source2;
			opcodeToDecimal = OP_INT[j];
			if ((j > 4) && (j < 7))
				rFormatShift(opcode, destHold, s1Hold, s2Hold, opcodeToDecimal);
			else rFormat(opcode, destHold, s1Hold, s2Hold, opcodeToDecimal);

		}
		else if ((j>=9) || (j<=17)) {
			opcode = op_str[j];
			destHold = i->destination;
			s1Hold = i->source1;
			s2Hold = i->source2;
			opcodeToDecimal = OP_INT[j];
			iFormat(opcode, destHold, s1Hold, s2Hold, opcodeToDecimal);
		}
	}
}


struct ireg {
	int opcode;
	int data1;
	int data2;
	int dest;
};


ireg ir;

int PC;
int NPC;
int muxPC_A;
int muxB_Imm;
int muxNPC_B;
int muxD_R;
int A;
int B;
int Imm;
int ALUout;
int regs[32];
int ALUop;
int lmd;

int fetch(forMem instruction);
void decode(int type);
void calcOp(int type, int opcode);
int execute(int a, int b);
void memory(int opcode);
void writeBack();
int mux(int choice, int op0, int op1);

void printTestFile() {
	int j = 0;
	int loop;
	write << "Instruction Memory Contents \n";
	cout << "Instruction Memory Contents \n";
	for (vector<forMem>::iterator i = IMEM.begin(); i != IMEM.end(); i++) {
		write << "IMEM[" << j << "] = " << i->opcode << " " << i->destination << " " << i->source1 << " " << i->source2 << "	";
		cout << "IMEM[" << j << "] = " << i->opcode << " " << i->destination << " " << i->source1 << " " << i->source2 << "	";
		loop = j % 3;
		if (loop == 0) {
			write << endl;
			cout << endl;
		}
		j = j + 1;
	}
	write << "\nData Memory Contents \n";
	cout << "\nData Memory Contents \n";
	
	for (int i = 0; i < val; i++) {
		write << "DMEM[" << i << "] = " << DMEMStart[i] << "	";
		cout << "DMEM[" << i << "] = " << DMEMStart[i] << "	";
		loop = i % 3;
		if (loop == 0) {
			write << endl;
			cout << endl;
		}
	}
	write << "\nRegister Contents \n";
	cout << "\nRegister Contents \n";
	for (int i = 0; i < val2; i++) {
		write << "REG[" << i << "] = " << registerStart[i] << "	";
		cout << "REG[" << i << "] = " << registerStart[i] << "	";
		loop = i % 3;
		if (loop == 0) {
			write << endl;
			cout << endl;
		}
	}
	write << endl << endl;
	cout << endl << endl;
	for (vector<forMem>::iterator i = IMEM.begin(); i != IMEM.end(); i++) {
		write << "Executing " << i->opcode << endl;
		cout << "Executing " << i->opcode << endl;
	}
	write << "\nData Memory Contents \n";
	cout << "\nData Memory Contents \n";
	for (int i = 0; i < val; i++) {
		write << "DMEM[" << i << "] = " << DMEM[i] << "	";
		cout << "DMEM[" << i << "] = " << DMEM[i] << "	";
		loop = i % 3;
		if (loop == 0) {
			write << endl;
			cout << endl;
		}
	}
	write << "\nRegister Contents \n";
	cout << "\nRegister Contents \n";
	for (int i = 0; i < val2; i++) {
		write << "REG[" << i << "] = " << regs[i] << "	";
		cout << "REG[" << i << "] = " << regs[i] << "	";
		loop = i % 3;
		if (loop == 0) {
			write << endl;
			cout << endl;
		}
	}
}


int main() {


	assemble();

	vector<forMem>::iterator i = IMEM.begin();
	PC = 0;
	while (i != IMEM.end()) {
		
		int flag = 0;
		NPC = PC + 1;
		int type = fetch(*i);
		calcOp(type, ir.opcode);
		decode(type);
		if (type == 5) {
			memory(ir.opcode);
			if (ir.opcode == 1984)
				flag = 1;
		}
		if ((type == 5 && !flag) || type < 3)
			writeBack();
		if (type == 3 || type == 4) {
			for (int j = 0; j < ir.data1; j++) {
				i--;
			}
		}
		else {
			i++;
		}
		PC = mux(muxNPC_B, NPC, ALUout);
		
	}
	printTestFile();
	system("pause");
}

int fetch(forMem instruction) {

	ir.opcode = instruction.opcode;
	for (int i = 0; i < 23; i++) {
		if (ir.opcode == OP_INT[i]) {
			if ((i == 16) || (i == 17)) {
				ir.data1 = instruction.source1;
				ir.data2 = instruction.source2;
				ir.dest = instruction.destination;
				return 5;
			}
			else if (i > 17) {
				if (i < 21) {
					ir.data1 = instruction.source1;
					return 4;
				}
				else {
					ir.data1 = instruction.source1;
					return 3;
				}
			}
			else if (i < 9) {
				if ((i > 4) && (i < 7)) {
					ir.data1 = instruction.source1;
					ir.data2 = instruction.source2;
					ir.dest = instruction.destination;
					return 1;
				}
				else {
					ir.data1 = instruction.source1;
					ir.data2 = instruction.source2;
					ir.dest = instruction.destination;
					return 2;
				}
			}
			else {
				ir.data1 = instruction.source1;
				ir.data2 = instruction.source2;
				ir.dest = instruction.destination;
				return 0;
			}

			break;
		}
	}
	return 'e';
}

void decode(int type) {
	switch (type) {

	case 0:
		A = regs[ir.data1];
		Imm = ir.data2;
		muxB_Imm = 1;
		muxPC_A = 1;
		muxD_R = 1;
		muxNPC_B = 0;
		Imm = ir.data2;
		ALUout = execute(mux(muxPC_A, PC, A), mux(muxB_Imm, B, Imm));
		break;

	case 1:
		A = regs[ir.data1];
		Imm = ir.data2;
		muxPC_A = 1;
		muxB_Imm = 0;
		muxD_R = 1;
		muxNPC_B = 0;
		ALUout = execute(mux(muxPC_A, PC, A), mux(muxB_Imm, B, Imm));
		break;

	case 2:
		A = regs[ir.data1];
		B = regs[ir.data2];
		muxPC_A = 1;
		muxB_Imm = 0;
		muxD_R = 1;
		muxNPC_B = 0;
		ALUout = execute(mux(muxPC_A, PC, A), mux(muxB_Imm, B, Imm));
		break;

	case 4:
		A = regs[ir.data2];
		Imm = ir.data1;
		muxPC_A = 0;
		muxB_Imm = 1;
		muxNPC_B = 1;
		ALUout = execute(mux(muxPC_A, PC, A), mux(muxB_Imm, B, Imm));
		break;

	case 3:
		Imm = ir.data1;
		muxPC_A = 0;
		muxB_Imm = 1;
		muxNPC_B = 1;
		ALUout = execute(mux(muxPC_A, PC, A), mux(muxB_Imm, B, Imm));
		break;

	case 5:
		if (ir.opcode == 1986) {
			A = regs[ir.data1];
			Imm = ir.data2;
			muxPC_A = 1;
			muxB_Imm = 1;
			muxD_R = 0;
			muxNPC_B = 0;

		}
		else {
			A = regs[ir.data1];
			Imm = ir.data2;
			muxPC_A = 1;
			muxB_Imm = 1;
			muxNPC_B = 0;

		}
		ALUout = execute(mux(muxPC_A, PC, A), mux(muxB_Imm, B, Imm));
		break;

	}
}

void calcOp(int type, int opcode) {

	int i;
	if (type > 2)
		ALUop = 1;		//Addition
	else {
		for (i = 0; i < 16; i++) {
			if (opcode == OP_INT[i])
				break;
		}
		if (i == 5)
			ALUop = 0;

		else if (i == 6)
			ALUop = 5;

		else {
			i = i % 9;
			switch (i) {
			case 0:
				ALUop = 1;
				break;

			case 1:
				ALUop = 2;
				break;

			case 2:
				ALUop = 3;
				break;

			case 3:
				ALUop = 4;

			case 4:
				ALUop = 5;
				break;

			case 5:
				ALUop = 2;
				break;

			case 7:
				ALUop = 1;
				break;

			case 8:
				ALUop = 2;
				break;

			}
		}

	}

} // add to execute if needed , int type, int opcode

int execute(int a, int b) {
	int out;

	switch (ALUop) {
	case 0:
		out = a * 2 * b;
		return out;
		break;

	case 1:
		out = a + b;
		return out;
		break;

	case 2:
		out = a - b;
		return out;
		break;

	case 3:
		out = a && b;
		return out;
		break;

	case 4:
		out = a || b;
		return out;
		break;

	case 5:
		out = a ^ b;
		return out;
		break;

	case 6:
		out = a / (2 * b);
		return out;
		break;
	}

	//return out;
}

void memory(int opcode) {

	if (opcode == 1986) {
		lmd = DMEM[ALUout];
	}
	//else if ((inputFile == "assignment5Code2") && (ALUout < 2)) {//not sure if this is needed to write to DMEM[2]
	//	DMEM[ALUout + 2] = B;
	//}
	else
		DMEM[ALUout] = B;

}

void writeBack() {
	regs[ir.dest] = mux(muxD_R, lmd, ALUout);

}

int mux(int choice, int op0, int op1) {

	switch (choice) {
	case 0: return op0; break;
	case 1: return op1; break;
	}

}



void branch(int cond) {

	if (cond == 0) {
		PC = mux(muxNPC_B, NPC, ALUout);
	}

}