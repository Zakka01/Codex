/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 13:54:07 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/16 14:54:50 by zahrabar         ###   ########.fr       */
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
    int j;
    int err;

    data->queue = malloc(sizeof(int) * data->number_of_coders);
    data->queue_size = 0;
    i = 0;

    while (i < data->number_of_coders){
        data->coders[i].id = i;
        data->coders[i].dongle_1_id = -1;
        data->coders[i].dongle_2_id = -1;
        data->coders[i].data = data;

        err = pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]);
        if (err != 0)
            return (0);
        i++;
    }

    j = 0;
    while (j < data->number_of_coders){
        pthread_join(data->coders[j].thread, NULL);
        j++;
    }
    return (1);
}

int initializer(t_data *data){

    data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
    if (!data->coders)
        return (0);

    data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    if (!data->dongles)
        return (0);

    init_dongles(data);

    if (!init_coders(data))
        return (0);

    return (1);
}
