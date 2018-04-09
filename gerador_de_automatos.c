#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>


int search(int element, int *end_states, int size)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        if (element == end_states[i])
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    int how_many_elements_in_alphabet = 0;
    int how_many_states = 0;
    int how_many_end_states = 0;
    int i = 0;
    int j = 0;
    FILE *fp;
    char filename[200];
    char *header = "#include <stdio.h>\n#include <curses.h>\n#include <stdlib.h>\n\nchar f[200];\nint p;\n\nvoid aceita();\nvoid rejeita();\n";
    char *accept = "void aceita()\n{\n\tprintf(\"Aceito\");\n\tgetch();\n\texit(0);\n}\n";
    char *rejected = "void rejeita()\n{\n\tprintf(\"Rejeitado\");\n\tgetch();\n\texit(0);\n}\n";

    printf("Quantos símbolos tem o alfabeto? \n");
    scanf("%d", &how_many_elements_in_alphabet);

    char alphabet[how_many_elements_in_alphabet];
    for (i = 0; i < how_many_elements_in_alphabet; i++) 
    {
        char symbol;
        printf("Qual o símbolo %d? \n", i);
        scanf("%s", &symbol);
        alphabet[i] = symbol;
    }

    printf("Quantos estados? \n");
    scanf("%d", &how_many_states);
    int states[how_many_states];
    
    printf("Quantos estados finais? \n");
    scanf("%d", &how_many_end_states);
    int end_states[how_many_end_states];

    for (i = 0; i < how_many_end_states; i++) 
    {
        int aux = 0;
        printf("Qual o estado final %d? \n", i);
        scanf("%d", &aux);
        end_states[i] = aux;
    }

    int initial_state = 0;
    printf("Qual o estado inicial? \n");
    scanf("%d", &initial_state);

    int transitions[how_many_states][how_many_elements_in_alphabet];
    for (i = 0; i < how_many_states; i++)
    {
        for (j = 0; j < how_many_elements_in_alphabet; j++)
        {
            printf("Para o estado e%d e símbolo %c, qual o próximo estado? \n", i, alphabet[j]);
            scanf("%d", &transitions[i][j]);
        }
    }

    printf("Qual o nome do programa? \n");
    scanf("%s", filename);

    char main_function[8000];
    sprintf(main_function, "int main()\n{\n\tp = 0;\n\tprintf(\"Linha: \\n\");\n\tgets(f);\n\te%d();\n\treturn 0;\n}\n", initial_state);
    /* char *functions = ""; */
    char prototypes[9000];
    for (i = 0; i < how_many_states; i++)
    {
        char prototype[300];
        sprintf(prototype, "void e%d();\n", i);
        strcat(prototypes, prototype);
    }

    char functions[100000];
    for (i = 0; i < how_many_states; i++)
    {
        sprintf(functions, "%s\nvoid e%d()\n{", functions, i);
        int exists = search(i, end_states, how_many_end_states);
        if (exists != -1)
        {
            sprintf(functions, "%s\n\tif (f[p] == 0)\n\t{\n\t\taceita();\n\t}", functions);
        }
        for (j = 0; j < how_many_elements_in_alphabet; j++)
        {
            if (transitions[i][j] != -1)
            {
                if (j == 0) 
                {
                    sprintf(functions, "%s\n\tif (f[p] == '%c')\n\t{\n\t\tp++;\n\t\te%d();\n\t}\n\t", functions, alphabet[j], transitions[i][j]);
                }
                else
                {
                    sprintf(functions, "%s\n\telse if (f[p] == '%c')\n\t{\n\t\tp++;\n\t\te%d();\n\t}\n\t", functions, alphabet[j], transitions[i][j]);
                }
            }
            else
            {
                sprintf(functions, "%s\n\telse if (f[p] == '%c')\n\t{\n\t\trejeita();\n\t}\n\t", functions, alphabet[j]);
            }
        }
        sprintf(functions, "%s\n}\n", functions);
    }

    fp = fopen(filename, "w");
    fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%s", header, prototypes, accept, rejected, main_function, functions);
    fclose(fp);


    printf("** Programa gerado **\n");

    return 0;
}

