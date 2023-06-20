#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/utils.h"

typedef struct usr {
    char *username;
    char *name;
    int birth_date;
    int account_creation;
    char pay_method;
    char account_status;
    char gender;
} USER;

USER *create_user   
(char *username,
 char *name,
 char *gender,
 int   birth_date,
 int   account_creation,
 char  pay_method,
 char  account_status
)
{
    USER *user = malloc(sizeof(USER));
    user->username         = strdup(username);
    user->name             = strdup(name);
    user->gender           = *gender;
    user->birth_date       = birth_date;
    user->account_creation = account_creation;
    user->pay_method       = pay_method;
    user->account_status   = account_status; 

    return user;
}

void destroy_user (void *data) {
    USER *user = data;
    free(user->username        );
    free(user->name            );
    free(user);
}

char *get_user_username           (const USER *user) {return strdup(user->username         );}
char *get_user_name               (const USER *user) {return strdup(user->name             );}
char  get_user_gender             (const USER *user) {return        user->gender            ;}
int   get_user_birth_date         (const USER *user) {return        user->birth_date        ;}
int   get_user_account_creation   (const USER *user) {return        user->account_creation  ;}
char  get_user_pay_method         (const USER *user) {return        user->pay_method        ;}
char  get_user_account_status     (const USER *user) {return        user->account_status    ;}

void set_user_username            (USER *user, char *username      ) {free(user->username); user->username         = username;        }
void set_user_name                (USER *user, char *name          ) {free(user->name);     user->name             = name;            }
void set_user_gender              (USER *user, char  gender        ) {                      user->gender           = gender;          }
void set_user_birth_date          (USER *user, int birth_date      ) {                      user->birth_date       = birth_date;      }
void set_user_account_creation    (USER *user, int account_creation) {                      user->account_creation = account_creation;}
void set_user_pay_method          (USER *user, char  pay_method    ) {                      user->pay_method       = pay_method;      }
void set_user_account_status      (USER *user, char  account_status) {                      user->account_status   = account_status;  }

void print_user  (USER *user) {
    printf("%s;",  user->username);
    printf("%s;",  user->name);
    printf("%c;",  user->gender);
    printf("%hu;", user->birth_date);
    printf("%hu;", user->account_creation);
    printf("%c;",  user->pay_method);
    printf("%c\n", user->account_status);
}

void parser_users (char *path, HASH *hash) {
    FILE *fp = fopen(path, "r");
    if (!fp) assert(0);

    void *users_hash = get_users_hash(hash);

    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, fp); // remover a primeira linha
    while (getline(&line, &len, fp) != -1) {
        char *aux = line;
        char *username         = strsep(&aux, ";\n");
        char *name             = strsep(&aux, ";\n");
        char *gender           = strsep(&aux, ";\n");
        char *birth_date       = strsep(&aux, ";\n");
        char *account_creation = strsep(&aux, ";\n");
        char *pay_method       = strsep(&aux, ";\n");
        char *account_status   = strsep(&aux, ";\n");
        
        to_lowercase(account_status);

        if (validate_date(birth_date) && validate_date(account_creation) 
        && (!strcmp(account_status, "active") || !strcmp(account_status, "inactive"))
        && strlen(username) && strlen(name) && strlen(gender) && strlen(pay_method)) { 
            USER *user = create_user(username,
                                     name,
                                     gender,
                                     date_to_int(birth_date),
                                     date_to_int(account_creation),
                                     pay_method[0],
                                     account_status[0]);
            g_hash_table_insert(users_hash, get_user_username(user), user);
        }

    }
    
    free(line);
    fclose(fp);
}
