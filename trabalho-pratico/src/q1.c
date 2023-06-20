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

void query_1 (STATS *stats, HASH *hash, char *path, int i, char *str) {
    char *file = malloc(sizeof(char) * 25);
    sprintf(file, "command%d_output.txt", i);
    char *filepath = concat(path, file);
    free(file);

    FILE *fp = fopen(filepath, "w+");
    free(filepath);

    if ((*str >= '0') && (*str <= '9')) { // driver
        DRIVER *driver = g_hash_table_lookup(get_drivers_hash(hash), str);
        if (driver != NULL) {
            DRIVER_STATS *driver_stats = g_hash_table_lookup(get_drivers_stats_hash(stats), str);
    
            char status = get_driver_account_status(driver);
            if (status == 'a') {
                char  *nome            = get_driver_name(driver);
                char   genero          = get_driver_gender(driver);
                int    idade           = int_to_age(get_driver_birth_date(driver));
            
                double avaliacao_media = get_driver_stats_avaliacao_media(driver_stats);
                int    numero_viagens  = get_driver_stats_numero_viagens(driver_stats);
                double total_auferido  = get_driver_stats_total_auferido(driver_stats);
            
                fprintf(fp, "%s;%c;%d;%.3f;%d;%.3f\n", nome, genero, idade, avaliacao_media, numero_viagens, total_auferido);
    
                free(nome);
            } 
        }
    } else { // user
        USER *user = g_hash_table_lookup(get_users_hash(hash), str);
        if (user != NULL) {
            USER_STATS *user_stats = g_hash_table_lookup(get_users_stats_hash(stats), str);

            char status = get_user_account_status(user);
            if (status == 'a') {
                char  *nome            = get_user_name(user);
                char   genero          = get_user_gender(user);
                int    idade           = int_to_age(get_user_birth_date(user));

                double avaliacao_media = get_user_stats_avaliacao_media(user_stats);
                int    numero_viagens  = get_user_stats_numero_viagens(user_stats);
                double total_gasto     = get_user_stats_total_gasto(user_stats);

                fprintf(fp, "%s;%c;%d;%.3f;%d;%.3f\n", nome, genero, idade, avaliacao_media, numero_viagens, total_gasto);

                free(nome);
            }
        }
    }

    fclose(fp);
}
