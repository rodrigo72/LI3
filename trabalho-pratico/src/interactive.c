#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../includes/query_manager.h"
#include "../includes/structs.h"
#include "../includes/parser.h"
#include "../includes/stats.h"
#include "../includes/lists.h"

#include "../includes/utils.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define SIZE 100
#define MAX_LINES 15

void clear_terminal () {
    // fprintf(stdout, "\033[2J\033[0;0H");
    fprintf(stdout, "\033c");
    fflush(stdout);
}

void pls_wait () {
    clear_terminal();
    printf("\n\n%sPlease wait, this may take a while.\n\n\n", KCYN);
}

int n_lines () {
    FILE *fp = fopen("./Resultados/command0_output.txt", "r+");
    int n = 0;
    char *aux = NULL; size_t size = 0;
    while (getline(&aux, &size, fp) != -1) 
        n++; 
    free(aux);
    fclose(fp);
    printf("\n\n%d\n\n", n);
    return n;
}

int pagina (int pagina, char l[MAX_LINES][SIZE]) {
    int n = n_lines();
    FILE *fp = fopen("./Resultados/command0_output.txt", "r+");
    int max_pags = (n/MAX_LINES) + !!(n%MAX_LINES);
    pagina = clamp (pagina, 0, max_pags);
    if (pagina == max_pags) pagina--;
    int beg = pagina*MAX_LINES;
    char *str = NULL; size_t size = 0;
    for (int i = 0, j = 0; i < (pagina+1) * MAX_LINES; i++) {
        int a = getline(&str, &size, fp);
        str = strsep(&str, "\n");
        if (i >= beg && i < beg + MAX_LINES) {
            if (a != -1)
                snprintf(l[j], SIZE, "%d\t%s", i+1, str);
            else
                strncpy(l[j], "\0", 2);
            j++;
        }
    }
    fclose(fp);
    return pagina;
}

void interact_query_1 (LIST *lists, STATS *stats, HASH *hash) {

    char *line = NULL;
    int flag = 1;
    char l1[SIZE]; l1[0] = '*';
    char l2[SIZE]; l2[0] = '*';
    while (flag) {

        clear_terminal();
        printf("%s", KWHT);
        printf("%sQuery 1:                                                                                   q - back%s\n", KGRN, KWHT);
        printf(  "Description: Returns the summary of a registered profile.                               <id/username>\n");
        printf(">>> ");
        if (l1[0] != '*') printf ("%s\n", l1);
        if (l2[0] != '*') printf ("%s\n>>> ", l2);

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");
        strncpy(l1, line, SIZE);

        if (strncmp(line, "q\0", 2) == 0) {
            flag = 0;
        } else {
            pls_wait();
            query_manager(lists, stats, hash, 1, 0, line);
            FILE *fp = fopen("./Resultados/command0_output.txt", "r+");
            char *str; size_t size = 0;
            if (getline(&str, &size, fp) == -1) {
                snprintf(l2 , SIZE,"%sInvalid Query%s", KRED, KWHT);
            } else {
                str = strsep(&str, "\n");
                snprintf(l2, SIZE, "%s%s%s", KYEL, str, KWHT);
            } 
            free(str);
            fclose(fp);
            remove("./Resultados/command0_output.txt");    
        }
         
    }
    free(line);
    return;
}

