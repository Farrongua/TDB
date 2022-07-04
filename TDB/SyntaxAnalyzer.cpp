#include "LexerGeneration.h"
#include "BinTree.h"
#include "CodeGenerationr.h" 

vector<Token> vector_lexem;
FILE* gen;

Token getToken() {
	Token tmp = *vector_lexem.begin();
	vector_lexem.erase(vector_lexem.begin());
	return tmp;
}

Token checkKeyToken(Token checkToken, string keyToken) {
	if (checkToken.id == findID(keyToken)) {
		addChild(checkToken.id, checkToken.value);
	}
	else {
		errorOutput(key_word_not_found, checkToken, keyToken);
	}
	if (!vector_lexem.empty()) {
		checkToken = getToken();
	}

	return checkToken;
}

void errorOutput(int error_code, Token error_token, string token) {
	printTree(gen);
	switch (error_code) {
	case key_word_not_found:
		printf("Parser : Error. Key word \'%s\'[%d, %d] not found.\n", token.c_str(), error_token.row, error_token.column);
		fprintf(gen, "Parser : Error. Key word \'%s\'[%d, %d] not found.\n", token.c_str(), error_token.row, error_token.column);
		break;
	case delimiter_not_found:
		printf("Parser : Error. Delimiter \'%s\'[%d, %d] not found.\n", token.c_str(), error_token.row, error_token.column + error_token.value.size());
		fprintf(gen, "Parser : Error. Delimiter \'%s\'[%d, %d] not found.\n", token.c_str(), error_token.row, error_token.column + error_token.value.size());
		break;
	case ident_not_found:
		printf("Parser : Error [%d, %d]. Identifier not found.\n", error_token.row, error_token.column);
		fprintf(gen, "Parser : Error [%d, %d]. Identifier not found.\n", error_token.row, error_token.column);
		break;
	case const_not_found:
		printf("Parser : Error [%d, %d]. Unsigned integer not found.\n", error_token.row, error_token.column);
		fprintf(gen, "Parser : Error [%d, %d]. Unsigned integer not found.\n", error_token.row, error_token.column);
		break;
	case wrong_delimiter:
		printf("Parser : Error [%d, %d]. Wrong delimiter.\n", error_token.row, error_token.column);
		fprintf(gen, "Parser : Error [%d, %d]. Wrong delimiter.\n", error_token.row, error_token.column);
		break;
	case wrong_key_word:
		printf("Parser : Error [%d, %d]. Wrong key word.\n", error_token.row, error_token.column);
		fprintf(gen, "Parser : Error [%d, %d]. Wrong key word.\n", error_token.row, error_token.column);
		break;
	case no_equal_rows:
		printf("Parser : Error [%d, %d]. Tokens must be on the same line.\n", error_token.row, error_token.column);
		fprintf(gen, "Parser : Error [%d, %d]. Tokens must be on the same line.\n", error_token.row, error_token.column);
		break;
	case no_statement:
		printf("Parser : Error [%d, %d]. After the mark should be statement.\n", error_token.row, error_token.column);
		fprintf(gen, "Parser : Error [%d, %d]. After the mark should be statement.\n", error_token.row, error_token.column);
		break;
	case label_value_not_found:
		printf("Parser : Error [%d, %d]. After the mark should be label.\n", error_token.row, error_token.column + error_token.value.size());
		fprintf(gen, "Parser : Error [%d, %d]. After the mark should be label.\n", error_token.row, error_token.column + error_token.value.size());
		break;
	}
	exit(error_code);
}

void parsing(FILE* generated) {
	gen = generated;
	vector_lexem = getVectorToken();
	if (vector_lexem.size() == 0) {
		fprintf(generated, " File is empty");
	}
	createRoot(-1, "<signal-program>");
	program(getToken());
	printTree(gen);

	
}

void program(Token token) {
	addChild(-1, "<program>");
	gotoLastChild();

	Token checkKeyWord = checkKeyToken(token, "PROCEDURE");
	Nodes* currentNode = getCurrentNode();
	Token next_token = procedureIdentifier(token, checkKeyWord);
	setCurrentNode(currentNode);
	
	next_token = parametersList(checkKeyWord, next_token);
	setCurrentNode(currentNode);
	next_token = delimiters(checkKeyWord, next_token, 59);
	
	next_token = blok(next_token);
	
}

Token procedureIdentifier(Token prev_token, Token current_token) {
	if (prev_token.row == current_token.row) {
		addChild(-1, "<procedure-identifier>");
		gotoLastChild();
		
		return identifier(current_token);
	}
	else {
		errorOutput(no_equal_rows, current_token);
	}
}

