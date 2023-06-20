#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DAY 9
#define MONTH 10
#define YEAR 2022

// recebe 2 strings e devolve uma nova string composta pelas duas strings dadas, concatenadas
char *concat(char *str1, char *str2) {
  	int size1 = strlen(str1);
  	int size2 = strlen(str2);
	
  	char *str = malloc(size1 + size2 + 1);
	
  	memcpy(str, str1, size1);
  	memcpy(str+size1, str2, size2);
	
  	str [size1 + size2] = '\0'; 
	
  	return str;
}

int date_to_int (char *a) {
    int d, m, y;
    sscanf (a, "%02d/%02d/%04d", &d, &m, &y);
    return (y*10000) + (m*100) + (d*1);
}

// valor positivo se a > b
// valor negativo se a < b
// zero se a == b
int date_comp (char *a, char*b) {
    int na = date_to_int (a);
    int nb = date_to_int (b);
    return (na-nb);
}

int date_comp_2(char *a, char *b) {
    if (a[6] != b[6]) return a[6] - b[6];
    if (a[7] != b[7]) return a[7] - b[7];
    if (a[8] != b[8]) return a[8] - b[8];
    if (a[9] != b[9]) return a[9] - b[9];
    if (a[3] != b[3]) return a[3] - b[3];
    if (a[4] != b[4]) return a[4] - b[4];
    if (a[0] != b[0]) return a[0] - b[0];
    return a[1] - b[1];
}

// valor positivo se a > b
// valor negativo se a < b
// zero se a == b
int id_comp (char *a, char*b) {
    return strcmp(a, b);
}

int username_comp (char *a, char *b) {
    return strcmp(a, b);
}

int is_leap_year (int year) {
    if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0))) return 1;
    else return 0;
}

int isLeapYear(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

int date_age (char *date) {
    
    int d, m, y;
    sscanf (date, "%02d/%02d/%04d", &d, &m, &y);
    
    int age = 0;
    
    y = YEAR  - y  - 1;
    m = MONTH - m;
    d = DAY   - d;
    
    age += y + ((m > 0) || ((m == 0 ) && (d >= 0)));
    
    return age;
}

void to_lowercase (char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
}

int validate_date(char *date) {
    int day, month, year;
    if (sscanf(date, "%02d/%02d/%04d", &day, &month, &year) != 3) return 0;

    if (day < 1 || day > 31)     return 0;
    if (month < 1 || month > 12) return 0;
    if (year < 1 || year > 2022) return 0;
    if (year == 2022 && (month < 10 || (month == 10 && day > 9))) return 0;
    
    return 1;
}

int is_positive_integer(const char *str) {

    if (str == NULL || str[0] == '\0') return 0;
    
    for (int i = 0; str[i] != '\0' && str[i] != '\0'; i++)
      if (!(str[i] >= '0' && str[i] <= '9'))
        return 0;

    int n = atoi(str);
    return n > 0;
}

int is_number(const char *str) {

    if (str == NULL || str[0] == '\0') return 0;
    
    int flag = 0;
    for (int i = 0; str[i] != '\0'; i++)
      if (str[i] == '.')
          if (flag) return 0;
          else flag = 1;
      else if (!(str[i] >= '0' && str[i] <= '9'))
          return 0;

    double n = atof(str);
    return n >= 0;
}

int next_date_int (int num) {
    
    int d, m, y;
    y =  num / 10000;
    m = (num % 10000) / 100;
    d =  num % 100;

    int arr[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                     {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};


    if (d < arr[is_leap_year(y)][m-1]) {
        d++;
    } else {
        d = 1;
        if (m < 12) m++;
        else m = 1, y++;
    }

    return y * 10000 + m * 100 + d;
}

int int_to_age2 (int num) {
    
    int d, m, y;
    y =  num / 10000;
    m = (num % 10000) / 100;
    d =  num % 100;

    int arr[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                   {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

    int count_days = 0;
    int leap = is_leap_year(y);

    while (!(y == YEAR && m == MONTH && d == DAY)) {

        if (d < arr[leap][m-1]) {
            d++;
        } else if (m < 12) {
            d = 1;
            m++;
        } else {
            d = m = 1;
            leap = is_leap_year(++y);
        }

        count_days++;
    }

    int age = 0;
    y = YEAR;

    while (count_days >= 365) {
        count_days -= is_leap_year(y-age) ? 366 : 365, age++;
    }

    return age;
}

int int_to_age (int num) {
    int d, m, y;
    y =  num / 10000;
    m = (num % 10000) / 100;
    d =  num % 100;

    int age = YEAR - y;
    if (MONTH - m < 0) age--;
    else if ((MONTH - m == 0) && (DAY - d < 0)) age--;

    return age;
}

char *int_to_date (int num) {
    unsigned short d, m, y;
    y =  num / 10000;
    m = (num % 10000) / 100;
    d =  num % 100;

    char *date = malloc(sizeof(char) * 11);
    sprintf(date, "%02hu/%02hu/%04hu", d, m, y);

    return date;
}

int clamp(int x, int min, int max) {
     if (x < min) 
        return min;
    if (x > max) 
        return max;
    return x;
}

// int days_to_age (int count_days) {

//     int age = 0;
//     int year = YEAR;

//     while (count_days >= 365) {
//         count_days -= is_leap_year(year-age) ? 366 : 365;
//         age++;
//     }
    
//     return age;
// }

// int date_to_days (char *date) {

//     int day, month, year;
//     if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) return -1;

//     int days[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
//                        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
    
//     int count_days = 0;
//     int leap = is_leap_year(year);

//     if (year > YEAR || (year == YEAR && month > MONTH) || (year == YEAR && month == MONTH && day > DAY)) {
//         while (!(year == YEAR && month == MONTH && day == DAY)) {

//             if (day > 1) {
//                 day--;
//             } else if (month > 1) {
//                 month--;
//                 day = days[leap][month-1];
//             } else {
//                 day = 31;
//                 month = 12;
//                 leap = is_leap_year(--year);
//             }

//             count_days--;
//         }
//     } else {
//         while (!(year == YEAR && month == MONTH && day == DAY)) {

//             if (day < days[leap][month-1]) {
//                 day++;
//             } else if (month < 12) {
//                 day = 1;
//                 month++;
//             } else {
//                 day = month = 1;
//                 leap = is_leap_year(++year);
//             }

//             count_days++;
//         }
//     }

//     return count_days;
// }

// char *days_to_date (int count_days) {

//     unsigned short day = DAY;
//     unsigned short month = MONTH;
//     unsigned short year = YEAR;

//     int leap = is_leap_year(year);

//     int days[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
//                        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

//     while (count_days > 0) {
//         if (day > 1) {
//             day--;
//         } else if (month > 1) {
//             month--;
//             day = days[leap][month-1];
//         } else {
//             day = 31;
//             month = 12;
//             year--;
//             leap = is_leap_year(year);
//         }

//         count_days--;
//     }

//     char *str = malloc(sizeof(char) * 11);
//     sprintf(str, "%02hu/%02hu/%hu", day, month, year);

//     return str;
// }