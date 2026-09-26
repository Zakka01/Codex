/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 15:04:32 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/26 19:56:53 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void	release_dongles(t_coder *coder)
{
	// printf("C%d release: d1=%d d2=%d\n",
	// 	coder->id + 1,
	// 	coder->dongle_1_id + 1,
	// 	coder->dongle_2_id + 1);

	usleep(coder->data->dongle_cooldown * 1000);
	
	pthread_mutex_unlock(&coder->data->dongles[coder->dongle_1_id].lock);
	pthread_mutex_unlock(&coder->data->dongles[coder->dongle_2_id].lock);

	pthread_mutex_lock(&coder->data->scheduler_lock);

	coder->data->dongles[coder->dongle_1_id].coder_id = -1;
    coder->data->dongles[coder->dongle_2_id].coder_id = -1;

	coder->dongle_1_id = -1;
	coder->dongle_2_id = -1;

	pthread_cond_broadcast(&coder->data->scheduler_cond);
	pthread_mutex_unlock(&coder->data->scheduler_lock);
}


int	acquire_dongles(t_coder *coder)
{
	if (coder->data->dongles[coder->id].coder_id != -1 
		|| coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].coder_id != -1)
		return (1);

	pthread_mutex_lock(&coder->data->dongles[coder->id].lock);
	printf("%lu %d has taken a dongle\n", get_time_ms() - coder->data->start_time, coder->id + 1);

	pthread_mutex_lock(&coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].lock);
	printf("%lu %d has taken a dongle\n", get_time_ms() - coder->data->start_time, coder->id + 1);

	coder->dongle_1_id = coder->data->dongles[coder->id].id;
	coder->dongle_2_id = coder->data->dongles[
		(coder->id + 1) % coder->data->number_of_coders].id;

	coder->data->dongles[coder->id].coder_id = coder->id;
	coder->data->dongles[(coder->id + 1) % coder->data->number_of_coders].coder_id = coder->id;

	return (0);
}