Token identifier(Token token) {
	if (token.id > 1000) {
		addChild(-1, "<identifier>");
		gotoChild("<identifier>");
		addChild(token.id, token.value);
	}
	else {
		errorOutput(ident_not_found, token);
	}
	return getToken();
}

Token delimiters(Token prev_token, Token current_token, int token_id) {
	if (current_token.id > 0 && current_token.id < 255) {
		if (current_token.id == token_id) {
			if (prev_token.row == current_token.row) {
				addChild(current_token.id, current_token.value);
			}
			else {
				errorOutput(no_equal_rows, current_token);
			}
		}
		else {
			errorOutput(wrong_delimiter, current_token);
		}
	}
	else {
		char buff[2];
		buff[0] = (char)token_id;
		buff[1] = '\0';
		string token = string(buff);
		errorOutput(delimiter_not_found, prev_token, token);
	}
	if (!vector_lexem.empty()) {
		current_token = getToken();
	}
	return current_token;
}

Token variableIdentifier(Token prev_token, Token current_token) {
	if (prev_token.row == current_token.row) {
		addChild(-1, "<variable-identifier>");
		gotoLastChild();

		return identifier(current_token);
	}
	else {
		errorOutput(no_equal_rows, current_token);
	}
}

Token identifierList(Token prev_token, Token current_token) {
	bool isIdentifierList = false;
	addChild(-1, "<identifier-list>");
	gotoLastChild();
	Token next_token;
	Nodes* currentNode = getCurrentNode();
	if (current_token.id != 41) {
		isIdentifierList = true;
		next_token = delimiters(prev_token, current_token, 44);
		current_token = next_token;
		if (next_token.id > 1000) {
			next_token = variableIdentifier(current_token, next_token);
			setCurrentNode(currentNode);
			next_token = identifierList(current_token, next_token);
			return next_token;
		}
		else {
			errorOutput(ident_not_found, next_token);
		}
	}


	if (!isIdentifierList) {
		addChild(-1, "<empty>");
	}

	return current_token;
}

Token parametersList(Token prev_token, Token current_token) {
	bool isParameterList = false;
	addChild(-1, "<parameters-list>");
	gotoLastChild();
	Token next_token;
	Nodes* currentNode = getCurrentNode();
	if (current_token.id != 59) {
		isParameterList = true;
		next_token = delimiters(prev_token, current_token, 40);
		prev_token = next_token;
		next_token = variableIdentifier(current_token, next_token);
		setCurrentNode(currentNode);
		while (next_token.id != 41) {
			next_token = delimiters(prev_token, next_token, 44);
			current_token = next_token;
			next_token = variableIdentifier(current_token, next_token);
			setCurrentNode(currentNode);
		}
		/*current_token = identifierList(prev_token, next_token);
		setCurrentNode(currentNode);*/
		next_token = delimiters(prev_token, next_token, 41);
		
		
		current_token = next_token;
	}
	
	if (!isParameterList) {
		addChild(-1, "<empty>");
	}
	
	return current_token;
}


Token blok(Token current_token) {
	addChild(-1, "<block>");
	gotoLastChild();
	Token next_token;
	Nodes* currentNode = getCurrentNode();
	current_token = declaration(current_token);
	setCurrentNode(currentNode);
	next_token = checkKeyToken(current_token, "BEGIN");
	setCurrentNode(currentNode);
	current_token = statementList(current_token, next_token);
	setCurrentNode(currentNode);
	next_token = checkKeyToken(current_token, "END");
	gotoParent();
	next_token = delimiters(current_token, next_token, 59);
	return next_token;
}


Token declaration(Token current_token) {
	addChild(-1, "<declaration>");
	gotoLastChild();
	current_token = labelDeclaration(current_token);
	return current_token;
}

Token labelDeclaration(Token current_token) {
	bool isLabelDeclaration = false;
	addChild(-1, "<label-declaration>");
	gotoLastChild();

	Nodes* currentNode = getCurrentNode();
	
	if (current_token.id != 402) {
		Token prev_token = current_token;
		isLabelDeclaration = true;
		Token next_token = checkKeyToken(current_token, "LABEL");
		Nodes* currentNode = getCurrentNode();
		current_token = unsignedInteger(current_token, next_token);
		setCurrentNode(currentNode);
		while (current_token.id != 59) {
			next_token = delimiters(prev_token, current_token, 44);
			current_token = unsignedInteger(current_token, next_token);
			setCurrentNode(currentNode);
		}
		/*next_token = labelList(next_token, current_token);
		setCurrentNode(currentNode);*/
		next_token = delimiters(next_token, current_token, 59);
		current_token = next_token;
	}


	if (!isLabelDeclaration) {
		addChild(-1, "<empty>");
	}
	
	return current_token; 
}

