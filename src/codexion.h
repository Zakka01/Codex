
#ifndef CODEXION_H
# define CODEXION_H

#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


typedef struct s_data t_data;
typedef struct s_coder t_coder;
typedef struct s_dongle t_dongle;


typedef struct s_dongle
{
  int             id;
  int             coder_id;
  pthread_mutex_t lock;

} t_dongle;


typedef struct s_coder
{
  int       id;
  pthread_t thread;
  int       dongle_1_id;
  int       dongle_2_id;

  
  t_data    *data;

} t_coder;


typedef struct s_data
{
  int         number_of_coders;
  long        time_to_burnout;
  long        time_to_compile;
  long        time_to_debug;
  long        time_to_refactor;
  long        number_of_compiles;
  long        dongle_cooldown;
  char        *scheduler;

  pthread_mutex_t start_mutex;
  pthread_cond_t  start_cond;
  int             start;

  int         *queue;
  int         queue_size;
  t_coder     *coders;
  t_dongle    *dongles;

} t_data;


int   ft_atoi(const char	*str);
int   valid_values(t_data *data);
int   valid_numbers(char *av);
int   initializer(t_data *data);
void  *coder_routine(void *coder);
int   get_args(int ac, char **av, t_data *data);
void  release_dongles(t_coder *coder);
void  append_queue(t_coder *coder);
int   acquire_dongles(t_coder *coder);


#endif