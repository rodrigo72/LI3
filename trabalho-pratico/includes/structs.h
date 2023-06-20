#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct usr USER;
typedef struct drv DRIVER;
typedef struct rd RIDE;

typedef struct ht HASH;


HASH *create_hash ();
void destroy_hash ();

void *get_drivers_hash (HASH *hash);
void *get_users_hash   (HASH *hash);
void *get_rides_hash   (HASH *hash);

#endif // STRUCTS_H
