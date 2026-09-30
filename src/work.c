/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 23:29:58 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/30 23:31:59 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

void	work(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->scheduler_lock);
	coder->last_action_time = get_time_ms();
	coder->is_compiling = 1;
	pthread_mutex_unlock(&coder->data->scheduler_lock);
	print_log(coder, " is compiling\n");
	usleep(coder->data->time_to_compile * 1000);
	pthread_mutex_lock(&coder->data->scheduler_lock);
	coder->is_compiling = 0;
	coder->compile_count++;
	pthread_mutex_unlock(&coder->data->scheduler_lock);
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
