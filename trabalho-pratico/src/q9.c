#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/lists.h"
#include "../includes/utils.h"

#include "../includes/rides.h"

int compare_rides (const void *a, const void *b) {
    const RIDE *ride_a = (RIDE *) a;
    const RIDE *ride_b = (RIDE *) b;

    int r = get_ride_distance(ride_b) - get_ride_distance(ride_a);
    if (r != 0) return r;

    int date_a = get_ride_date(ride_a);
    int date_b = get_ride_date(ride_b);
    
    r = date_a - date_b;

    if (r != 0) return r;

    int id_a = get_ride_id(ride_a);
    int id_b = get_ride_id(ride_b);

    return id_b - id_a;
}

void *q9_time_interval_rides (STATS *stats, char *line) {

    void *rides = NULL;

    char *date_inf, *date_sup;
    char *aux = line;
    date_inf = strsep(&aux, " \n");
    date_sup = strsep(&aux, " \n");

    int d_inf = date_to_int(date_inf);
    int d_sup = date_to_int(date_sup);


    while (d_inf <= d_sup) {

        DATE_STATS *date_stats = g_hash_table_lookup(get_dates_stats_hash(stats), GINT_TO_POINTER(d_inf));

        if (date_stats != NULL) {
            void *list = get_date_stats_rides_list(date_stats);
            if (list != NULL)
                rides = g_list_concat(rides, list);
        }

        d_inf = next_date_int(d_inf);
    }

    return g_list_sort(rides, compare_rides);
}

void query_9 (STATS *stats, char *path, int i, char *line) {
    
    char *file = malloc(sizeof(char) * 25);
    sprintf(file, "command%d_output.txt", i);
    char *filepath = concat(path, file);
    free(file);

    FILE *fp = fopen(filepath, "w+");
    free(filepath);

    void *list = q9_time_interval_rides(stats, line);

    GList *aux = list;
    while (aux != NULL) {

        RIDE *ride = aux->data;
        int id = get_ride_id(ride);
        char *date = int_to_date(get_ride_date(ride));
        int dist = get_ride_distance(ride);
        char *city = get_ride_city(ride);
        double tip = get_ride_tip(ride);

        fprintf(fp, "%012d;%s;%d;%s;%.3f\n", id, date, dist, city, tip);

        free(date);
        free(city);
        aux = aux->next;
    }
    
    g_list_free(list);
    fclose(fp);    
}