#ifndef RIDES_H
#define RIDES_H

#include "./structs.h"

void parser_rides (char *path, HASH *hash);

RIDE *create_ride
(char *id,
 int date,
 char *driver,
 char *user,
 char *city,
 char *distance,
 char *score_user,
 char *score_driver,
 char *tip
);

void destroy_ride(void* data);

int            get_ride_id           (const RIDE *ride);
int            get_ride_date         (const RIDE *ride);
char          *get_ride_driver       (const RIDE *ride);
char          *get_ride_user         (const RIDE *ride);
char          *get_ride_city         (const RIDE *ride);
unsigned short get_ride_distance     (const RIDE *ride);
unsigned short get_ride_score_user   (const RIDE *ride);
int            get_ride_score_driver (const RIDE *ride);
double         get_ride_tip          (const RIDE *ride);
char          *get_ride_comment      (const RIDE *ride);

void print_ride   (RIDE   *ride);

#endif //RIDES_H
