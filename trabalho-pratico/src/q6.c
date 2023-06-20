#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/stats.h"
#include "../includes/utils.h"

double q6_time_interval_city_avg_distance (STATS *stats, char *line) {

    char *city, *date_inf, *date_sup;
    char *aux = line;
    city = strsep(&aux, " \n");
    date_inf = strsep(&aux, " \n");
    date_sup = strsep(&aux, " \n");

    int d_inf = date_to_int(date_inf);
    int d_sup = date_to_int(date_sup);

    int total_de_viagens = 0;
    int distancia_total = 0.0;

    while (d_inf <= d_sup) {

        DATE_STATS *date_stats = g_hash_table_lookup(get_dates_stats_hash(stats), GINT_TO_POINTER(d_inf));

        if (date_stats != NULL) {
            DATE_CITY_STATS *dc_stats = g_hash_table_lookup(get_date_stats_city_stats(date_stats), city);
            if (dc_stats != NULL) {
                total_de_viagens += get_date_city_stats_numero_viagens(dc_stats);
                distancia_total  += get_date_city_stats_distancia_viajada(dc_stats);
            }
        }   

        d_inf = next_date_int(d_inf);
    }

    double result = (double) distancia_total / total_de_viagens;
    return (total_de_viagens != 0) ? result : 0;
}

void query_6 (STATS *stats, char *path, int i, char *line) {
    
    char *file = malloc(sizeof(char) * 25);
    sprintf(file, "command%d_output.txt", i);
    char *filepath = concat(path, file);
    free(file);

    FILE *fp = fopen(filepath, "w+");
    free(filepath);

    double d = q6_time_interval_city_avg_distance(stats, line);

    if (d != 0) fprintf(fp, "%.3f\n", d);
    fclose(fp);
}