/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 13:54:07 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/26 19:56:43 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int init_dongles(t_data *data)
{
    int i;

    i = 0;
    while (i < data->number_of_coders){
        data->dongles[i].coder_id = -1;
        data->dongles[i].id = i;
        pthread_mutex_init(&data->dongles[i].lock, NULL);
        i++;
    }
    return (1);
}

int init_coders(t_data *data)
{
    int i;

    i = 0;

    while (i < data->number_of_coders){
        data->coders[i].id = i;
        data->coders[i].dongle_1_id = -1;
        data->coders[i].dongle_2_id = -1;
        data->coders[i].data = data;
        data->coders[i].last_action_time = data->start_time;
        data->coders[i].compile_count = 0;
        data->coders[i].is_compiling = 0;
        i++;
    }

    return (1);
}

int initializer(t_data *data)
{
    data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
    if (!data->coders)
        return (0);

    data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    if (!data->dongles)
        return (0);

    if (!init_dongles(data))
        return (0);

    data->start_time = get_time_ms();
    if (!init_coders(data))
        return (0);

    data->scheduler_over = 0;

    init_heap(data);
    data->priority = 0;

    pthread_mutex_init(&data->scheduler_lock, NULL);
    pthread_cond_init(&data->scheduler_cond, NULL);
    data->coders_finished = 0;

    create_threads(data);
    join_threads(data);

    return (1);
}
