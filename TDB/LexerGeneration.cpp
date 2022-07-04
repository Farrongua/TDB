#include "LexerGeneration.h"
vector<Token> token_vector;

vector<Token> getVectorToken() {
	return token_vector;
}

Token* dumpToken(FILE* generated, int row, int column, string token, Token* tokenStruct, const int count) {
	tokenStruct = AddToken(row, column, findID(token), token, tokenStruct, count);
	fprintf(generated, " %4d | %6d | %11d | %s\n", row, column, findID(token), token.c_str());
	return tokenStruct;
}

void dumpTokError(FILE* generated, int row, int *column, char *err_symb, string token, int count) {
	fprintf(generated, " Lexer : Error. Illegam symbol : ");
	for (int i = 0; i < count; i++) {
		fprintf(generated, "'%c'[%d, %d] ", err_symb[i], row, column[i]);
	}
	fprintf(generated, "in %s\n", token.c_str());
}

void dumpLexError(FILE* generated, int row, int column, string token) {
	fprintf(generated, " Lexer : Error. Illegam symbol : '%s'[%d, %d]\n", token.c_str(), row, column);
}

Token* AddToken(int row, int column, int id, string token, Token* tokenStruct, const  int count) {
	/*if (count == 0) {
		tokenStruct = new Token[count + 1];
	}
	else {
		Token* tmpToken = new Token[count + 1];
		for (int i = 0; i < count; i++) {
			tmpToken[i] = tokenStruct[i];
		}
		delete[] tokenStruct;

		tokenStruct = tmpToken;
	}
	tokenStruct[count].row = row;
	tokenStruct[count].column = column;
	tokenStruct[count].id = id;
	tokenStruct[count].value = token;*/

	Token tmp(row, column, id, token);
	token_vector.push_back(tmp);
	
	return tokenStruct;
}

void showTokens() {
	for (vector<Token>::iterator it = token_vector.begin(); it != token_vector.end(); it++ ) {
		cout << it->row << " | " << it->column << " | " << it->id << " | " << it->value << endl;
	}
}

int symbolClassifier(char symbol) {
	if (symbol == 32 || symbol == 13 || symbol == 10 || symbol == 9 || symbol == 11 || symbol == 12) {
		return whitespaces;
	}
	else if (48 <= symbol && symbol <= 57) { //from '0' to '9'
		return digits;
	}
	else if ((65 <= symbol && symbol <= 90) || (97 <= symbol && symbol <= 122)) { //from 'A' to 'Z' or from 'a' to 'z'
		return letters;
	}
	else if (symbol == 59 || symbol == 58 || symbol == 44 || symbol == 36 || symbol == 40 || symbol == 41) {
		return separators;
	}
	/*else if (symbol == 35) {
		return tests;
	}*/
	else if (symbol != -1) {
		return errors;
	}
	
}