Token unsignedInteger(Token prev_token, Token current_token) {
	if (prev_token.row == current_token.row) {
		if (current_token.id > 500 && current_token.id <= 1000) {
			addChild(-1, "<unsigned-integer>");
			gotoLastChild();
			addChild(current_token.id, current_token.value);
		}
		else {
			errorOutput(const_not_found, current_token);
		}
	}
	else {
		errorOutput(no_equal_rows, current_token);
	}
	return getToken();
}

Token labelList(Token prev_token, Token current_token) {
	bool isLabelList = false;
	addChild(-1, "<label-list>");
	gotoLastChild();
	Token next_token;
	Nodes* currentNode = getCurrentNode();
	
	if (current_token.id != 59) {
		
		isLabelList = true;
		next_token = delimiters(prev_token, current_token, 44);
		prev_token = next_token;
		if (next_token.id > 500 && next_token.id < 1000) {
			next_token = unsignedInteger(current_token, next_token);
			
		}
		else {
			errorOutput(label_value_not_found, current_token);
		}
		setCurrentNode(currentNode);
		if (prev_token.row != next_token.row) {
			errorOutput(delimiter_not_found, prev_token, ";");
		}
		next_token = labelList(prev_token, next_token);
		return next_token;
	}


	if (!isLabelList) {
		addChild(-1, "<empty>");
	}

	return current_token;
}

Token statement(Token prev_token, Token current_token) {
	addChild(-1, "<statement>");
	gotoLastChild();
	Nodes* currentNode = getCurrentNode();
	Token next_token;
	if (current_token.id > 500 && current_token.id <= 1000) {
		next_token = unsignedInteger(current_token, current_token);
		setCurrentNode(currentNode);
		current_token = delimiters(current_token, next_token, 58);
		//current_token = statement(current_token, current_token);
		setCurrentNode(currentNode);
	}
	else if (current_token.id > 400 && current_token.id <= 500) {
		if (current_token.id == 405) {
			prev_token = current_token;
			current_token = checkKeyToken(current_token, "GOTO");
			current_token = unsignedInteger(prev_token, current_token);
			setCurrentNode(currentNode);
			current_token = delimiters(prev_token, current_token, 59);
		}
		else if (current_token.id == 406) {
			prev_token = current_token;
			current_token = checkKeyToken(current_token, "RETURN");
			current_token = delimiters(prev_token, current_token, 59);
		}
		else {
			errorOutput(wrong_key_word, current_token);
		}
	}
	else if (current_token.id > 0 && current_token.id < 255) {
		if (current_token.id == 59) {
			current_token = delimiters(prev_token, current_token, 59);
		}
		else if (current_token.id == 40) {
			next_token = delimiters(prev_token, current_token, 40);
			current_token = delimiters(prev_token, next_token, 36);
			next_token = assemblyInsertFileIdentifier(next_token, current_token);
			setCurrentNode(currentNode);
			current_token = delimiters(current_token, next_token, 36);
			next_token = delimiters(next_token, current_token, 41);
			return next_token;
		}
		else {
			errorOutput(wrong_delimiter, current_token);
		}
	}
	else {
		errorOutput(no_statement, current_token);
	}

	return current_token;
}

Token statementList(Token prev_token, Token current_token) {
	bool isStatementList = false;
	addChild(-1, "<statement-list>");
	gotoLastChild();
	Nodes* currentNode = getCurrentNode();
	Token next_token;
	while (current_token.id != 403) {
		isStatementList = true;
		current_token = statement(current_token, current_token);
		setCurrentNode(currentNode);
		//current_token = statementList(current_token, current_token);
	}


	if (!isStatementList) {
		addChild(-1, "<empty>");
	}

	return current_token;
}

Token assemblyInsertFileIdentifier(Token prev_token, Token current_token) {
	addChild(-1, "<assembly-insert-file-identifier>");
	gotoLastChild();
	if (prev_token.row == current_token.row) {
		current_token = identifier(current_token);
	}
	else {
		errorOutput(no_equal_rows, current_token);
	}
	

	return current_token;
}

