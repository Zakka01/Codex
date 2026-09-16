/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:36:36 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/16 18:47:09 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int get_coder(t_coder *coder)
{
    int chosen_coder;
    int i;

    chosen_coder = coder->data->queue[0];
    i = 0;

    while (i < coder->data->queue_size)
    {
        coder->data->queue[i] = coder->data->queue[i + 1];
        i++;
    }

    coder->data->queue_size--;
    return (chosen_coder);
}

int scheduler_fifo(t_coder *coder)
{
    int n_compiles;
    int chosen_coder;
    
    n_compiles = coder->data->number_of_compiles;
    while (n_compiles > 0)
    {
        chosen_coder = get_coder(coder);
        acquire_dongles(&coder->data->coders[chosen_coder]);

        n_compiles--;
    }

    return (chosen_coder);
}

int scheduler_edf(t_coder *coder)
{
    (void)coder;
    return 0;
}