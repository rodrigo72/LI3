#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/stats.h"
#include "../includes/utils.h"
#include "../includes/lists.h"

typedef struct lst {
    GList *active_user_stats;
    GList *active_driver_stats;
    void *city_driver_stats_lists_ht;
} LIST;

// Positive value if a < b
// Negative value if a > b
// Zero if a = b
int comp_func_drivers (const void *d1, const void *d2) {
    // const DRIVER_STATS *driver_1 = *((DRIVER_STATS **) d1);
    // const DRIVER_STATS *driver_2 = *((DRIVER_STATS **) d2);

    const DRIVER_STATS *driver_1 = d1;
    const DRIVER_STATS *driver_2 = d2;

    char account_status_1 = get_driver_stats_account_status(driver_1);
    char account_status_2 = get_driver_stats_account_status(driver_2);

    if (account_status_1 != 'a' && account_status_2 == 'a') return 1;
    if (account_status_1 == 'a' && account_status_2 != 'a') return -1;
    if (account_status_1 != 'a' && account_status_2 != 'a') return 0;

    double av_d1 = get_driver_stats_avaliacao_media(driver_1);
    double av_d2 = get_driver_stats_avaliacao_media(driver_2);

    if (av_d1 < av_d2) return 1; 
    if (av_d1 > av_d2) return -1; 
    
    int date_1 = get_driver_stats_ultima_viagem(driver_1);
    int date_2 = get_driver_stats_ultima_viagem(driver_2);

    if (date_1 > date_2) return  1;
    if (date_1 < date_2) return -1;
    
    char *id_1 = get_driver_stats_id(driver_1);
    char *id_2 = get_driver_stats_id(driver_2);

    int id = id_comp(id_1, id_2);

    free(id_1);
    free(id_2);
    
    return id;
}

// The comparison function used to sort the GList. 
// This function is passed the data from 2 elements of the GList. 
// It should return 0 if they are equal.
// A negative value if the first element comes before the second.
// A positive value if the first element comes after the second.

int comp_func_users (const void *u1, const void *u2) {
    const USER_STATS *user_1 = u1;
    const USER_STATS *user_2 = u2;

    int account_status_1 = get_user_stats_account_status(user_1);
    int account_status_2 = get_user_stats_account_status(user_2);

    if (account_status_1 != 'a' && account_status_2 == 'a') return 1;
    if (account_status_1 == 'a' && account_status_2 != 'a') return -1;
    if (account_status_1 != 'a' && account_status_2 != 'a') return 0;

    int av_d1 = get_user_stats_distancia_viajada(user_1);
    int av_d2 = get_user_stats_distancia_viajada(user_2);

    if (av_d1 < av_d2) return 1;
    if (av_d1 > av_d2) return -1;
    
    int date_1 = get_user_stats_ultima_viagem(user_1);
    int date_2 = get_user_stats_ultima_viagem(user_2);

    if (date_1 > date_2) return 1;
    if (date_1 < date_2) return -1;
    
    char *usr_nm_1 = get_user_stats_username(user_1);
    char *usr_nm_2 = get_user_stats_username(user_2);

    int usr_nm = strcmp(usr_nm_1, usr_nm_2);

    free(usr_nm_1);
    free(usr_nm_2);

    return usr_nm;
}

GList *make_active_driver_stats_list(STATS *stats) {
    GList *list = g_hash_table_get_values(get_drivers_stats_hash(stats));
    list = g_list_sort(list, comp_func_drivers);
    return list;
}

GList *make_active_user_stats_list(STATS *stats) {
    GList *list = g_hash_table_get_values(get_users_stats_hash(stats));
    list = g_list_sort(list, comp_func_users);
    return list;
}

void destroy_city_driver_stats_ht (void *a) {
    GList *list = a;
    g_list_free(list);
}

void collect_values(void *key, void *value, void *user_data) {
    (void) key;
    GPtrArray *values = (GPtrArray *) user_data;
    g_ptr_array_add(values, value);
}

// GPtrArray *make_arr (STATS *stats) {
//     GPtrArray *values = g_ptr_array_new();
//     g_hash_table_foreach(get_drivers_stats_hash(stats), collect_values, values);
//     g_ptr_array_sort(values, comp_func_drivers);
//     return values;
// }

LIST *create_lists (STATS *stats, HASH *hash) {
    (void) hash;
    LIST *lists = malloc(sizeof(LIST));
    lists->active_driver_stats        = make_active_driver_stats_list(stats);
    lists->active_user_stats          = make_active_user_stats_list  (stats);
    lists->city_driver_stats_lists_ht = g_hash_table_new_full(g_str_hash, g_str_equal, free, destroy_city_driver_stats_ht);
    // lists->active_driver_stats_arr    = make_arr(stats);
    return lists;
}

GList *get_active_driver_stats_list(LIST *lists) {
    return lists->active_driver_stats;
}

GList *get_active_user_stats_list(LIST *lists) {
    return lists->active_user_stats;
}

void *get_city_driver_stats_lists_ht (LIST *lists) {
    return lists->city_driver_stats_lists_ht;
}
void destroy_lists (LIST *list) {
    g_list_free(list->active_driver_stats);
    g_list_free(list->active_user_stats);
    g_hash_table_destroy(list->city_driver_stats_lists_ht);
    free(list);
}