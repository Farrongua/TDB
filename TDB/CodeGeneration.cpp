#include "BinTree.h"
#include "LexerGeneration.h"
#include "CodeGenerationr.h"

FILE* gener;
list<string> asm_code;
list<string> asm_data;
list<label*> labels;
list<param*> params;
int param_asm = 20;

void assemFile(Nodes* current_Node);

void printCode() {
	for (string s: asm_code) {
		cout << s << endl;
		fprintf(gener, "%s \n", s.c_str());
	}
}

label* get_label(string label_name) {
	for (label* l: labels) {
		if (l->value == label_name)
			return l;
	}

	return nullptr;
}

void errorCodeOutput(int error_code, string lexem_name = "") {
	printCode();
	switch (error_code) {
	case double_labels :
		cout << "\nCode Generator : Error: different labels cannot have the same name" << endl;
		fprintf(gener, "\nCode Generator : Error: different labels cannot have the same name\n");
		break;
	case double_params:
		cout << "\nCode Generator : Error: different parameters cannot have the same name" << endl;
		fprintf(gener, "\nCode Generator : Error: different parameters cannot have the same name\n");
		break;
	case similar_prog_name:
		cout << "\nCode Generator : Error :The identifier \"" << lexem_name << "\" cannot be similar to the procedure name" << endl;
		fprintf(gener, "\nCode Generator : Error: The identifier \"%s\" cannot be similar to the procedure name\n", lexem_name.c_str());
		break;
	case not_init_label:
		cout << "\nCode Generator : Error: Use undeclarated label \"" + lexem_name + "\"";
		fprintf(gener, "\nCode Generator : Error: Use undeclarated label \"%s\" \n", lexem_name.c_str());
		break;
	case asm_file_not_found:
		cout << "\nCode Generator : Error: Insert assembler file \"" + lexem_name + "\" not found \n";
		fprintf(gener, "\nCode Generator : Error: Insert assembler file \"%s\" not found \n", lexem_name.c_str());
		break;
	case used_but_not_added_label:
		cout << "\nCode Generator : Error: label \"" + lexem_name + "\" was called but not added\n";
		fprintf(gener, "\nCode Generator : Error: Error: label \"%s\" was called but not added\n", lexem_name.c_str());

	}
	

	exit(error_code);
}


void paramCheck(Nodes* currentNode, string proc_name) {
	for (int i = 0; i < (int)currentNode->childNodes.size(); i++) {
		if (currentNode->childNodes[i].lexem_name == "<variable-identifier>") {
			if (currentNode->childNodes[i].childNodes[0].childNodes[0].lexem_name != proc_name) {
				params.push_back(new param(currentNode->childNodes[i].childNodes[0].childNodes[0].lexem_name));
				string asm_param = "\tMOV DWORD PTR[rqb-" + to_string(param_asm) + "], " + currentNode->childNodes[i].childNodes[0].childNodes[0].lexem_name + "\n";
				param_asm += 4;
				printf("%s", asm_param.c_str());
			}
			else
				errorCodeOutput(similar_prog_name, currentNode->childNodes[i].childNodes[0].childNodes[0].lexem_name);
		}
	}
	for (param* p1 : params) {
		for (param* p2 : params) {
			if (p1 != p2 && p1->value == p2->value)
				errorCodeOutput(double_params, currentNode->childNodes[0].lexem_name);				
		}
	}
}


void labelCheck(Nodes* currentNode) {
	for (int i = 0; i < (int)currentNode->childNodes.size(); i++) {
		if (currentNode->childNodes[i].lexem_name == "<unsigned-integer>") {
			labels.push_back(new label(currentNode->childNodes[i].childNodes[0].lexem_name, false, false));
		}
	}
	for (label* l1 : labels) {
		for (label* l2 : labels) {
			if (l1 != l2 && l1->value == l2->value) {
				errorCodeOutput(double_labels, currentNode->childNodes[0].lexem_name);
			}
		}
	}
}

void checkCorrectLabels() {
	for (label* l : labels) {
		if (l->wasAdd == false && l->wasCall == true) {
			errorCodeOutput(used_but_not_added_label, l->value);
		}
	}
}

void statementsCheck(Nodes* currentNode) {
	if (currentNode->childNodes[0].lexem_name == "<empty>") {
		asm_code.push_back("\tNOP");
		return;
	}
	for (int i = 0; i < (int)currentNode->childNodes.size(); i++) {
		setCurrentNode(currentNode);
		if (currentNode->childNodes[i].childNodes[0].lexem_name == "<unsigned-integer>") {
			label* l1 = get_label(currentNode->childNodes[i].childNodes[0].childNodes[0].lexem_name);
			if (l1 == nullptr) {
				errorCodeOutput(not_init_label, currentNode->childNodes[i].childNodes[0].childNodes[0].lexem_name);
			}
			l1->wasAdd = true;
			asm_code.push_back(l1->value + ":\n");
		}
		if (currentNode->childNodes[i].childNodes[0].lexem_name == "GOTO"){
			label* l1 = get_label(currentNode->childNodes[i].childNodes[1].childNodes[0].lexem_name);
			if (l1 == nullptr) {
				errorCodeOutput(not_init_label, currentNode->childNodes[i].childNodes[1].childNodes[0].lexem_name);
			}
			l1->wasCall = true;
			asm_code.push_back("\tJMP " + l1->value + "\n");
		}
		if (currentNode->childNodes[i].childNodes[0].lexem_name == "RETURN") {
			asm_code.push_back("\tMOV ax, 4C00h\
								\n\tINT 21h\n");
		}
		if (currentNode->childNodes[i].childNodes[0].lexem_code == 40) {
			gotoChild(i);
			gotoChild(2);
			assemFile(getCurrentNode());
		}
	}
}


void assemFile(Nodes* currentNode) {
	string asmFileName = currentNode->childNodes[0].childNodes[0].lexem_name;
	ifstream asmFile(asmFileName);

	if (!asmFile) {
		errorCodeOutput(asm_file_not_found, currentNode->childNodes[0].childNodes[0].lexem_name);
	}
	stringstream buff;
	buff << asmFile.rdbuf();
	asm_code.push_back(buff.str());
}

void generation(Nodes* tree) {
	gotoChild(0);
	tree = getCurrentNode();
	gotoChild(1); //procedure name
	gotoChild(0);
	gotoChild(0);
	string program_name = getNodeName();
	asm_data.push_back("\ndata	SEGMENT\
                        \ndata	ENDS\n\
                        \ncode	SEGMENT\
                        \n\tASSUME   cs:code, ds:data\n" + program_name + ":\n");
	for (string s : asm_data) {
		cout << s;
		fprintf(gener, "%s", s.c_str());
	}
		
	
	setCurrentNode(tree);
	gotoChild(2); //parameter list
	paramCheck(getCurrentNode(), program_name);
	
	setCurrentNode(tree);
	gotoChild(4); //blok
	tree = getCurrentNode();
	gotoChild(0);
	gotoChild(0); //label declaration
	labelCheck(getCurrentNode());
	setCurrentNode(tree);
	gotoChild(2); //statements list
	statementsCheck(getCurrentNode());
	checkCorrectLabels();
	printCode();
	asm_data.clear();
	asm_data.push_back("\n\ncode	ENDS\
                \n\tend	" + program_name + "\n");
	for (string s : asm_data) {
		cout << s;
		fprintf(gener, "%s", s.c_str());
	}

}


bool codeGeneraion(FILE* generated) {
	gener = generated;
	Nodes* rootNode = getLinkRoot();
	setCurrentNode(rootNode);
	generation(rootNode);
	
	return 0;
}