#include "BinTree.h"
#include "LexerGeneration.h"
#include "LexerGeneration.h"

Nodes root;
Nodes* currentNode = &root;

void createRoot(int _lexem_code, string _lexem_name) {
	root.lexem_code = _lexem_code;
	root.lexem_name = _lexem_name;
	root.parent = NULL;
}

void addChild(int lexem_code, string lexem_name) {
	Nodes tmp(lexem_code, lexem_name, currentNode);
	currentNode->childNodes.push_back(tmp);
}

Nodes* getCurrentNode() {
	return currentNode;
}

void setCurrentNode(Nodes* newCurrentNode) {
	currentNode = newCurrentNode;
}

void gotoChild(int index) {
	setCurrentNode(&currentNode->childNodes[index]);
}

void gotoChild(string _lexem_name) {
	for (int i = 0; i < (int)currentNode->childNodes.size(); i++) {
		if (currentNode->childNodes[i].lexem_name == _lexem_name) {
			gotoChild(i);
			return;
		}
	}
}

void gotoBrother(int index) {
	gotoParent();
	gotoChild(index);
}

void gotoLastChild() {
	setCurrentNode(&currentNode->childNodes.back());
}

bool gotoParent() {
	if (currentNode == &root) return false;
	currentNode = currentNode->parent;
	return true;
}

void printTree(FILE* gen, Nodes tree, int _depth) {
	if (tree.lexem_code == -1) {
		cout << tree.lexem_name << endl;
		fprintf(gen, "%s\n", tree.lexem_name.c_str());
	}
	else {
		cout << tree.lexem_code << "  " << tree.lexem_name << endl;
		fprintf(gen, "%d %s\n", tree.lexem_code, tree.lexem_name.c_str());
	}

	if (!tree.childNodes.empty()) {
		for (int i = 0; i < (int)tree.childNodes.size(); i++) {
			for (int i = 0; i <= _depth; i++) {
				cout << "..";
				fprintf(gen, "..");
			}
			printTree(gen, tree.childNodes[i], _depth + 1);
		}
	}
}

void printTree(FILE* gen) {
	cout << endl << "Parse tree" << endl;
	fprintf(gen, "\nParse tree\n");
	printTree(gen, root, 0);
}

string getNodeName() {
	return currentNode->lexem_name;
}

Nodes* getLinkRoot() {
	return &root;
}

Nodes getRoot() {
	return root;
}

