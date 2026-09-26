/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 17:17:26 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/26 19:54:25 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void *arg)
{
	t_data          *data;
	int             i;
	unsigned long   now;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->scheduler_lock);

		if (data->scheduler_over)
		{
			pthread_mutex_unlock(&data->scheduler_lock);
			break;
		}
        
		now = get_time_ms();
		i = 0;
		while (i < data->number_of_coders)
		{
            if ((data->coders[i].compile_count < data->number_of_compiles) 
            && !data->coders[i].is_compiling
            && now - data->coders[i].last_action_time >= data->time_to_burnout)
			{
                printf("%lu %d burnout\n",
					now - data->start_time, i + 1);
                    data->scheduler_over = 1;
                    pthread_cond_broadcast(&data->scheduler_cond);
                    pthread_mutex_unlock(&data->scheduler_lock);
                    return (NULL);
            }
            i++;
        }

		pthread_mutex_unlock(&data->scheduler_lock);
		usleep(1000);
	}
	return (NULL);
}