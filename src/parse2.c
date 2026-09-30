/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 23:25:13 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/30 23:27:07 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int	valid_args(char **av)
{
	int	i;

	i = 1;
	while (i <= 8)
	{
		if (i == 8)
		{
			if (strcmp(av[i], "fifo") != 0 && strcmp(av[i], "edf") != 0)
				return (1);
		}
		else if (valid_numbers(av[i]))
			return (1);
		i++;
	}
	return (0);
}

void	assign_args(char **av, t_data *data)
{
	data->number_of_coders = ft_atoul(av[1]);
	data->time_to_burnout = ft_atoul(av[2]);
	data->time_to_compile = ft_atoul(av[3]);
	data->time_to_debug = ft_atoul(av[4]);
	data->time_to_refactor = ft_atoul(av[5]);
	data->number_of_compiles = ft_atoul(av[6]);
	data->dongle_cooldown = ft_atoul(av[7]);
	data->scheduler = av[8];
}
