/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 23:29:58 by zahrabar          #+#    #+#             */
/*   Updated: 2026/10/05 21:23:23 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

struct timespec	control_time(unsigned long the_time)
{
	struct timespec	timeout;

	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += the_time / 1000;
	timeout.tv_nsec += (the_time % 1000) * 1000000;
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
	timeout = control_time(coder->data->time_to_compile);
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

void	debugging(t_coder *coder)
{
	struct timespec	timeout;

	timeout = control_time(coder->data->time_to_debug);
	print_log(coder, " is debugging\n");
	pthread_mutex_lock(&coder->data->scheduler_lock);
	while (!coder->data->scheduler_over
		&& pthread_cond_timedwait(&coder->data->scheduler_cond,
			&coder->data->scheduler_lock, &timeout) != ETIMEDOUT)
	{
	}
	pthread_mutex_unlock(&coder->data->scheduler_lock);
}

void	refactoring(t_coder *coder)
{
	struct timespec	timeout;

	timeout = control_time(coder->data->time_to_refactor);
	print_log(coder, " is refactoring\n");
	pthread_mutex_lock(&coder->data->scheduler_lock);
	while (!coder->data->scheduler_over
		&& pthread_cond_timedwait(&coder->data->scheduler_cond,
			&coder->data->scheduler_lock, &timeout) != ETIMEDOUT)
	{
	}
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
	debugging(coder);
	if (coder->data->scheduler_over)
		return ;
	refactoring(coder);
	if (coder->data->scheduler_over)
		return ;
}
