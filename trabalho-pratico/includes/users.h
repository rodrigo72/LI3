#ifndef USERS_H
#define USERS_H

#include "./structs.h"

void parser_users (char *path, HASH *hash);

USER *create_user   
(char *username,
 char *name,
 char *gender,
 int birth_date,
 int account_creation,
 char  pay_method,
 char  account_status
);

void destroy_user(void* data);

char *get_user_username           (const USER *user);
char *get_user_name               (const USER *user);
char  get_user_gender             (const USER *user);
int   get_user_birth_date         (const USER *user);
int   get_user_account_creation   (const USER *user);
char  get_user_pay_method         (const USER *user);
char  get_user_account_status     (const USER *user);

void print_user   (USER   *user);

#endif //USERS_H
