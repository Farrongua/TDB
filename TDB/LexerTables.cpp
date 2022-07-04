#include "LexerGeneration.h"

int ident_count = 1001;
int const_count = 501;
int test_count = 5001;

map <string, int> kwrd = {
  {"PROCEDURE", 401},
  {"BEGIN", 402},
  {"END", 403},
  {"LABEL", 404},
  {"GOTO", 405},
  {"RETURN", 406}
};

map <string, int> sep = {
	{";", 59},
	{",", 44},
	{":", 58},
	{"(", 40},
	{")", 41},
	{"$", 36}
};

unordered_map <string, int> ident;
unordered_map <string, int> _const;
unordered_map <string, int> test;

int findID(string _token) {
	Token token;
	token.value = _token;
	map<string, int>::iterator iter;
	unordered_map<string, int>::iterator iter1;
	if (symbolClassifier(token.value[0]) == letters) {
		if (kwrd.count(token.value) == 1) {
			iter = kwrd.find(token.value);
			token.id = iter->second;
		}
		else if (ident.count(token.value) == 0) {
			ident.insert(make_pair(token.value, ident_count));
			token.id = ident_count;
			ident_count++;
		}
		else {
			iter1 = ident.find(token.value);
			token.id = iter1->second;
		}
	}
	else if (symbolClassifier(token.value[0]) == digits) {
		if (_const.count(token.value) == 0) {
			_const.insert(pair<string, int>(token.value, const_count));
			token.id = const_count;
			const_count++;
		}
		else {
			iter1 = _const.find(token.value);
			token.id = iter1->second;
		}
	}
	else if (symbolClassifier(token.value[0]) == tests) {
		if (test.count(token.value) == 0) {
			test.insert(pair<string, int>(token.value, test_count));
			token.id = test_count;
			test_count++;
		}
		else {
			iter1 = test.find(token.value);
			token.id = iter1->second;
		}
	}
	else if (sep.count(token.value) == 1) {
		iter = sep.find(token.value);
		token.id = iter->second;
	}

	return token.id;
}

void printTables(FILE* gen) {
	if (ident.size() != 0) {
		fprintf(gen, "\nIdentifier table\n");
		for (auto it : ident) {
			cout << it.first << " " << it.second << endl;
			fprintf(gen, "%s %d\n", it.first.c_str(), it.second);
		}
	}
	
	if (_const.size() != 0) {
		fprintf(gen, "\nConstant table\n");
		for (auto it : _const) {
			cout << it.first << " " << it.second << endl;
			fprintf(gen, "%s %d\n", it.first.c_str(), it.second);
		}
	}
	
}