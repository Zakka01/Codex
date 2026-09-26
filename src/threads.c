/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 15:44:49 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/25 18:42:41 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int create_threads(t_data *data)
{
    int i;

    i = 0;
    while (i < data->number_of_coders)
    {   
        if (pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]) != 0)
            return (0);
        i++;
    }
    pthread_create(&data->monitor, NULL, monitor_routine, data);
    return (1);
}

int join_threads(t_data *data)
{
    int j;

    j = 0;
    while (j < data->number_of_coders){
        if (pthread_join(data->coders[j].thread, NULL) != 0)
            return (0);
        j++;
    }
    pthread_join(data->monitor, NULL);
    return (1);
}
