#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/stats.h"
#include "../includes/utils.h"

#include "../includes/drivers.h"
#include "../includes/users.h"
#include "../includes/rides.h"

typedef struct {
    HASH *hash;
    STATS *stats;    
} AUX_STRUCT_Q8;

static gint compare_rides_q8_2 (gconstpointer a, gconstpointer b, gpointer user_data) {
    
    const RIDE *ride_a = *((RIDE **) a);
    const RIDE *ride_b = *((RIDE **) b);

    const AUX_STRUCT_Q8 *aux = user_data;
    HASH *hash = aux->hash;

    char *driver_id_a = get_ride_driver(ride_a);
    char *driver_id_b = get_ride_driver(ride_b);


    DRIVER *driver_a = g_hash_table_lookup(get_drivers_hash(hash), driver_id_a);
    DRIVER *driver_b = g_hash_table_lookup(get_drivers_hash(hash), driver_id_b);

    free(driver_id_a);
    free(driver_id_b);

    int driver_account_creation_a = get_driver_account_creation(driver_a);
    int driver_account_creation_b = get_driver_account_creation(driver_b);

    if (driver_account_creation_a > driver_account_creation_b) return 1;
    if (driver_account_creation_a < driver_account_creation_b) return -1;
    
    char *user_username_a = get_ride_user(ride_a);
    char *user_username_b = get_ride_user(ride_b);

    USER *user_a = g_hash_table_lookup(get_users_hash(hash), user_username_a);
    USER *user_b = g_hash_table_lookup(get_users_hash(hash), user_username_b);

    free(user_username_a);
    free(user_username_b);

    int user_account_creation_a = get_user_account_creation(user_a);
    int user_account_creation_b = get_user_account_creation(user_b);

    if (user_account_creation_a > user_account_creation_b) return 1;
    if (user_account_creation_a < user_account_creation_b) return -1;

    int ride_id_a = get_ride_id(ride_a);
    int ride_id_b = get_ride_id(ride_b);

    return ride_id_a - ride_id_b;
}

void query_8 (STATS *stats, HASH *hash, char *path, int i, char *line) {
    
    char *file = malloc(sizeof(char) * 25);
    sprintf(file, "command%d_output.txt", i);
    char *filepath = concat(path, file);
    free(file);

    FILE *fp = fopen(filepath, "w+");
    free(filepath);

    char *gender_str, *age_str;
    gender_str = strsep(&line, " \n");
    age_str    = strsep(&line, " \n");

    char gender = gender_str[0];
    int N = atoi(age_str);

    // GPtrArray *arr1;
    // if (gender == 'F') {
    //     arr1 = get_f_rides_arr(stats);
    // } else {
    //     arr1 = get_m_rides_arr(stats);
    // }

    // for (int i = 0; i < arr1->len; i++) {
    //     RIDE *ride = g_ptr_array_index(arr1, i);
    //     char *driver_id = get_ride_driver(ride);

    //     DRIVER *driver = g_hash_table_lookup(get_drivers_hash(hash), driver_id);
    //     int age_driver = int_to_age(get_driver_account_creation(driver));

    //     if (age_driver < N) {
    //         free(driver_id);
    //         break;
    //     }

    //     char *driver_name = get_driver_name(driver);
    //     char *user_username = get_ride_user(ride);

    //     USER *user = g_hash_table_lookup(get_users_hash(hash), user_username);
    //     char *user_name = get_user_name(user);

    //     fprintf(fp, "%s;%s;%s;%s\n", driver_id, driver_name, user_username, user_name);

    //     free(driver_id);
    //     free(user_name);
    //     free(user_username);
    //     free(driver_name);
    // }
    
    AUX_STRUCT_Q8 aux;
    aux.hash = hash;
    aux.stats = stats;

    GPtrArray **main_arr;
    if (gender == 'F') main_arr = get_f_account_status_ht(stats);
    else               main_arr = get_m_account_status_ht(stats);

    GPtrArray *merge_all = g_ptr_array_new();

    for (int i = 20; i >= N; i--) {
        GPtrArray *arr = main_arr[i];
        if (arr != NULL) {
            for (guint j = 0; j < arr->len; j++) {
                g_ptr_array_add(merge_all, g_ptr_array_index(arr, j));
            }
        }
    }

    g_ptr_array_sort_with_data(merge_all, compare_rides_q8_2, &aux);

    for (guint j = 0; j < merge_all->len; j++) {

        RIDE *ride = g_ptr_array_index(merge_all, j);
        char *driver_id = get_ride_driver(ride);

        DRIVER *driver = g_hash_table_lookup(get_drivers_hash(hash), driver_id);
        int age_driver = int_to_age(get_driver_account_creation(driver));

        if (age_driver < N) {
            free(driver_id);
            break;
        }

        char *driver_name = get_driver_name(driver);
        char *user_username = get_ride_user(ride);

        USER *user = g_hash_table_lookup(get_users_hash(hash), user_username);
        char *user_name = get_user_name(user);

        fprintf(fp, "%s;%s;%s;%s\n", driver_id, driver_name, user_username, user_name);

        free(driver_id);
        free(user_name);
        free(user_username);
        free(driver_name);
    }

    g_ptr_array_free(merge_all, 1);

    // int flag = 1;
    // for (int i = 20; i >= N && flag; i--) {
    //     GPtrArray *arr = main_arr[i];
    //     if (arr == NULL) continue;

    //     g_ptr_array_sort_with_data(arr, compare_rides_q8_2, &aux);
    //     // g_ptr_array_sort_with_data(arr, compare_rides_q8_3, &aux);

    //     for (guint j = 0; j < arr->len; j++) {
    //         RIDE *ride = g_ptr_array_index(arr, j);
    //         char *driver_id = get_ride_driver(ride);

    //         DRIVER *driver = g_hash_table_lookup(get_drivers_hash(hash), driver_id);
    //         int age_driver = int_to_age(get_driver_account_creation(driver));

    //         if (age_driver < N) {
    //             free(driver_id);
    //             flag = 0;
    //             break;
    //         }

    //         char *driver_name = get_driver_name(driver);
    //         char *user_username = get_ride_user(ride);
    //         int driver_account_age = get_driver_account_creation(driver);

    //         USER *user = g_hash_table_lookup(get_users_hash(hash), user_username);
    //         char *user_name = get_user_name(user);
    //         int user_account_age = get_user_account_creation(user);

    //         fprintf(fp, "%s;%s;%s;%s;  %d;%d\n", driver_id, driver_name, user_username, user_name, driver_account_age, user_account_age);

    //         free(driver_id);
    //         free(user_name);
    //         free(user_username);
    //         free(driver_name);
    //     }
    // }
    
    fclose(fp);    
}