bool lexer(FILE* test, FILE* gen) {
	fprintf(gen, " Line | Column | Ident token | Token\n-------------------------------------------------\n");
	char symbol = fgetc(test);
	char buff[255], err_symbols[255];
	string lexem;
	int row = 1, column = 1, token_count = 0, buffLen, unifier_col, unifier_row, err_count, err_column[255];
	bool err_flag = false;
	Token* token_struct = 0;
	bool error_check = false;
	
	while (symbol != -1) {
		switch (symbolClassifier(symbol)) {
		case whitespaces :
			while (symbolClassifier(symbol) == whitespaces) {
				if (symbol == 9) {
					column += 6;
				}
				column++;
				if (symbol == 10) {
					row++;
					column = 1;
				}
				symbol = fgetc(test);
			}
			break;
		case digits:
			buffLen = 0;
			err_count = 0;
			while (symbolClassifier(symbol) == digits || symbolClassifier(symbol) == errors 
				|| symbolClassifier(symbol) == letters) 
			{
				if (symbolClassifier(symbol) == errors || symbolClassifier(symbol) == letters) {
					err_flag = true;
					err_symbols[err_count] = symbol;
					err_column[err_count] = column + buffLen;
					err_count++;
				}
				buff[buffLen] = symbol;
				buffLen++;
				symbol = fgetc(test);
			}
			buff[buffLen] = '\0';
			lexem = string(buff);
			if (err_flag == false) {
				token_struct = dumpToken(gen, row, column, lexem, token_struct, token_count);
				token_count++;
			}
			else {
				error_check = true;
				dumpTokError(gen, row, err_column, err_symbols, lexem, err_count);
			}
			column += buffLen;
			err_flag = false;
			break;
		case letters:
			buffLen = 0;
			err_count = 0;
			while (symbolClassifier(symbol) == digits || symbolClassifier(symbol) == errors
				|| symbolClassifier(symbol) == letters)
			{
				if (symbolClassifier(symbol) == errors) {
					err_flag = true;
					err_symbols[err_count] = symbol;
					err_column[err_count] = column + buffLen;
					err_count++;
				}
				buff[buffLen] = symbol;
				buffLen++;
				symbol = fgetc(test);
			}
			buff[buffLen] = '\0';
			lexem = string(buff);
			if (err_flag == false) {
				token_struct = dumpToken(gen, row, column, lexem, token_struct, token_count);
				token_count++;
			}
			else {
				error_check = true;
				dumpTokError(gen, row, err_column, err_symbols, lexem, err_count);
			}
			column += buffLen;
			err_flag = false;
			break;
		case separators:
			if (symbol == 59) { // ;
				token_struct = dumpToken(gen, row, column, ";", token_struct, token_count);
				token_count++;
				column++;
				symbol = fgetc(test);
				break;
			}
			else if (symbol == 58) { //:
				token_struct = dumpToken(gen, row, column, ":", token_struct, token_count);
				token_count++;
				column++;
				symbol = fgetc(test);
				break;
			}
			else if (symbol == 44) { // ,
				token_struct = dumpToken(gen, row, column, ",", token_struct, token_count);
				token_count++;
				column++;
				symbol = fgetc(test);
				break;
			}
			if (symbol == 40) { // (
				unifier_row = row;
				unifier_col = column;
				symbol = fgetc(test);
				column++;
				if (symbol == 42) { // *
					while (true) {
						if (symbol == 10) {
							row++;
							column = 0;
						}
						if (symbol == -1) {
							fprintf(gen, " Lexer : Error. Unclosed commet [%d, %d]\n", unifier_row, unifier_col);
							error_check = true;
							break;
						}
						if (symbol == 42) {
							column++;
							symbol = fgetc(test);
							if (symbol == 41) {
								column++;
								break;
							}
						}
						else {
							symbol = fgetc(test);
							column++;
						}
					}
					symbol = fgetc(test);
				}
				else {
					token_struct = dumpToken(gen, unifier_row, unifier_col, "(", token_struct, token_count);
					token_count++;
					break;
				}
			}
			else if (symbol == 41) { // )
				token_struct = dumpToken(gen, row, column, ")", token_struct, token_count);
				token_count++;
				column++;
				symbol = fgetc(test);
				break;
			}
			else if (symbol == 36) { // $
				token_struct = dumpToken(gen, row, column, "$", token_struct, token_count);
				token_count++;
				column++;
				symbol = fgetc(test);
				break;
			}
			break;
		case errors:
			error_check = true;
			buffLen = 0;
			err_count = 0;
			while (symbolClassifier(symbol) == digits || symbolClassifier(symbol) == errors
				|| symbolClassifier(symbol) == letters)
			{
				if (symbolClassifier(symbol) == errors) {
					err_flag = true;
					err_symbols[err_count] = symbol;
					err_column[err_count] = column + buffLen;
					err_count++;
				}
				buff[buffLen] = symbol;
				buffLen++;
				symbol = fgetc(test);
			}
			buff[buffLen] = '\0';
			lexem = string(buff);
			if (buffLen > 1)
				dumpTokError(gen, row, err_column, err_symbols, lexem, err_count);
			else
				dumpLexError(gen, row, column, lexem);
			column += buffLen;
			err_flag = false;
			break;
		case tests:
			buffLen = 0;
			err_count = 0;
			buff[buffLen] = symbol;
			buffLen++;
			symbol = fgetc(test);
			//#38-0??-???-??-??
			for (buffLen; buffLen < 17; buffLen++) {
				if (buffLen == 1) { 
					if (symbol != 51) {
						err_flag = true;
						err_symbols[err_count] = symbol;
						err_column[err_count] = column + buffLen;
						err_count++;
					}
					buff[buffLen] = symbol;
					symbol = fgetc(test);
					continue;
				}
				if (buffLen == 2) {
					if (symbol != 56) {
						err_flag = true;
						err_symbols[err_count] = symbol;
						err_column[err_count] = column + buffLen;
						err_count++;
					}
					buff[buffLen] = symbol;
					symbol = fgetc(test);
					continue;
				}
				if (buffLen == 3 || buffLen == 7 || buffLen == 11 || buffLen == 14) {
					if (symbol != 45) {
						err_flag = true;
						err_symbols[err_count] = symbol;
						err_column[err_count] = column + buffLen;
						err_count++;
					}
					buff[buffLen] = symbol;
					symbol = fgetc(test);
					continue;
				}
				if (buffLen == 4 ) {
					if (symbol != 48) {
						err_flag = true;
						err_symbols[err_count] = symbol;
						err_column[err_count] = column + buffLen;
						err_count++;
					}
					buff[buffLen] = symbol;
					symbol = fgetc(test);
					continue;
				}
				if (symbolClassifier(symbol) == digits) {
					buff[buffLen] = symbol;
					symbol = fgetc(test);
					continue;
				}
				else {
					err_flag = true;
					err_symbols[err_count] = symbol;
					err_column[err_count] = column + buffLen;
					err_count++;
					buff[buffLen] = symbol;
					symbol = fgetc(test);
					continue;
				}
			}
			buff[buffLen] = '\0';
			lexem = string(buff);
			if (err_flag == false) {
				token_struct = dumpToken(gen, row, column, lexem, token_struct, token_count);
				token_count++;
			}
			else {
				fprintf(gen, "  Lexer : Error. Incorrect number.\n ");
				dumpTokError(gen, row, err_column, err_symbols, lexem, err_count);
			}
			column += buffLen;
			err_flag = false;
			break;
			
		}
	}
	showTokens();
	printTables(gen);
	return error_check;
}