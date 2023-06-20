#include <stdlib.h>

#include "../includes/structs.h"
#include "../includes/stats.h"
#include "../includes/lists.h"
#include "../includes/utils.h"


#include "../includes/q1.h"
#include "../includes/q2.h"
#include "../includes/q3.h"
#include "../includes/q4.h"
#include "../includes/q5.h"
#include "../includes/q6.h"
#include "../includes/q7.h"
#include "../includes/q8.h"
#include "../includes/q9.h"

void query_manager (LIST *lists, STATS *stats, HASH *hash, int query, int i, char *line) {

    char *filepath = "./Resultados/";

    switch(query) {
        case 1:
            query_1 (stats, hash, filepath, i, line);
            break;
        case 2:
            query_2 (lists, hash, filepath, i, line);
            break;
        case 3:
            query_3 (lists, hash, filepath, i, line);
            break;
        case 4:
            query_4 (stats, filepath, i, line);
            break;
        case 5:
            query_5 (stats, filepath, i, line);
            break;
        case 6:
            query_6 (stats, filepath, i, line);
            break;
        case 7:
            query_7 (stats, hash, lists, filepath, i, line);
            break;
        case 8:
            query_8 (stats, hash, filepath, i, line);
            break;
        case 9:
            query_9 (stats, filepath, i, line);
            break;
        default:
            break;
    }
}
