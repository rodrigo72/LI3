#ifndef QUERIES_H
#define QUERIES_H

#include "./structs.h"
#include "./stats.h"
#include "./lists.h"

void query_manager (LIST *lists, STATS *stats, HASH *hash, int query, int i, char *line);

#endif // QUERIES_H