void interact_query_2 (LIST *lists, STATS *stats, HASH *hash) {
    char *line = NULL;
    int flag = 1;
    char l1[SIZE]; int s1 = 0;
    char l2[MAX_LINES][SIZE]; int s2 = 0;
    int invalid = 1;
    int page = 0;
    while (flag) {

        clear_terminal();
        printf("%s", KWHT);
        printf("%sQuery 2:                                                                               q - back\n%s", KGRN, KWHT);
        printf(  "Description: Lists the top N drivers with the highest average score.                        <N>\n");
        printf(">>> ");
        if (s1) printf ("%s\n", l1);
        if (s2) {
            if (invalid) {
                printf("%sInvalid Query\n", KRED);
            } else {
                printf("%s", KYEL);
                for (int i = 0; i < 15; i++)
                    if (l2[i][0] != '*')
                        printf ("%s\n", l2[i]);
                printf("%s< previous page - b                                                             n - next page >%s\n", KGRN, KWHT);
            }
            printf ("%s>>> ", KWHT);
        }
        s1 = 1; s2 = 1;

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");

        if (strncmp(line, "q\0", 2) == 0) {
            flag = 0;
        } else if (strncmp(line, "n\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else
                page = pagina(page+1, l2);
        } else if (strncmp(line, "b\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else 
                page = pagina(page-1, l2);
        } else if (is_positive_integer(line)) {
            invalid = 0;
            pls_wait();
            query_manager(lists, stats, hash, 2, 0, line);
            strncpy(l1, line, SIZE);
            page = 0;
            pagina(page, l2);
        } else {
            strncpy(l1, line, SIZE);
            invalid = 1;
        }
    }
    free(line);
    remove("./Resultados/command0_output.txt");   
    return;
}

void interact_query_3 (LIST *lists, STATS *stats, HASH *hash) {
    char *line = NULL;
    int flag = 1;
    char l1[SIZE]; int s1 = 0;
    char l2[MAX_LINES][SIZE]; int s2 = 0;
    int invalid = 1;
    int page = 0;
    while (flag) {

        clear_terminal();
        printf("%s", KWHT);
        printf("%sQuery 3:                                                                               q - back\n%s", KGRN, KWHT);
        printf(  "Description: Lists the top N users with the highest average score.                          <N>\n");
        printf(">>> ");
        if (s1) printf ("%s\n", l1);
        if (s2) {
            if (invalid) {
                printf("%sInvalid Query\n", KRED);
            } else {
                printf("%s", KYEL);
                for (int i = 0; i < 15; i++)
                    if (l2[i][0] != '*')
                        printf ("%s\n", l2[i]);
                printf("%s< previous page - b                                                             n - next page >%s\n", KGRN, KWHT);
            }
            printf ("%s>>> ", KWHT);
        }
        s1 = 1; s2 = 1;

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");

        if (strncmp(line, "q\0", 2) == 0) {
            flag = 0;
        } else if (strncmp(line, "n\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else
                page = pagina(page+1, l2);
        } else if (strncmp(line, "b\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else 
                page = pagina(page-1, l2);
        } else if (is_positive_integer(line)) {
            invalid = 0;
            pls_wait();
            query_manager(lists, stats, hash, 3, 0, line);
            strncpy(l1, line, SIZE);
            page = 0;
            pagina(page, l2);
        } else {
            strncpy(l1, line, SIZE);
            invalid = 1;
        }
    }
    free(line);
    remove("./Resultados/command0_output.txt");   
    return;
}

void interact_query_4 (LIST *lists, STATS *stats, HASH *hash) {

    char *line = NULL;
    int flag = 1;
    char l1[SIZE]; l1[0] = '*';
    char l2[SIZE]; l2[0] = '*';
    while (flag) {

        clear_terminal();
        printf("%s", KWHT);
        printf("%sQuery 4:                                                                               q - back\n%s", KGRN, KWHT);
        printf(  "Description: Returns the average price in a given city.                                  <City>\n");
        printf(">>> ");
        if (l1[0] != '*') printf ("%s\n", l1);
        if (l2[0] != '*') printf ("%s\n>>> ", l2);

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");
        strncpy(l1, line, SIZE);

        if (strncmp(line, "q\0", 2) == 0) {
            flag = 0;
        } else {
            pls_wait();
            query_manager(lists, stats, hash, 4, 0, line);
            FILE *fp = fopen("./Resultados/command0_output.txt", "r+");
            char *str; size_t size = 0;
            if (getline(&str, &size, fp) == -1) {
                snprintf(l2 , SIZE,"%sInvalid Query%s", KRED, KWHT);
            } else {
                str = strsep(&str, "\n");
                snprintf(l2, SIZE, "%s%s%s", KYEL, str, KWHT);
            } 
            free(str);
            fclose(fp);
            remove("./Resultados/command0_output.txt");    
        }
         
    }
    free(line);
    return;
}

int is_query_5_input_valid (char *line) {
    char *str1 = strdup(line);

    char *str2 = str1;
    strsep(&str2, " ");
    if (!str2) return 0;

    int a1 = validate_date(str1);
    int a2 = validate_date(str2);

    free(str1);
    return (a1 && a2);
}

void interact_query_5 (LIST *lists, STATS *stats, HASH *hash) {

    char *line = NULL;
    int flag = 1;
    char l1[SIZE]; l1[0] = '*';
    char l2[SIZE]; l2[0] = '*';
    while (flag) {

        clear_terminal();
        printf("%s", KWHT);
        printf("%sQuery 5:                                                                               q - back\n%s", KGRN, KWHT);
        printf(  "Description: Returns the average price between two dates.                     <date_A> <date_B>\n");
        printf(">>> ");
        if (l1[0] != '*') printf ("%s\n", l1);
        if (l2[0] != '*') printf ("%s\n>>> ", l2);

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");
        strncpy(l1, line, SIZE);

        if (strncmp(line, "q\0", 2) == 0) {
            flag = 0;
        } else if (is_query_5_input_valid(line)) {
            pls_wait();
            query_manager(lists, stats, hash, 5, 0, line);
            FILE *fp = fopen("./Resultados/command0_output.txt", "r+");
            char *str; size_t size = 0;
            if (getline(&str, &size, fp) == -1) {
                snprintf(l2 , SIZE,"%sInvalid Query%s", KRED, KWHT);
            } else {
                str = strsep(&str, "\n");
                snprintf(l2, SIZE, "%s%s%s", KYEL, str, KWHT);
            } 
            free(str);
            fclose(fp);
            remove("./Resultados/command0_output.txt");
        } else {
            snprintf(l2 , SIZE,"%sInvalid Query%s", KRED, KWHT);
        }
         
    }
    free(line);
    return;
}

int is_query_6_input_valid (char *line) {
    char *str = strdup(line);
        
    char *str1 = str;
    strsep(&str1, " ");
    if (!str1) return 0;

    char *str2 = str1;
    strsep(&str2, " ");
    if (!str2) return 0;

    int a1 = validate_date(str1);
    int a2 = validate_date(str2);

    free(str);
    return (a1 && a2);
}

void interact_query_6 (LIST *lists, STATS *stats, HASH *hash) {

    char *line = NULL;
    int flag = 1;
    char l1[SIZE]; l1[0] = '*';
    char l2[SIZE]; l2[0] = '*';
    while (flag) {

        clear_terminal();
        printf("%s", KWHT);
        printf("%sQuery 6:                                                                               q - back\n%s", KGRN, KWHT);
        printf(  "Description: Average distance traveled in a city between two dates.    <City> <date_A> <date_B>\n");
        printf(">>> ");
        if (l1[0] != '*') printf ("%s\n", l1);
        if (l2[0] != '*') printf ("%s\n>>> ", l2);

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");
        strncpy(l1, line, SIZE);

        if (strncmp(line, "q\0", 2) == 0) {
            flag = 0;
        } else if (is_query_6_input_valid(line)) {
            pls_wait();
            query_manager(lists, stats, hash, 6, 0, line);
            FILE *fp = fopen("./Resultados/command0_output.txt", "r+");
            char *str; size_t size = 0;
            if (getline(&str, &size, fp) == -1) {
                snprintf(l2 , SIZE,"%sInvalid Query%s", KRED, KWHT);
            } else {
                str = strsep(&str, "\n");
                snprintf(l2, SIZE, "%s%s%s", KYEL, str, KWHT);
            } 
            free(str);
            fclose(fp);
            remove("./Resultados/command0_output.txt");
        } else {
            snprintf(l2 , SIZE,"%sInvalid Query%s", KRED, KWHT);
        }
         
    }
    free(line);
    return;
}

int is_query_7_input_valid (char *line) {
    int a1 = is_positive_integer(line);
    return (a1);
}

void interact_query_7 (LIST *lists, STATS *stats, HASH *hash) {
    char *line = NULL;
    int flag = 1;
    char l1[SIZE]; int s1 = 0;
    char l2[MAX_LINES][SIZE]; int s2 = 0;
    int invalid = 1;
    int page = 0;
    while (flag) {

        clear_terminal();
        printf("%s", KWHT);
        printf("%sQuery 7:                                                                           q - back\n%s", KGRN, KWHT);
        printf(  "Description: Lists the top N drivers in a city by average score.                 <N> <City>\n");
        printf(">>> ");
        if (s1) printf ("%s\n", l1);
        if (s2) {
            if (invalid) {
                printf("%sInvalid Query\n", KRED);
            } else {
                printf("%s", KYEL);
                for (int i = 0; i < 15; i++)
                    if (l2[i][0] != '*')
                        printf ("%s\n", l2[i]);
                printf("%s< previous page - b                                                             n - next page >%s\n", KGRN, KWHT);
            }
            printf ("%s>>> ", KWHT);
        }
        s1 = 1; s2 = 1;

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");

        if (strncmp(line, "q\0", 2) == 0) {
            flag = 0;
        } else if (strncmp(line, "n\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else
                page = pagina(page+1, l2);
        } else if (strncmp(line, "b\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else 
                page = pagina(page-1, l2);
        } else if (is_query_7_input_valid(line)) {
            invalid = 0;
            pls_wait();
            strncpy(l1, line, SIZE);
            query_manager(lists, stats, hash, 7, 0, line);
            page = 0;
            pagina(page, l2);
        } else {
            strncpy(l1, line, SIZE);
            invalid = 1;
        }
    }
    free(line);
    remove("./Resultados/command0_output.txt");   
    return;
}

int is_query_8_input_valid (char *line) {
    int a1 = (((line[0] == 'M') || (line[0] == 'F')) && (line[1] == ' '));
    int a2 = is_positive_integer(line+2);
    return (a1 && a2);
}

void interact_query_8 (LIST *lists, STATS *stats, HASH *hash) {
    char *line = NULL;
    int flag = 1;
    char l1[SIZE]; int s1 = 0;
    char l2[MAX_LINES][SIZE]; int s2 = 0;
    int invalid = 1;
    int page = 0;
    while (flag) {

        clear_terminal();
        printf("%s", KWHT);
        printf("%sQuery 8:                                                                           q - back\n%s", KGRN, KWHT);
        printf(  "Description: Lists the rides in which the occupants are of the specified       <gender> <X>\n");
        printf(  "              gender, and whose profiles are at least X years old.\n");
        printf(">>> ");
        if (s1) printf ("%s\n", l1);
        if (s2) {
            if (invalid) {
                printf("%sInvalid Query\n", KRED);
            } else {
                printf("%s", KYEL);
                for (int i = 0; i < 15; i++)
                    if (l2[i][0] != '*')
                        printf ("%s\n", l2[i]);
                printf("%s< previous page - b                                                             n - next page >%s\n", KGRN, KWHT);
            }
            printf ("%s>>> ", KWHT);
        }
        s1 = 1; s2 = 1;

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");

        if (strncmp(line, "q\0", 2) == 0) {
            flag = 0;
        } else if (strncmp(line, "n\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else
                page = pagina(page+1, l2);
        } else if (strncmp(line, "b\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else 
                page = pagina(page-1, l2);
        } else if (is_query_8_input_valid(line)) {
            invalid = 0;
            pls_wait();
            strncpy(l1, line, SIZE);
            query_manager(lists, stats, hash, 8, 0, line);
            page = 0;
            if (n_lines() != 0)
                pagina(page, l2);
            else 
                invalid = 1;
        } else {
            strncpy(l1, line, SIZE);
            invalid = 1;
        }
    }
    free(line);
    remove("./Resultados/command0_output.txt");   
    return;
}

int is_query_9_input_valid (char *line) {
    int a1 = validate_date(line);
    int a2 = validate_date(line+12);
    return (a1 && a2);
}

void interact_query_9 (LIST *lists, STATS *stats, HASH *hash) {
    char *line = NULL;
    int flag = 1;
    char l1[SIZE]; int s1 = 0;
    char l2[MAX_LINES][SIZE]; int s2 = 0;
    int invalid = 1;
    int page = 0;
    while (flag) {

        clear_terminal();
        printf("%s", KWHT);
        printf("%sQuery 9:                                                                           q - back\n%s", KGRN, KWHT);
        printf(  "Description: Lists the rides where tips occured, between two dates,       <date_A> <date_B>\n");
        printf(  "              sorted by highest travel distance.\n");
        printf(">>> ");
        if (s1) printf ("%s\n", l1);
        if (s2) {
            if (invalid) {
                printf("%sInvalid Query\n", KRED);
            } else {
                printf("%s", KYEL);
                for (int i = 0; i < 15; i++)
                    if (l2[i][0] != '*')
                        printf ("%s\n", l2[i]);
                printf("%s< previous page - b                                                             n - next page >%s\n", KGRN, KWHT);
            }
            printf ("%s>>> ", KWHT);
        }
        s1 = 1; s2 = 1;

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");

        if (strncmp(line, "q\0", 2) == 0) {
            flag = 0;
        } else if (strncmp(line, "n\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else
                page = pagina(page+1, l2);
        } else if (strncmp(line, "b\0", 2) == 0) {
            if (invalid) 
                strncpy(l1, line, SIZE);
            else 
                page = pagina(page-1, l2);
        } else if (is_query_9_input_valid(line)) {
            invalid = 0;
            pls_wait();
            strncpy(l1, line, SIZE);
            query_manager(lists, stats, hash, 9, 0, line);
            page = 0;
            pagina(page, l2);
        } else {
            strncpy(l1, line, SIZE);
            invalid = 1;
        }
    }
    free(line);
    remove("./Resultados/command0_output.txt");   
    return;
}

int menu (LIST *lists, STATS *stats, HASH *hash) {
    char *line;
    int flag = 1;
    while (flag) {
        clear_terminal();
        printf("%s", KWHT);
        printf("Choose a query to execute:\n\n");
        printf("\tquery 1\n");
        printf("\tquery 2\n");
        printf("\tquery 3\n");
        printf("\tquery 4\n");
        printf("\tquery 5\n");
        printf("\tquery 6\n");
        printf("\tquery 7\n");
        printf("\tquery 8\n");
        printf("\tquery 9\n");
        printf("\n");
        printf("%sq - leave%s\n", KGRN, KWHT);

        line = malloc(sizeof(char) * SIZE);
        fgets(line, SIZE, stdin);
        line = strsep(&line, "\n");

        int select = line[0];
        switch (select) {
            case '1':
                interact_query_1(lists, stats, hash);
                break;
            case '2':
                interact_query_2(lists, stats, hash);
                break;
            case '3':
                interact_query_3(lists, stats, hash);
                break;
            case '4':
                interact_query_4(lists, stats, hash);
                break;
            case '5':
                interact_query_5(lists, stats, hash);
                break;
            case '6':
                interact_query_6(lists, stats, hash);
                break;
            case '7':
                interact_query_7(lists, stats, hash);
                break;
            case '8':
                interact_query_8(lists, stats, hash);
                break;
            case '9':
                interact_query_9(lists, stats, hash);
                break;
            case 'q':
                flag = 0;
            default:
                break;
        } 

    }
    
    free(line);
    return 1;
}

char *get_path_menu () {
    clear_terminal();
    int flag = 1;
    char *path = malloc(sizeof(char) * SIZE);
    while (flag == 1) {
        printf("\n");
        printf("%sPlease insert the path to the files:\n>>> ", KWHT);
        fgets(path, SIZE, stdin);
        path = strsep(&path, "\n");

        char *test = concat(path, "drivers.csv");
        FILE *fp = fopen(test, "r");

        if (fp == NULL) {
            printf("%sInvalid file path.", KRED);
        } else {
            fclose(fp);
            flag = 0;
        }
        free(test);
    }
    return path;
}

void interactive () {
    char *path_csv = get_path_menu();
    pls_wait();  

    HASH *hash = create_hash();
    STATS *stats = create_stats();

    parser(path_csv, hash);
    free(path_csv);
    
    calc_stats(hash, stats);
    LIST *lists = create_lists(stats, hash);
    
    menu(lists, stats, hash);

    destroy_hash(hash);
    destroy_stats(stats);
    destroy_lists(lists);

    clear_terminal();
    printf("%s", KNRM);
}
