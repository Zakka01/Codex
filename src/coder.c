/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:47:29 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/16 15:04:57 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"
#include <stdio.h>


void *coder_routine(void *arg)
{
    t_coder *coder;
    int     res;

    coder = (t_coder *)arg;

    res = acquire_dongles(coder);
    if (res == 1)
    {
        printf("0 %d is compiling\n", coder->id + 1);
        usleep(coder->data->time_to_compile * 1000);

        release_dongles(coder);

        printf("0 %d is debugging\n", coder->id + 1);
        usleep(coder->data->time_to_debug * 1000);
        printf("0 %d is refactoring\n", coder->id + 1);
        usleep(coder->data->time_to_refactor * 1000);
    }
    else {
        append_queue(coder);
    }

    return (NULL);
}