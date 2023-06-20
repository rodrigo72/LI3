#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

#include "../includes/structs.h"
#include "../includes/stats.h"
#include "../includes/utils.h"
#include "../includes/lists.h"

#include "../includes/drivers.h"


int compare_drivers_av (const void *a, const void *b) {
    const DRIVER_CITY_STATS *d_a = a;
    const DRIVER_CITY_STATS *d_b = b;

    double av_a = (double) get_driver_city_stats_sum_rating(d_a) / get_driver_city_stats_numero_viagens(d_a);
    double av_b = (double) get_driver_city_stats_sum_rating(d_b) / get_driver_city_stats_numero_viagens(d_b);

    if (av_a < av_b) return 1; 
    if (av_a > av_b) return -1;

    char *id_a = get_driver_city_stats_id(d_a);
    char *id_b = get_driver_city_stats_id(d_b);

    int r = strcmp(id_b, id_a);

    free(id_a);
    free(id_b);

    return r;
}

void query_7 (STATS *stats, HASH *hash, LIST *lists, char *path, int i, char *line) {
    char *file = malloc(sizeof(char) * 25);
    sprintf(file, "command%d_output.txt", i);
    char *filepath = concat(path, file);
    free(file);

    FILE *fp = fopen(filepath, "w+");
    free(filepath);

    char *delim = " ";
    char *str = line;
    char *ptr1 = strsep(&str, delim);
    char *ptr2 = strsep(&str, delim);

    int N = atoi(ptr1);
    char *city = ptr2;

    GList *list = g_hash_table_lookup(get_city_driver_stats_lists_ht(lists), city);
    if (list == NULL) {
        CITY_STATS *city_stats = g_hash_table_lookup(get_cities_stats_hash(stats), city);
        if (city_stats != NULL) {
            void *drivers_ht = get_city_stats_driver_stats_ht(city_stats);
            if (drivers_ht != NULL) {
                list = g_hash_table_get_values(drivers_ht);
                list = g_list_sort(list, compare_drivers_av);
                char *city_dup = strdup(city);
                g_hash_table_insert(get_city_driver_stats_lists_ht(lists), city_dup, (void *) list);
            }
        }
    }
    
    GList *aux = list;
    for (int i = 0; i < N && aux; i++) {
        DRIVER_CITY_STATS *dc_stats = aux->data;

        char *id               = get_driver_city_stats_id(dc_stats);
        DRIVER *driver = g_hash_table_lookup(get_drivers_hash(hash), id);

        double avaliacao_media = (double) get_driver_city_stats_sum_rating(dc_stats) / get_driver_city_stats_numero_viagens(dc_stats);
        char *nome             = get_driver_name(driver);

        fprintf(fp, "%s;%s;%.3f\n", id, nome, avaliacao_media);

        free(id);
        free(nome);

        aux = aux->next;
    }

    fclose(fp);
}
