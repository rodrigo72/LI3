#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <glib.h>

#include "../includes/structs.h"
#include "../includes/utils.h"
#include "../includes/stats.h"

#include "../includes/drivers.h"
#include "../includes/users.h"
#include "../includes/rides.h"

typedef struct usr_st {
    char  *username;

    double avaliacao_media;
    double total_gasto;
    int    numero_viagens;
    int    distancia_viajada;
    int    ultima_viagem;

    char  account_status;
} USER_STATS;

typedef struct drv_st {
    char  *id;

    double avaliacao_media;
    double total_auferido;
    int    numero_viagens;
    int    distancia_viajada;
    int    ultima_viagem;

    char account_status;
    char *city;
} DRIVER_STATS;

typedef struct dt_st {
    int numero_viagens;
    int distancia_viajada;
    double preco_total;

    void *date_city_stats;
    void *rides_list;

} DATE_STATS;

typedef struct dt_ct_st {
    int numero_viagens;
    int distancia_viajada;
} DATE_CITY_STATS;

typedef struct ct_st {
    int numero_viagens;
    int distancia_viajada;
    double preco_total;

    void *driver_stats_ht;
} CITY_STATS;

typedef struct dr_ct_st {
    int numero_viagens;
    int sum_rating;
    char *id;
} DRIVER_CITY_STATS;

typedef struct st {
    void *user_stats;
    void *driver_stats;
    void *date_stats;
    void *city_stats;

    void *f_account_status_ht;
    void *m_account_status_ht;

    int f_max_account_age;
    int m_max_account_age;

    void *f_rides_arr;
    void *m_rides_arr;

} STATS;

typedef struct {
    HASH *hash;
    STATS *stats;    
} AUX_STRUCT;

USER_STATS *create_user_stats (char *username, char account_status) {
    USER_STATS *user_stats = malloc(sizeof(USER_STATS));

    user_stats->username = strdup(username);
    user_stats->avaliacao_media = 0;
    user_stats->total_gasto = 0;
    user_stats->numero_viagens = 0;
    user_stats->distancia_viajada = 0;
    user_stats->ultima_viagem = 90000000;
    user_stats->account_status = account_status;
    
    return user_stats; 
}

void destroy_user_stats (void *a) {
    USER_STATS *user_stats = a;
    free(user_stats->username);
    free(user_stats);
}

DRIVER_STATS *create_driver_stats (char *id, char account_status, char *city) {
    DRIVER_STATS *driver_stats = malloc(sizeof(DRIVER_STATS));

    driver_stats->id = strdup(id);
    driver_stats->avaliacao_media = 0;
    driver_stats->total_auferido = 0;
    driver_stats->numero_viagens = 0;
    driver_stats->distancia_viajada = 0;
    driver_stats->ultima_viagem = 90000000;
    driver_stats->account_status = account_status;
    driver_stats->city = strdup(city);

    return driver_stats; 
}

void destroy_driver_stats (void *a) {
    DRIVER_STATS *driver_stats = a;
    free(driver_stats->id);
    free(driver_stats->city);
    free(driver_stats);
}

DATE_STATS *create_date_stats () {
    DATE_STATS *date_stats = malloc(sizeof(DATE_STATS));
    date_stats->distancia_viajada = 0;
    date_stats->numero_viagens = 0;
    date_stats->preco_total = 0.0;
    date_stats->date_city_stats = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    date_stats->rides_list = NULL;
    return date_stats; 
}

void destroy_date_stats (void *a) {
    DATE_STATS *date_stats = a;
    g_hash_table_destroy(date_stats->date_city_stats);
    g_list_free(date_stats->rides_list);
    free(date_stats);
}

void destroy_driver_city_stats(void *a) {
    DRIVER_CITY_STATS *dc_stats = a;
    free(dc_stats->id);
    free(dc_stats);
}

