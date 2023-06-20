#include <stdlib.h>

#include "../includes/utils.h"
#include "../includes/structs.h"

#include "../includes/users.h"
#include "../includes/drivers.h"
#include "../includes/rides.h"

void parser (char *path, HASH *hash) {
    
    char *path_users = concat(path, "/users.csv");
    parser_users(path_users, hash);
    free(path_users);

    char *path_drivers = concat(path, "/drivers.csv");
    parser_drivers(path_drivers, hash);
    free(path_drivers);

    char *path_rides = concat(path, "/rides.csv");
    parser_rides(path_rides, hash);
    free(path_rides);
}
