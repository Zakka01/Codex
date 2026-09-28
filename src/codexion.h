/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 18:17:14 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/28 18:47:59 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CODEXION_H
# define CODEXION_H

#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>


typedef struct s_data t_data;
typedef struct s_coder t_coder;
typedef struct s_dongle t_dongle;

typedef struct s_request
{
  int           coder_id;
  int           priority;
  unsigned long deadline;

} t_request;



typedef struct s_heap
{
  t_request *items;
  int        size;
  int        capacity;

  t_data    *data;
} t_heap;



typedef struct s_dongle
{
  int             id;
  int             coder_id;
  pthread_mutex_t lock;
  t_heap          *heap;
  
  t_data    *data;
} t_dongle;


typedef struct s_coder
{
  int             id;
  pthread_t       thread;
  int             dongle_1_id;
  int             dongle_2_id;
  unsigned long   compile_count;
  int             is_compiling;
  unsigned long   last_action_time;
  unsigned long   deadline;

  t_data          *data;

} t_coder;


typedef struct s_data
{
  // program data/params
  int             number_of_coders;
  unsigned long   time_to_burnout;
  unsigned long   time_to_compile;
  unsigned long   time_to_debug;
  unsigned long   time_to_refactor;
  unsigned long   number_of_compiles;
  unsigned long   dongle_cooldown;
  unsigned long   start_time;
  char            *scheduler;

  // coders (threads) and dongles (shared resources) and heap
  t_coder         *coders;
  t_dongle        *dongles;

  // scheduler lock and cond for singnal coder's turn
  pthread_mutex_t print_lock;
  pthread_mutex_t scheduler_lock;
  pthread_cond_t scheduler_cond;

  // stop the scheduler when coder burnout
  int             scheduler_over;
  int             priority;

  int             coders_finished;

  pthread_t       monitor;
} t_data;


int   get_args(int ac, char **av, t_data *data);
int   ft_atoi(const char	*str);
int   valid_values(t_data *data);
int   valid_numbers(char *av);
int   initializer(t_data *data);
int   create_threads(t_data *data);
int   join_threads(t_data *data);
void  *coder_routine(void *coder);
void	*monitor_routine(void *arg);

void  init_heap(t_dongle *dongle);
void  heap_push(t_heap *heap, int coder, int priority);
void  heap_push_edf(t_heap *heap, int coder, unsigned long deadline, int priority);
void  heap_pop(t_heap *heap);
int   acquire_dongles(t_coder *coder);
void  release_dongles(t_coder *coder);
int   scheduler_fifo(t_coder *coder);
int   scheduler_edf(t_coder *coder);
void  work(t_coder *coder);
long  get_time_ms(void);
void	print_log(t_coder *coder, char *msg);

#endif