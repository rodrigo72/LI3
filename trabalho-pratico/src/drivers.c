#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <glib.h>

#include "../includes/utils.h"
#include "../includes/structs.h"

typedef struct drv {
    char *id;
    char *name;
    int birth_date;
    char car_class;
    char *license_plate;
    char *city;
    int account_creation;
    char account_status;
    char gender;
} DRIVER;

DRIVER *create_driver 
(char *id,
 char *name,
 int   birth_date,
 char *gender,
 char  car_class,
 char *license_plate,
 char *city,
 int   account_creation,
 char  account_status
)
{
    DRIVER *driver = malloc(sizeof(DRIVER));
    driver->id               = strdup(id);
    driver->name             = strdup(name);
    driver->birth_date       = birth_date;
    driver->gender           = *gender;
    driver->car_class        = car_class;
    driver->license_plate    = strdup(license_plate);
    driver->city             = strdup(city);
    driver->account_creation = account_creation;
    driver->account_status   = account_status;

    return driver;
}

void destroy_driver (void *data) {
    DRIVER *driver = data;
    free(driver->id              );
    free(driver->name            );
    free(driver->license_plate   );
    free(driver->city            );
    free(driver);
}

char *get_driver_id               (const DRIVER *driver) {return strdup(driver->id              );}
char *get_driver_name             (const DRIVER *driver) {return strdup(driver->name            );}
int   get_driver_birth_date       (const DRIVER *driver) {return        driver->birth_date       ;}
char  get_driver_gender           (const DRIVER *driver) {return        driver->gender           ;}
char  get_driver_car_class        (const DRIVER *driver) {return        driver->car_class        ;}
char *get_driver_license_plate    (const DRIVER *driver) {return strdup(driver->license_plate   );}
char *get_driver_city             (const DRIVER *driver) {return strdup(driver->city            );}
int   get_driver_account_creation (const DRIVER *driver) {return        driver->account_creation ;}
char  get_driver_account_status   (const DRIVER *driver) {return        driver->account_status   ;}

void set_driver_id               (DRIVER *driver, char *id            ) {free(driver->id);            driver->id               = id;               }
void set_driver_name             (DRIVER *driver, char *name          ) {free(driver->name);          driver->name             = name;             }
void set_driver_birth_date       (DRIVER *driver, int  birth_date     ) {                             driver->birth_date       = birth_date;       }
void set_driver_gender           (DRIVER *driver, char  gender        ) {                             driver->gender           = gender;           }
void set_driver_car_class        (DRIVER *driver, char  car_class     ) {                             driver->car_class        = car_class;        }
void set_driver_license_plate    (DRIVER *driver, char *license_plate ) {free(driver->license_plate); driver->license_plate    = license_plate;    }
void set_driver_city             (DRIVER *driver, char *city          ) {free(driver->city);          driver->city             = city;             }
void set_driver_account_creation (DRIVER *driver, int account_creation) {                             driver->account_creation = account_creation; }
void set_driver_account_status   (DRIVER *driver, char  account_status) {                             driver->account_status   = account_status;   }

void print_driver(DRIVER *driver) {
    printf("%s;",  driver->id);
    printf("%s;",  driver->name);
    printf("%hu;", driver->birth_date);
    printf("%c;",  driver->gender);
    printf("%c;",  driver->car_class);
    printf("%s;",  driver->license_plate);
    printf("%s;",  driver->city);
    printf("%hu;", driver->account_creation);
    printf("%c\n", driver->account_status);
}

void parser_drivers (char *path, HASH *hash) {
    FILE *fp = fopen(path, "r");
    if (!fp) assert(0);

    void *drivers_hash = get_drivers_hash(hash);

    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, fp); // remover a primeira linha
    while (getline(&line, &len, fp) != -1) {
        char *aux = line;
        char *id               = strsep(&aux, ";\n");
        char *name             = strsep(&aux, ";\n");
        char *birth_date       = strsep(&aux, ";\n");
        char *gender           = strsep(&aux, ";\n");
        char *car_class        = strsep(&aux, ";\n");
        char *license_plate    = strsep(&aux, ";\n");
        char *city             = strsep(&aux, ";\n");
        char *account_creation = strsep(&aux, ";\n");
        char *account_status   = strsep(&aux, ";\n");

        to_lowercase(car_class);
        to_lowercase(account_status);

        if (validate_date(birth_date) && validate_date(account_creation)
        && (!strcmp(car_class, "basic") || !strcmp(car_class, "premium") || !strcmp(car_class, "green"))
        && (!strcmp(account_status, "active") || !strcmp(account_status, "inactive"))
        && strlen(id) && strlen(name) && strlen(gender) && strlen(license_plate) && strlen(city)) {
            DRIVER *driver = create_driver(id,
                                           name,
                                           date_to_int(birth_date),
                                           gender,
                                           car_class[0],
                                           license_plate,
                                           city,
                                           date_to_int(account_creation),
                                           account_status[0]);
            g_hash_table_insert(drivers_hash, get_driver_id(driver), driver);
        }

    }

    free(line);
    fclose(fp);
}
