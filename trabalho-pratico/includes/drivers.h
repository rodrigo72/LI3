#ifndef DRIVERS_H
#define DRIVERS_H

#include "./structs.h"

void parser_drivers (char *path, HASH *hash);

DRIVER *create_driver 
(char *id,
 char *name,
 int birth_date,
 char *gender,
 char  car_class,
 char *license_plate,
 char *city,
 int account_creation,
 char  account_status
);

void destroy_driver (void *data);

char *get_driver_id               (const DRIVER *driver);
char *get_driver_name             (const DRIVER *driver);
int   get_driver_birth_date       (const DRIVER *driver);
char  get_driver_gender           (const DRIVER *driver);
char  get_driver_car_class        (const DRIVER *driver);
char *get_driver_license_plate    (const DRIVER *driver);
char *get_driver_city             (const DRIVER *driver);
int   get_driver_account_creation (const DRIVER *driver);
char  get_driver_account_status   (const DRIVER *driver);

void print_driver (DRIVER *driver);

#endif //DRIVERS_H
