#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/lists.h"
#include "../includes/utils.h"

#include "../includes/drivers.h"

void query_2 (LIST *lists, HASH *hash, char *path, int i, char *line) {
    
    char *file = malloc(sizeof(char) * 25);
    sprintf(file, "command%d_output.txt", i);
    char *filepath = concat(path, file);
    free(file);

    FILE *fp = fopen(filepath, "w+");
    free(filepath);

    int N = atoi(line);

    // GPtrArray *arr = get_active_driver_stats_arr (lists);

    // for (int i = 0; i < N; i++) {
    //     DRIVER_STATS *driver_stats = g_ptr_array_index(arr, i);
    //     char *id = get_driver_stats_id(driver_stats);
    //     double avaliacao_media = get_driver_stats_avaliacao_media(driver_stats);
    //     DRIVER *driver = g_hash_table_lookup(get_drivers_hash(hash), id);
    //     char *nome = get_driver_name(driver);
    //     printf("%s;%s;%lf;%d\n", id, nome, avaliacao_media, get_driver_stats_ultima_viagem(driver_stats));
    //     fprintf(fp, "%s;%s;%.3f\n", id, nome, avaliacao_media);
    //     free(id);
    //     free(nome);
    // }
    
    GList *aux = get_active_driver_stats_list(lists);
    for (int j = 0; (j < N) && aux; j++) {
        DRIVER_STATS *driver_stats = aux->data;

        char *id = get_driver_stats_id(driver_stats);
        double avaliacao_media = get_driver_stats_avaliacao_media(driver_stats);

        DRIVER *driver = g_hash_table_lookup(get_drivers_hash(hash), id);
        char *nome = get_driver_name(driver);

        // printf("%s;%s;%lf;%d\n\n", id, nome, avaliacao_media, get_driver_stats_ultima_viagem(driver_stats));

        fprintf(fp, "%s;%s;%.3f\n", id, nome, avaliacao_media);

        free(id);
        free(nome);

        aux = aux->next;
    }

    fclose(fp);
}
