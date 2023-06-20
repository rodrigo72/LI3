#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/lists.h"
#include "../includes/utils.h"

double q5_time_interval_avg_price (STATS *stats, char *line) {

    char *date_inf, *date_sup;
    char *aux = line;
    date_inf = strsep(&aux, " \n");
    date_sup = strsep(&aux, " \n");

    int d_inf = date_to_int(date_inf);
    int d_sup = date_to_int(date_sup);

    int total_de_viagens = 0;
    double total_gasto = 0.0;

    while (d_inf <= d_sup) {

        DATE_STATS *date_stats = g_hash_table_lookup(get_dates_stats_hash(stats), GINT_TO_POINTER(d_inf));
        if (date_stats != NULL) {
            total_de_viagens += get_date_stats_numero_viagens(date_stats);
            total_gasto += get_date_stats_preco_total(date_stats);
        }

        d_inf = next_date_int(d_inf);
    }

    double result = total_gasto / (double) total_de_viagens;
    return (total_de_viagens != 0) ? result : 0;
}

// Preço médio das viagens num dado intervalo de tempo
void query_5 (STATS *stats, char *path, int i, char *line) {
    
    char *file = malloc(sizeof(char) * 25);
    sprintf(file, "command%d_output.txt", i);
    char *filepath = concat(path, file);
    free(file);

    FILE *fp = fopen(filepath, "w+");
    free(filepath);

    double d = q5_time_interval_avg_price(stats, line);

    if (d > 0) fprintf(fp, "%.3f\n", d);
    fclose(fp);    
}