CITY_STATS *create_city_stats () {
    CITY_STATS *city_stats = malloc(sizeof(CITY_STATS));
    city_stats->distancia_viajada = 0;
    city_stats->numero_viagens = 0;
    city_stats->preco_total = 0.0;
    city_stats->driver_stats_ht = g_hash_table_new_full(g_str_hash, g_str_equal, free, destroy_driver_city_stats);
    return city_stats;
}

void destroy_city_stats (void *a) {
    CITY_STATS *city_stats = a;
    g_hash_table_destroy(city_stats->driver_stats_ht);
    free(city_stats);
}

DRIVER_CITY_STATS *create_driver_city_stats (char *id) {
    DRIVER_CITY_STATS *dc_stats = malloc(sizeof(DRIVER_CITY_STATS));
    dc_stats->id = strdup(id);
    dc_stats->numero_viagens = 0;
    dc_stats->sum_rating = 0;
    return dc_stats;
}

DATE_CITY_STATS *create_date_city_stats () {
    DATE_CITY_STATS *dc_stats = malloc(sizeof(DATE_CITY_STATS));
    dc_stats->distancia_viajada = 0;
    dc_stats->numero_viagens = 0;
    return dc_stats;
}

void destroy_ride_array (void *a) {
    GPtrArray *arr = a;
    g_ptr_array_free(arr, 1);
}

STATS *create_stats () {
    STATS *stats = malloc(sizeof(STATS));

    stats->user_stats   = g_hash_table_new_full(g_str_hash, g_str_equal, free, destroy_user_stats);
    stats->driver_stats = g_hash_table_new_full(g_str_hash, g_str_equal, free, destroy_driver_stats);
    stats->city_stats   = g_hash_table_new_full(g_str_hash, g_str_equal, free, destroy_city_stats);
    stats->date_stats   = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, destroy_date_stats);

    GPtrArray **array_f = malloc(sizeof(GPtrArray *) * 20);
    for (int i = 0; i < 20; i++) array_f[i] = NULL;

    GPtrArray **array_m = malloc(sizeof(GPtrArray *) * 20);
    for (int i = 0; i < 20; i++) array_m[i] = NULL;

    stats->f_account_status_ht = array_f;
    stats->m_account_status_ht = array_m;
    
    stats->f_max_account_age = 0;
    stats->m_max_account_age = 0;

    stats->f_rides_arr = g_ptr_array_new();
    stats->m_rides_arr = g_ptr_array_new();

    return stats;
}

void destroy_stats (STATS *stats) {
    g_hash_table_destroy(stats->user_stats);
    g_hash_table_destroy(stats->driver_stats);
    g_hash_table_destroy(stats->city_stats);
    g_hash_table_destroy(stats->date_stats);

    GPtrArray **array_m = stats->m_account_status_ht;
    GPtrArray **array_f = stats->f_account_status_ht;

    for (int i = 0; i < 20; i++) {
        if (array_f[i] != NULL) g_ptr_array_free(array_f[i], 1);
        if (array_m[i] != NULL) g_ptr_array_free(array_m[i], 1);
    }

    free(array_m);
    free(array_f);

    g_ptr_array_free(stats->f_rides_arr, 1);
    g_ptr_array_free(stats->m_rides_arr, 1);
    free(stats);
}


void update_driver_stats
(DRIVER_STATS *driver_stats,
 int   score_driver,
 int   distance,
 char car_class,
 int ultima_viagem,
 double tip
)
{

    driver_stats->numero_viagens = driver_stats->numero_viagens + 1;
    driver_stats->avaliacao_media = (driver_stats->avaliacao_media * (double) (driver_stats->numero_viagens - 1) + score_driver) / (double) driver_stats->numero_viagens; 
    driver_stats->distancia_viajada = driver_stats->distancia_viajada + distance;

    driver_stats->ultima_viagem = driver_stats->ultima_viagem > ultima_viagem ? ultima_viagem : driver_stats->ultima_viagem;
    
    if (car_class == 'b') 
        driver_stats->total_auferido = driver_stats->total_auferido + 3.25 + (0.62 * (double) distance);
    else if (car_class == 'g')
        driver_stats->total_auferido = driver_stats->total_auferido + 4.00 + (0.79 * (double) distance);
    else if (car_class == 'p')
        driver_stats->total_auferido = driver_stats->total_auferido + 5.20 + (0.94 * (double) distance);
    
    driver_stats->total_auferido = driver_stats->total_auferido + tip;
}

