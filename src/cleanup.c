/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 00:05:54 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/30 00:28:55 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void    cleanup(t_data *data)
{
    int i;

    free(data->coders);
    i = 0;
    
    while (i < data->number_of_coders)
    {
        free(data->dongles[i].heap->items);
        free(data->dongles[i].heap);
        i++;
    }
    free(data->dongles);
}