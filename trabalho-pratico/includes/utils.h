#ifndef UTILS_H
#define UTILS_H

char *concat(char *str1, char *str2);

int date_comp   (char *a, char*b);
int date_comp_2 (char *a, char *b);

int username_comp (char *a, char *b);
int id_comp       (char *a, char*b);
int date_age      (char *date);

int  validate_date (char *date);
void to_lowercase  (char *str);

int is_positive_integer (const char *str);
int is_number           (const char *str);

char *int_to_date   (int num);
int   date_to_int   (char *date);
int   int_to_age    (int num);
int   next_date_int (int num);

int clamp(int x, int min, int max);

#endif // UTILS_H
