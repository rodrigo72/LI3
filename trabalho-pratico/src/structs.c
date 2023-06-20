#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/utils.h"

#include "../includes/drivers.h"
#include "../includes/users.h"
#include "../includes/rides.h"


typedef struct ht {
    void *users;    // key: char *username   || value: USER*
    void *drivers;  // key: char *driver_id  || value: DRIVER*
    void *rides;    // key: char *ride_id    || value: RIDE*
} HASH;

HASH *create_hash () {
    HASH *hash = malloc(sizeof(HASH));
    
    hash->users   = g_hash_table_new_full(g_str_hash, g_str_equal, free, destroy_user      );
    hash->drivers = g_hash_table_new_full(g_str_hash, g_str_equal, free, destroy_driver    );
    hash->rides   = g_ptr_array_new_with_free_func(destroy_ride);

    return hash;
}

void destroy_hash (HASH *hash) {
    g_hash_table_destroy(hash->users);
    g_hash_table_destroy(hash->drivers);
    g_ptr_array_free(hash->rides, 1);
    free(hash);
}

void *get_drivers_hash (HASH *hash) {return hash->drivers;}
void *get_users_hash   (HASH *hash) {return hash->users  ;}
void *get_rides_hash   (HASH *hash) {return hash->rides  ;}
