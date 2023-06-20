#ifndef STATS_H
#define STATS_H

#include "./structs.h"

typedef struct st STATS;

typedef struct usr_st   USER_STATS;
typedef struct drv_st   DRIVER_STATS;
typedef struct dt_st    DATE_STATS;
typedef struct ct_st    CITY_STATS;
typedef struct dt_ct_st DATE_CITY_STATS;
typedef struct dr_ct_st DRIVER_CITY_STATS;

STATS *create_stats  ();
void   destroy_stats (STATS *stats);
void   calc_stats    (HASH *hash, STATS *stats);

void *get_drivers_stats_hash   (STATS *stats);
void *get_users_stats_hash     (STATS *stats);
void *get_dates_stats_hash     (STATS *stats);
void *get_cities_stats_hash    (STATS *stats);
void *get_f_rides_array        (STATS *stats);

void *get_f_account_status_ht  (STATS *stats); 
void *get_m_account_status_ht  (STATS *stats); 

void *get_f_rides_arr          (STATS *stats);
void *get_m_rides_arr          (STATS *stats);

int   get_f_max_account_age    (STATS *stats); 
int   get_m_max_account_age    (STATS *stats);

char  *get_user_stats_username             (const USER_STATS *user);
double get_user_stats_avaliacao_media      (const USER_STATS *user);
double get_user_stats_total_gasto          (const USER_STATS *user);
int    get_user_stats_numero_viagens       (const USER_STATS *user);
int    get_user_stats_distancia_viajada    (const USER_STATS *user);
int    get_user_stats_ultima_viagem        (const USER_STATS *user);
char   get_user_stats_account_status       (const USER_STATS *user);

char  *get_driver_stats_id                 (const DRIVER_STATS *driver);
double get_driver_stats_avaliacao_media    (const DRIVER_STATS *driver);
double get_driver_stats_total_auferido     (const DRIVER_STATS *driver);
int    get_driver_stats_numero_viagens     (const DRIVER_STATS *driver);
int    get_driver_stats_distancia_viajada  (const DRIVER_STATS *driver);
int    get_driver_stats_ultima_viagem      (const DRIVER_STATS *driver);
char   get_driver_stats_account_status     (const DRIVER_STATS *driver);
char  *get_driver_stats_city               (const DRIVER_STATS *driver);

int    get_date_stats_distancia_viajada    (const DATE_STATS *date);
int    get_date_stats_numero_viagens       (const DATE_STATS *date);
double get_date_stats_preco_total          (const DATE_STATS *date);
void  *get_date_stats_city_stats           (const DATE_STATS *date);
void  *get_date_stats_rides_list           (const DATE_STATS *date);

int get_date_city_stats_distancia_viajada  (const DATE_CITY_STATS *dc);
int get_date_city_stats_numero_viagens     (const DATE_CITY_STATS *dc);

int    get_city_stats_distancia_viajada    (const CITY_STATS *city);
int    get_city_stats_numero_viagens       (const CITY_STATS *city);
double get_city_stats_preco_total          (const CITY_STATS *city);
void  *get_city_stats_driver_stats_ht      (const CITY_STATS *city);

int   get_driver_city_stats_numero_viagens (const DRIVER_CITY_STATS *dc);
int   get_driver_city_stats_sum_rating     (const DRIVER_CITY_STATS *dc);
char *get_driver_city_stats_id             (const DRIVER_CITY_STATS *dc);

#endif // STATS_H