void update_user_stats
(USER_STATS *user_stats,
 int   score_user,
 int   distance,
 char  car_class,
 int ultima_viagem,
 double tip
) 
{
    user_stats->numero_viagens = user_stats->numero_viagens + 1;
    user_stats->avaliacao_media = (user_stats->avaliacao_media * (user_stats->numero_viagens - 1) + score_user) / user_stats->numero_viagens; 
    user_stats->distancia_viajada = user_stats->distancia_viajada + distance;

    user_stats->ultima_viagem = user_stats->ultima_viagem > ultima_viagem ? ultima_viagem : user_stats->ultima_viagem;
    
    if (car_class == 'b') 
        user_stats->total_gasto = user_stats->total_gasto + 3.25 + (0.62 * distance);
    else if (car_class == 'g')
        user_stats->total_gasto = user_stats->total_gasto + 4.00 + (0.79 * distance);
    else if (car_class == 'p')
        user_stats->total_gasto = user_stats->total_gasto + 5.20 + (0.94 * distance);
    
    user_stats->total_gasto = user_stats->total_gasto + tip;
}

void update_city_stats (CITY_STATS *city_stats, int distance, char car_class, char account_status, int score_driver, char *driver_id) {

    city_stats->numero_viagens += 1;
    city_stats->distancia_viajada += distance;

    if (car_class == 'b') 
        city_stats->preco_total += 3.25 + (0.62 * (double) distance);
    else if (car_class == 'g')
        city_stats->preco_total += 4.00 + (0.79 * (double) distance);
    else if (car_class == 'p')
        city_stats->preco_total += 5.20 + (0.94 * (double) distance);

    if (account_status == 'a') {
        DRIVER_CITY_STATS *dc_stats = g_hash_table_lookup(city_stats->driver_stats_ht, driver_id);
        if (dc_stats == NULL) {
            char *id_dup = strdup(driver_id);
            dc_stats = create_driver_city_stats(driver_id);
            g_hash_table_insert(city_stats->driver_stats_ht, id_dup, dc_stats);
        }
        dc_stats->numero_viagens++;
        dc_stats->sum_rating += score_driver;
    }

}

void update_date_stats (DATE_STATS *date_stats, int distance, char car_class, char *city, double tip, RIDE *ride) {

    date_stats->numero_viagens += 1;
    date_stats->distancia_viajada += distance;

    if (car_class == 'b') 
        date_stats->preco_total += 3.25 + (0.62 * (double) distance);
    else if (car_class == 'g')
        date_stats->preco_total += 4.00 + (0.79 * (double) distance);
    else if (car_class == 'p')
        date_stats->preco_total += 5.20 + (0.94 * (double) distance);

    DATE_CITY_STATS *dc_stats = g_hash_table_lookup(date_stats->date_city_stats, city);
    if (!dc_stats) {
        char *city_dup = strdup(city);
        dc_stats = create_date_city_stats();
        g_hash_table_insert(date_stats->date_city_stats, city_dup, dc_stats);
    }

    if (tip > 0) date_stats->rides_list = g_list_prepend(date_stats->rides_list, ride);

    dc_stats->distancia_viajada += distance;
    dc_stats->numero_viagens += 1;

}

