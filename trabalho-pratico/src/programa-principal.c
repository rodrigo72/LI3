#include "../includes/interactive.h"
#include "../includes/sub_parser.h"
#include "../includes/structs.h"
#include "../includes/parser.h"
#include "../includes/stats.h"
#include "../includes/lists.h"

int main (int argc, char **argv) {

    if (argc == 1)
        interactive();
    
    if (argc == 3) {
        HASH *hash = create_hash();
        STATS *stats = create_stats();
        
        parser(argv[1], hash);
        calc_stats(hash, stats);
        LIST *lists = create_lists(stats, hash);
        
        sub_parser(lists, stats, hash, argv[2]);

        destroy_hash(hash);
        destroy_stats(stats);
        destroy_lists(lists);
    }

    return 0;
}
