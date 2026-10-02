/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 17:17:26 by zahrabar          #+#    #+#             */
/*   Updated: 2026/10/02 23:16:30 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void	burnout_loop(t_data *data, unsigned long now)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if ((data->coders[i].compile_count < data->number_of_compiles)
			&& data->coders[i].is_compiling == 0
			&& now - data->coders[i].last_action_time
			>= data->time_to_burnout)
		{
			print_log(&data->coders[i], " burnout\n");
			data->scheduler_over = 1;
			pthread_cond_broadcast(&data->scheduler_cond);
			pthread_mutex_unlock(&data->scheduler_lock);
			return ;
		}
		i++;
	}
}

void	*monitor_routine(void *arg)
{
	t_data			*data;
	unsigned long	now;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->scheduler_lock);
		if (data->scheduler_over)
		{
			pthread_mutex_unlock(&data->scheduler_lock);
			break ;
		}
		now = get_time_ms();
		burnout_loop(data, now);
		pthread_mutex_unlock(&data->scheduler_lock);
		usleep(1000);
	}
	return (NULL);
}