void ride_iter (void *value, void *user_data) {
    RIDE *ride = value;
    AUX_STRUCT *aux = user_data;
    HASH *hash = aux->hash;
    STATS *stats = aux->stats;
    
    char *driver_id = get_ride_driver(ride);
    char *user_username = get_ride_user(ride);
    int date = get_ride_date(ride);

    int distance = get_ride_distance(ride);
    int score_driver = get_ride_score_driver(ride);
    int score_user = get_ride_score_user(ride);
    char *city_ride = get_ride_city(ride);

    double tip = get_ride_tip(ride);

    USER   *user   = g_hash_table_lookup(get_users_hash(hash), user_username);
    DRIVER *driver = g_hash_table_lookup(get_drivers_hash(hash), driver_id);
    if (driver != NULL) {
        char car_class = get_driver_car_class(driver);
        char *city = get_driver_city(driver);
        char d_account_status = get_driver_account_status(driver);

        DRIVER_STATS *driver_stats = g_hash_table_lookup(stats->driver_stats, driver_id);
        if (!driver_stats) {
            driver_stats = create_driver_stats(driver_id, d_account_status, city);
            g_hash_table_insert(stats->driver_stats, get_ride_driver(ride), driver_stats);
        }
        update_driver_stats(driver_stats,
                            score_driver,
                            distance,
                            car_class,
                            date,
                            tip);

        USER_STATS *user_stats = g_hash_table_lookup(stats->user_stats, user_username);
        char u_account_status = get_user_account_status(user);
        if (!user_stats) {
            user_stats = create_user_stats(user_username, u_account_status);
            g_hash_table_insert(stats->user_stats, get_ride_user(ride), user_stats);
        }
        update_user_stats(user_stats,
                          score_user,
                          distance,
                          car_class,
                          date,
                          tip);


        DATE_STATS *date_stats = g_hash_table_lookup((GHashTable *)stats->date_stats, GINT_TO_POINTER(date));
        if (!date_stats) {
            date_stats = create_date_stats();
            g_hash_table_insert(stats->date_stats, GINT_TO_POINTER(date), date_stats);
        }
        update_date_stats(date_stats, distance, car_class, city_ride, tip, ride);
        
        CITY_STATS *city_stats = g_hash_table_lookup(stats->city_stats, city_ride);
        if (!city_stats) {
            city_stats = create_city_stats();
            g_hash_table_insert(stats->city_stats, get_ride_city(ride), city_stats);
        }
        update_city_stats(city_stats, distance, car_class, d_account_status, score_driver, driver_id);
        
        free(city);
    
        char gender_driver = get_driver_gender(driver);
        char gender_user   = get_user_gender(user);

        if ((gender_driver == gender_user) && (d_account_status == 'a') && (u_account_status == 'a')) {
    
            int d_account_age = int_to_age(get_driver_account_creation(driver));
            int u_account_age = int_to_age(get_user_account_creation(user));

            int key = u_account_age > d_account_age ? d_account_age : u_account_age;
            if (gender_driver == 'F') {

                GPtrArray **main_arr = stats->f_account_status_ht;
                GPtrArray *arr = main_arr[key];

                if (arr == NULL) {
                    GPtrArray *new = g_ptr_array_new();
                    g_ptr_array_add(new, ride);
                    main_arr[key] = new;
                } else {
                    g_ptr_array_add(arr, ride);
                }

                // g_ptr_array_add(stats->f_rides_arr, ride);

            } else {

                GPtrArray **main_arr = stats->m_account_status_ht;
                GPtrArray *arr = main_arr[key];

                if (arr == NULL) {
                    GPtrArray *new = g_ptr_array_new();
                    g_ptr_array_add(new, ride);
                    main_arr[key] = new;
                } else {
                    g_ptr_array_add(arr, ride);
                }
                
                // g_ptr_array_add(stats->m_rides_arr, ride);
                
            }
        }
    }

    free(city_ride);
    free(driver_id);
    free(user_username);
}

void calc_stats (HASH *hash, STATS *stats) {
    AUX_STRUCT aux;
    aux.hash = hash;
    aux.stats = stats;
    g_ptr_array_foreach((GPtrArray *)get_rides_hash(hash), ride_iter, &aux);
    // g_ptr_array_sort_with_data(stats->f_rides_arr, compare_rides_q8, &aux);
    // g_ptr_array_sort_with_data(stats->m_rides_arr, compare_rides_q8, &aux);
}

