/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 13:54:07 by zahrabar          #+#    #+#             */
/*   Updated: 2026/10/03 18:07:19 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		data->dongles[i].coder_id = -1;
		data->dongles[i].id = i;
		init_heap(&data->dongles[i]);
		pthread_mutex_init(&data->dongles[i].lock, NULL);
		i++;
	}
	return (0);
}

void	init_heap(t_dongle *dongle)
{
	dongle->heap = malloc(sizeof(t_heap));
	if (!dongle->heap)
		return ;
	dongle->heap->items = malloc(sizeof(t_request) * 2);
	if (!dongle->heap->items)
	{
		free(dongle->heap);
		return ;
	}
	dongle->heap->size = 0;
	dongle->heap->capacity = 2;
}

int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i;
		data->coders[i].dongle_1_id = -1;
		data->coders[i].dongle_2_id = -1;
		data->coders[i].data = data;
		data->coders[i].last_action_time = data->start_time;
		data->coders[i].compile_count = 0;
		data->coders[i].is_compiling = 0;
		i++;
	}
	return (0);
}

int	initializer(t_data *data)
{
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (1);
	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
	{
		free(data->coders);
		return (1);
	}
	if (init_dongles(data))
		return (1);
	data->start_time = get_time_ms();
	if (init_coders(data))
		return (1);
	data->scheduler_over = 0;
	data->priority = 0;
	data->coders_finished = 0;
	pthread_mutex_init(&data->scheduler_lock, NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_cond_init(&data->scheduler_cond, NULL);
	create_threads(data);
	join_threads(data);
	cleanup(data);
	return (0);
}
