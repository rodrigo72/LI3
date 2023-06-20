#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/lists.h"
#include "../includes/utils.h"

#include "../includes/users.h"

void query_3 (LIST *lists, HASH *hash, char *path, int i, char *line) {

    char *file = malloc(sizeof(char) * 25);
    sprintf(file, "command%d_output.txt", i);
    char *filepath = concat(path, file);
    free(file);

    FILE *fp = fopen(filepath, "w+");
    free(filepath);

    int N = atoi(line);

    GList *aux = get_active_user_stats_list(lists);
    for (int j = 0; (j < N) && aux; j++) {
        USER_STATS *user_stats = g_list_nth_data(aux, 0);

        char *username          = get_user_stats_username(user_stats);
        USER *user              = g_hash_table_lookup(get_users_hash(hash), username);
        char *nome              = get_user_name(user);
        int   distancia_viajada = get_user_stats_distancia_viajada(user_stats);

        fprintf(fp, "%s;%s;%d\n", username, nome, distancia_viajada);

        free(username);
        free(nome);

        aux = aux->next;
    }

    fclose(fp);
}
