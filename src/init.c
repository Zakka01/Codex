/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 13:54:07 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/16 19:12:23 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void init_queue(t_data *data)
{
    int i;

    data->queue = malloc(sizeof(int) * data->number_of_coders);
    if (!data->queue)
        return;

    data->queue_size = 0;
    i = 0;

    while (i < data->number_of_coders)
    {
        data->queue[data->queue_size] = data->coders[i].id;
        data->queue_size++;
        printf("C%d added\n", data->coders[i].id + 1);
        i++;
    }
}

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

    if (!init_coders(data))
        return (0);

    init_queue(data);
    create_threads(data);
    join_threads(data);
    

    return (1);
}
