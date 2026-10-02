/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 23:29:58 by zahrabar          #+#    #+#             */
/*   Updated: 2026/10/02 23:14:07 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

struct timespec	control_time(t_coder *coder)
{
	struct timespec	timeout;

	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += coder->data->time_to_compile / 1000;
	timeout.tv_nsec += (coder->data->time_to_compile % 1000) * 1000000;
	if (timeout.tv_nsec >= 1000000000)
	{
		timeout.tv_sec++;
		timeout.tv_nsec -= 1000000000;
	}
	return (timeout);
}

void	compiling(t_coder *coder)
{
	struct timespec	timeout;

	pthread_mutex_lock(&coder->data->scheduler_lock);
	timeout = control_time(coder);
	coder->last_action_time = get_time_ms();
	coder->is_compiling = 1;
	print_log(coder, " is compiling\n");
	while (!coder->data->scheduler_over
		&& pthread_cond_timedwait(&coder->data->scheduler_cond,
			&coder->data->scheduler_lock, &timeout) != ETIMEDOUT)
	{
	}
	coder->is_compiling = 0;
	coder->compile_count++;
	pthread_mutex_unlock(&coder->data->scheduler_lock);
}

void	work(t_coder *coder)
{
	compiling(coder);
	if (coder->data->scheduler_over)
		return ;
	release_dongles(coder);
	if (coder->data->scheduler_over)
		return ;
	print_log(coder, " is debugging\n");
	usleep(coder->data->time_to_debug * 1000);
	if (coder->data->scheduler_over)
		return ;
	print_log(coder, " is refactoring\n");
	usleep(coder->data->time_to_refactor * 1000);
	if (coder->data->scheduler_over)
		return ;
}
