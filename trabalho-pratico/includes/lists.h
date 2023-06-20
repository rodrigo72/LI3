#include <glib.h>
#include "./stats.h"

typedef struct lst LIST;

LIST *create_lists (STATS *stats, HASH *hash);
void destroy_lists (LIST *list);

GList *get_active_driver_stats_list    (LIST *lists);
GList *get_active_user_stats_list      (LIST *lists);
// GList *get_rides_list                  (LIST *lists);
void *get_city_driver_stats_lists_ht   (LIST *lists);
// GPtrArray *get_active_driver_stats_arr (LIST *lists);
