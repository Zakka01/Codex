/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 15:44:49 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/17 18:40:26 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int create_threads(t_data *data)
{
    int i;

    i = 0;
    while (i < data->number_of_coders)
    {   
        if (pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]))
            return (0);
        i++;
    }

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
    return (1);
}