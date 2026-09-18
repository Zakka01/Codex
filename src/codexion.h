/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 18:17:14 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/18 17:11:21 by zahrabar         ###   ########.fr       */
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
  // program data
  int             number_of_coders;
  long            time_to_burnout;
  long            time_to_compile;
  long            time_to_debug;
  long            time_to_refactor;
  long            number_of_compiles;
  long            dongle_cooldown;
  char            *scheduler;

  // coders (threads) and dongles (shared resources)
  t_coder         *coders;
  t_dongle        *dongles;
  
  // queue waiting coders
  int             *queue;
  int             queue_size;

  // start gate's states
  pthread_mutex_t start_lock;
  pthread_cond_t  start_cond;
  int             ready_count;

  pthread_mutex_t scheduler_lock;
  pthread_cond_t scheduler_cond;

  int             current_coder;

} t_data;


int   get_args(int ac, char **av, t_data *data);

int   ft_atoi(const char	*str);
int   valid_values(t_data *data);
int   valid_numbers(char *av);

int   initializer(t_data *data);
void  init_queue(t_data *data);

int create_threads(t_data *data);
int join_threads(t_data *data);

void  *coder_routine(void *coder);

int   acquire_dongles(t_coder *coder);
void  release_dongles(t_coder *coder);

void  append_queue(t_coder *coder);

int scheduler_fifo(t_coder *coder);
int scheduler_edf(t_coder *coder);

#endif