#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <glib.h>

#include "../includes/utils.h"
#include "../includes/structs.h"

typedef struct rd {
    int id;
    int date;
    char *driver;
    char *user;
    char *city;
    unsigned short distance;
    unsigned short score_user;
    unsigned short score_driver;
    double tip;
} RIDE;

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
)
{
    RIDE *ride = malloc(sizeof(RIDE));
    ride->id           = atoi(id);
    ride->date         = date;
    ride->driver       = strdup(driver);
    ride->user         = strdup(user);
    ride->city         = strdup(city);
    ride->distance     = atoi(distance);
    ride->score_user   = atoi(score_user);
    ride->score_driver = atoi(score_driver);
    ride->tip          = atof(tip);
    return ride;
}

void destroy_ride (void *data) {
    RIDE *ride = data;
    free(ride->driver);
    free(ride->user  );
    free(ride->city  );
    free(ride);
}

int            get_ride_id           (const RIDE *ride) {return        ride->id           ;}
int            get_ride_date         (const RIDE *ride) {return        ride->date         ;}
char          *get_ride_driver       (const RIDE *ride) {return strdup(ride->driver      );}
char          *get_ride_user         (const RIDE *ride) {return strdup(ride->user        );}
char          *get_ride_city         (const RIDE *ride) {return strdup(ride->city        );}
unsigned short get_ride_distance     (const RIDE *ride) {return        ride->distance     ;}
unsigned short get_ride_score_user   (const RIDE *ride) {return        ride->score_user   ;}
int            get_ride_score_driver (const RIDE *ride) {return        ride->score_driver ;}
double         get_ride_tip          (const RIDE *ride) {return        ride->tip          ;}


void set_ride_id           (RIDE *ride, int    id          ) {                    ride->id           = id;           }
void set_ride_date         (RIDE *ride, int    date        ) {                    ride->date         = date;         }
void set_ride_driver       (RIDE *ride, char  *driver      ) {free(ride->driver); ride->driver       = driver;       }
void set_ride_user         (RIDE *ride, char  *user        ) {free(ride->user);   ride->user         = user;         }
void set_ride_city         (RIDE *ride, char  *city        ) {free(ride->city);   ride->city         = city;         }
void set_ride_distance     (RIDE *ride, int    distance    ) {                    ride->distance     = distance;     }
void set_ride_score_user   (RIDE *ride, int    score_user  ) {                    ride->score_user   = score_user;   }
void set_ride_score_driver (RIDE *ride, int    score_driver) {                    ride->score_driver = score_driver; }
void set_ride_tip          (RIDE *ride, double tip         ) {                    ride->tip          = tip;          }

void print_ride(RIDE *ride) {
    printf("%d;",   ride->id);
    printf("%hu;",  ride->date);
    printf("%s;",   ride->driver);
    printf("%s;",   ride->user);
    printf("%s;",   ride->city);
    printf("%hu;",  ride->distance);
    printf("%hu;",  ride->score_user);
    printf("%hu;",  ride->score_driver);
    printf("%.3f;", ride->tip);
}

void parser_rides (char *path, HASH *hash) {
    FILE *fp = fopen(path, "r");
    if (!fp) assert(0);

    GPtrArray *rides_hash = get_rides_hash(hash);
    
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, fp); // remover a primeira linha
    while (getline(&line, &len, fp) != -1) {
        char *aux = line;
        char *id           = strsep(&aux, ";\n");
        char *date         = strsep(&aux, ";\n");
        char *driver       = strsep(&aux, ";\n");
        char *user         = strsep(&aux, ";\n");
        char *city         = strsep(&aux, ";\n");
        char *distance     = strsep(&aux, ";\n");
        char *score_user   = strsep(&aux, ";\n");
        char *score_driver = strsep(&aux, ";\n");
        char *tip          = strsep(&aux, ";\n");

        if (validate_date(date) 
        && is_positive_integer(distance) && is_positive_integer(score_driver) && is_positive_integer(score_user) && is_number(tip)
        && strlen(id) && strlen(driver)  && strlen(user) && strlen(city)) {
            RIDE *ride = create_ride(id,
                                     date_to_int(date),
                                     driver,
                                     user,
                                     city,
                                     distance,
                                     score_user,
                                     score_driver,
                                     tip);
            g_ptr_array_add(rides_hash, ride);
        }
    }

    free(line);
    fclose(fp);
}
