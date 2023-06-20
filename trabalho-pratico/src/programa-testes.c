#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <ncurses.h>

#include "../includes/utils.h"
#include "../includes/structs.h"
#include "../includes/stats.h"
#include "../includes/lists.h"
#include "../includes/query_manager.h"
#include "../includes/parser.h"

#include <time.h>

#define KCYN "\x1B[36m"
#define KWHT "\033[0;37m"

int compare_files (int size, int query) {

    for (int i = 0; i < size; i++) {

        char *file = malloc(sizeof(char) * 25);
        sprintf(file, "command%d_output.txt", 100*query + i);
        char *file1 = concat("./Resultados/", file);
        free(file);

        FILE *fp = fopen(file1, "r");

        if (fp == NULL) {
            printf("Erro ao abrir o ficheiro: %s\n", file1);
            free(file1);
            return 0;
        }
        free(file1);


        char *file2 = malloc(sizeof(char) * 50);
        sprintf(file2, "./tests/output_query_%d/command%d_output.txt", query, 100*query + i);

        FILE *fp2 = fopen(file2, "r");

        if (fp2 == NULL) {
            printf("Erro ao abrir o ficheiro: %s.\n", file2);
            free(file2);
            return 0;
        }
        free(file2);

        int line = 1;

        char *current_line1 = NULL;
        char *current_line2 = NULL;
        size_t len1 = 0, len2 = 0;
        while (getline(&current_line1, &len1, fp) != -1 && getline(&current_line2, &len2, fp2)) {            
            if (strcmp(current_line1, current_line2)) {
                printf("\nQ%d - Erro no ficheiro: %d; linha: %d\n", query, i+100*query, line);
                printf("Correto:   %s", current_line2);
                printf("Incorreto: %s\n", current_line1);
                return 0;
            }
            line++;
        }
        
        fclose(fp);
        fclose(fp2);
    }
    return 1;
}

void test_query_n (LIST *lists, STATS *stats, HASH *hash, int query, char **input, int input_len, int repeat) {
    clock_t begin, end;

    double result = 0;
    for (int i = 0; i < input_len*repeat; i++) {
        begin = clock();
        query_manager(lists, stats, hash, query, (100*query)+(i%input_len), strdup(input[i%input_len]));
        end = clock();
        result += (double) (end - begin) / CLOCKS_PER_SEC;
    }

    result = result / (input_len*repeat);
    printf("--------------------------------------------------------\n");
    printf("Média de %d testes da query %s%d%s: %lf seg\n", input_len*repeat, KCYN, query, KWHT, result);

    if (compare_files(input_len, query))
        printf("A query %d passou em todos os testes.\n", query);
    else
        printf("A query %d não passou em todos os testes.\n", query);
    printf("--------------------------------------------------------\n\n\n");

}

int main (int argc, char **argv) {

    if (argc >= 2) {

        double result;
        clock_t begin, end;
        begin = clock();

        HASH *hash = create_hash();
        STATS *stats = create_stats();
        
        parser(argv[1], hash);
        calc_stats(hash, stats);
        LIST *lists = create_lists(stats, hash);

        end = clock();
        result = (double) (end - begin) / CLOCKS_PER_SEC;
        
        printf("\n[              Loading time: %lf seg.            ]\n\n", result);

        char *input_q1[20] = {"PetrPacheco",
                              "ÂngVieira",
                              "LeTavares103",
                              "NoMaia",
                              "ErPinheiro7",
                              "FrederAraújo",
                              "PeFernandes144",
                              "ASousa7",
                              "RReis26",
                              "TerLima",
                              "000000002639",
                              "000000008561",
                              "000000004987",
                              "000000001936",
                              "000000003518",
                              "000000002581",
                              "000000004456",
                              "000000000879",
                              "000000009721",
                              "000000003558"};

        char *input_q2[2] = {"10", "50"};
        char *input_q3[2] = {"10", "50"};
        char *input_q4[2] = {"Faro", "Setúbal"};

        char *input_q5[5] = {"01/01/2021 01/01/2021",
                             "02/01/2021 08/01/2021",
                             "01/02/2021 01/03/2021",
                             "01/06/2021 31/12/2021",
                             "01/01/2012 01/01/2022"};

        char *input_q6[5] = {"Faro 01/01/2021 01/01/2021",
                             "Setúbal 02/01/2021 08/01/2021",
                             "Lisboa 01/02/2021 01/03/2021",
                             "Porto 01/06/2021 31/12/2021",
                             "Braga 01/01/2012 01/01/2022"};

        char *input_q7[6] = {"50 Braga"
                             "50 Lisboa",
                             "50 Porto",
                             "50 Setúbal",
                             "50 Faro",
                             " "};

        char *input_q8[7] = {"M 13"
                             "M 12",
                             "M 11",
                             "F 13",
                             "F 12",
                             "F 11", 
                             " "};

        char *input_q9[6] = {"01/01/2021 01/01/2021",
                             "02/01/2021 08/01/2021",
                             "01/02/2021 01/03/2021",
                             "01/06/2021 31/07/2021",
                             "01/01/2012 01/01/2013",
                             " "};

        test_query_n(lists, stats, hash, 1, input_q1, 20, 2);
        test_query_n(lists, stats, hash, 2, input_q2, 2,  1);
        test_query_n(lists, stats, hash, 3, input_q3, 2,  1);
        test_query_n(lists, stats, hash, 4, input_q4, 2,  1);
        test_query_n(lists, stats, hash, 5, input_q5, 5,  1);
        test_query_n(lists, stats, hash, 6, input_q6, 5,  1);
        test_query_n(lists, stats, hash, 7, input_q7, 5,  1);
        test_query_n(lists, stats, hash, 8, input_q8, 6,  1);
        test_query_n(lists, stats, hash, 9, input_q9, 5,  1);

        destroy_hash(hash);
        destroy_stats(stats);
        destroy_lists(lists);
    }

    return 0;
}