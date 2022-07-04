#pragma once
#ifndef LEXERGENERATION_H
#define LEXERGENERATION_H

#include <iostream>
#include <string>

#include <cctype>
#include <algorithm>
#include <vector>
#include <typeinfo>
#include <cstring>
#include <stdio.h>
#include <map>
#include <list>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

enum symbolCategories {
    whitespaces,
    digits,
    letters,
    unifier,
    separators,
    errors,
    tests
};

struct Token {
    Token() {};
    Token(int _row, int _column, int _id, string _value) {
        row = _row;
        column = _column;
        id = _id;
        value = _value;
    }
    int row, column, id;
    string value;
};



vector<Token> getVectorToken();
void printTables(FILE *gen);

/* File operations */
Token* dumpToken(FILE* generated, int row, int column, string token, Token* tokenStruct, const int count);
void dumpLexError(FILE* generated, int row, int column, string undefinedToken);
void dumpTokError(FILE* generated, int row, int* column, char* err_symb, string token, int count);

/* Struct operations */
Token* AddToken(int row, int column, int id, string token, Token* tokenStruct, const  int count);
void showTokens();

/* Lexer operations */
bool lexer(FILE* test, FILE* gen);
int findID(string _token);
int symbolClassifier(char symbol);

#endif