char  *get_user_stats_username               (const USER_STATS *user) {return strdup(user->username) ;}
int    get_user_stats_ultima_viagem          (const USER_STATS *user) {return user->ultima_viagem    ;}
char   get_user_stats_account_status         (const USER_STATS *user) {return user->account_status   ;}
double get_user_stats_avaliacao_media        (const USER_STATS *user) {return user->avaliacao_media  ;}
double get_user_stats_total_gasto            (const USER_STATS *user) {return user->total_gasto      ;}
int    get_user_stats_numero_viagens         (const USER_STATS *user) {return user->numero_viagens   ;}
int    get_user_stats_distancia_viajada      (const USER_STATS *user) {return user->distancia_viajada;}

char  *get_driver_stats_id                   (const DRIVER_STATS *driver) {return strdup(driver->id)       ;}
int    get_driver_stats_ultima_viagem        (const DRIVER_STATS *driver) {return driver->ultima_viagem    ;}
char   get_driver_stats_account_status       (const DRIVER_STATS *driver) {return driver->account_status   ;}
char  *get_driver_stats_city                 (const DRIVER_STATS *driver) {return strdup(driver->city)     ;}
double get_driver_stats_avaliacao_media      (const DRIVER_STATS *driver) {return driver->avaliacao_media  ;}
double get_driver_stats_total_auferido       (const DRIVER_STATS *driver) {return driver->total_auferido   ;}
int    get_driver_stats_numero_viagens       (const DRIVER_STATS *driver) {return driver->numero_viagens   ;}
int    get_driver_stats_distancia_viajada    (const DRIVER_STATS *driver) {return driver->distancia_viajada;}

int    get_date_stats_distancia_viajada      (const DATE_STATS *date) {return date->distancia_viajada      ;}
int    get_date_stats_numero_viagens         (const DATE_STATS *date) {return date->numero_viagens         ;}
double get_date_stats_preco_total            (const DATE_STATS *date) {return date->preco_total            ;}
void  *get_date_stats_city_stats             (const DATE_STATS *date) {return date->date_city_stats        ;}
void  *get_date_stats_rides_list             (const DATE_STATS *date) {return g_list_copy(date->rides_list);}

int    get_date_city_stats_distancia_viajada (const DATE_CITY_STATS *dc) {return dc->distancia_viajada;}
int    get_date_city_stats_numero_viagens    (const DATE_CITY_STATS *dc) {return dc->numero_viagens   ;}

int    get_city_stats_distancia_viajada      (const CITY_STATS *city) {return city->distancia_viajada;}
int    get_city_stats_numero_viagens         (const CITY_STATS *city) {return city->numero_viagens   ;}
double get_city_stats_preco_total            (const CITY_STATS *city) {return city->preco_total      ;}
void  *get_city_stats_driver_stats_ht        (const CITY_STATS *city) {return city->driver_stats_ht  ;}

void  *get_drivers_stats_hash                (STATS *stats) {return stats->driver_stats;}
void  *get_users_stats_hash                  (STATS *stats) {return stats->user_stats  ;}
void  *get_dates_stats_hash                  (STATS *stats) {return stats->date_stats  ;}
void  *get_cities_stats_hash                 (STATS *stats) {return stats->city_stats  ;}

void *get_f_account_status_ht                (STATS *stats) {return stats->f_account_status_ht;}
void *get_m_account_status_ht                (STATS *stats) {return stats->m_account_status_ht;}

void *get_f_rides_arr                        (STATS *stats) {return stats->f_rides_arr        ;}
void *get_m_rides_arr                        (STATS *stats) {return stats->m_rides_arr        ;}

int get_f_max_account_age                    (STATS *stats) {return stats->f_max_account_age;}
int get_m_max_account_age                    (STATS *stats) {return stats->m_max_account_age;}

int    get_driver_city_stats_numero_viagens  (const DRIVER_CITY_STATS *dc) {return dc->numero_viagens;}
int    get_driver_city_stats_sum_rating      (const DRIVER_CITY_STATS *dc) {return dc->sum_rating    ;}
char  *get_driver_city_stats_id              (const DRIVER_CITY_STATS *dc) {return strdup(dc->id)    ;}
