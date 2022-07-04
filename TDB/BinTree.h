#pragma once
#ifndef BIN_TREE_H
#define BIN_TREE_H

#include "LexerGeneration.h"
#include "CodeGenerationr.h"

struct Nodes {
    Nodes() {};
    Nodes(int _lexem_code, string _lexem_name, Nodes* _parent) {
        lexem_code = _lexem_code;
        lexem_name = _lexem_name;
        parent = _parent;
    }
    int lexem_code;
    string lexem_name;
    Nodes* parent;
    vector<Nodes> childNodes;
};


enum errorCode {
    key_word_not_found,
    delimiter_not_found,
    ident_not_found,
    const_not_found,
    wrong_delimiter,
    wrong_key_word,
    no_equal_rows,
    no_statement,
    label_value_not_found
};

void parsing(FILE* generated);
void createRoot(int _lexem_code, string _lexem_name);
void addChild(int _lexem_code, string _lexem_name);
void gotoChild(string _lexem_name);
void gotoChild(int index);
void setCurrentNode(Nodes* child);
void gotoLastChild();
void gotoBrother(int index);
Nodes* getCurrentNode();
bool gotoParent();
Nodes* getLinkRoot();
Nodes getRoot();
string getNodeName();

Token getToken();
Token checkKeyToken(Token checkToken, string keyToken);
Token delimiters(Token prev_token, Token current_token, int token_id);

void program(Token token);
Token identifier(Token token);
Token procedureIdentifier(Token prev_token, Token current_token);
Token parametersList(Token prev_token, Token current_token);
Token variableIdentifier(Token prev_token, Token current_token);
Token identifierList(Token prev_token, Token current_token);
Token blok(Token current_token);
Token declaration(Token current_token);
Token labelDeclaration(Token current_token);
Token unsignedInteger(Token prev_token, Token current_token);
Token labelList(Token prev_token, Token current_token);
Token statementList(Token prev_token, Token current_token);
Token statement(Token prev_token, Token current_token);
Token assemblyInsertFileIdentifier(Token prev_token, Token current_token);



void errorOutput(int error_code, Token error_token = Token(), string token = "");
void printTree(FILE* gen, Nodes _tree, int _depth);
void printTree(FILE* gen);
#endif // !BIN_TREE_H
