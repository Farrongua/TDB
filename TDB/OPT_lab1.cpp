//1. < signal - program > -- > < program>
//2. < program > -- > PROCEDURE <procedure - identifier> <parameters - list>; <block>;
//3. < block > -- > <declarations> BEGIN <statements-list> END
//4. < declarations > -- > < label - declarations>
//5. < label - declarations > -- > LABEL <unsigned-integer> <labels - list>; | < empty>
//6. < labels - list > -- > , <unsigned - integer> <labels - list> | < empty>
//7. < parameters - list > -- > (<variable - identifier> <identifiers - list>) | < empty>
//8. < identifiers - list > -- > , <variable - identifier> <identifiers - list> | < empty>
//9. < statements - list > -- > <statement> <statements-list> | < empty>
//10. < statement > -- > <unsigned - integer> : <statement> | GOTO <unsigned - integer>; | RETURN; | ; | ($ <assembly - insert - file - identifier> $)
//11. < variable - identifier > -- > < identifier>
//12. < procedure - identifier > -- > < identifier>
//13. < assembly - insert - file - identifier > -- > < identifier>
//14. < identifier > -- > <letter> < string>
//15. < string > -- > <letter><string> | <digit><string> | < empty>
//16. < unsigned - integer > -- > <digit> < digits - string>
//17. < digits - string > -- > <digit><digits - string> | <empty>
//18. < digit > -- > 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
//19. < letter > -- > A | B | C | D | ... | Z

#include "LexerGeneration.h"
#include "BinTree.h"
#include "CodeGenerationr.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Lexer: Invalid number of parameters.");
        return 1;
    }
    else {
        for (int i = 1; i < argc; i++) {
            printf("%s \n", argv[i]);
        }

    }
  
    FILE* test, * gen;
    char input[30];
    char output[30];
    char inputfile[] = "\\input.sig";
    char outputfile[] = "\\generated.txt";

    // For Visual Studio 2019
    strcpy_s(input, _countof(input), argv[1]);
    strcat_s(input, _countof(input), inputfile);
    strcpy_s(output, _countof(output), argv[1]);
    strcat_s(output, _countof(output), outputfile);
    errno_t err_test, err_gen;
    if ((err_test = fopen_s(&test, input, "r") != 0) || (err_gen = fopen_s(&gen, output, "w") != 0)) {
        return 1;
    }

    // For g++
    /*strcpy(input, argv[1]);
    strcat(input, inputfile);
    strcpy(output, argv[1]);
    strcat(output, outputfile);
    if (((test = fopen(input, "r")) == NULL) || ((gen = fopen(output, "w")) == NULL)) {
        return 1;
    }*/

    

    else {
        if (!lexer(test, gen)) {
            parsing(gen);
            codeGeneraion(gen);
        }
        else
            fprintf(gen, "Pasrer cannt work : Lexer found errors\n");
        fclose(test);
        fclose(gen);
    }

    return 0;

}