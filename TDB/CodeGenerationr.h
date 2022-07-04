#pragma once
#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "BinTree.h"
#include "LexerGeneration.h"



struct label {
	label(string value, bool wasAdd, bool wasCall) {
		this->value = value;
		this->wasAdd = wasAdd;
		this->wasCall = wasCall;
	}
	string value;
	bool wasAdd;
	bool wasCall;
};

struct param {
	param(string value) {
		this->value = value;
	}
	string value;
};

enum codeGenErrorCode {
	double_labels,
	double_params,
	similar_prog_name,
	not_init_label,
	asm_file_not_found,
	used_but_not_added_label
};


bool codeGeneraion(FILE* generated);

#endif // !CODE_GENERATION_H
