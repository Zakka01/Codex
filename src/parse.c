/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 16:05:51 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/29 17:33:41 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

unsigned long	ft_atoul(const char *str)
{
	int				i;
	unsigned long	res;

	i = 0;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res > (ULONG_MAX - (str[i] - '0')) / 10)
			return (ULONG_MAX);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res);
}

int valid_values(t_data *data)
{
    if (data->number_of_coders < 1 || data->number_of_coders > 200)
    {
      printf("\033[41mError: number of coders is not valid\033[0m\n");
      return (1);
    }

    if (data->time_to_burnout == 0 || data->time_to_compile == 0 || data->time_to_debug == 0 || data->time_to_refactor == 0)
    {
      printf("\033[41mError: time to burnout, compile, debug, or refactor is not valid (must be greater than 0)\033[0m\n");
      return (1);
    }

    if (data->number_of_compiles == 0)
    {
      printf("\033[41mError: number_of_compiles is not valid\033[0m\n");
      return (1);
    }
    return (0);
}


int valid_numbers(char *av)
{
    int j;

    j = 0;
    while (av[j]){
      if (av[j] < '0' || av[j] > '9')
        return (1);
      j++;
    }
    return (0);
}

int get_args(int ac, char **av, t_data *data)
{
    int     i;

    if (ac != 9)
    {
        printf("\033[41mError: arguments are not valid\033[0m\n");
        return (1);
    }

    i = 1;
    while (i <= 8)
    {
        // check if the scheduler is valid
        if (i == 8){
          if (strcmp(av[i], "fifo") != 0 && strcmp(av[i], "edf") != 0) {
            printf("\033[41mError: invalid scheduler\033[0m\n");
            return (1);
          }
        }

        // check if the numbers are valid
        else if (valid_numbers(av[i]))
        {
            printf("\033[41mError: invalid numeric argument\033[0m\n");
            return (1);
        }
        i++;
    }

    data->number_of_coders = ft_atoul(av[1]);
    data->time_to_burnout = ft_atoul(av[2]);
    data->time_to_compile = ft_atoul(av[3]);
    data->time_to_debug = ft_atoul(av[4]);
    data->time_to_refactor = ft_atoul(av[5]);
    data->number_of_compiles = ft_atoul(av[6]);
    data->dongle_cooldown = ft_atoul(av[7]);
    data->scheduler = av[8];

    if (data->time_to_burnout == ULONG_MAX
      || data->time_to_compile == ULONG_MAX
      || data->time_to_debug == ULONG_MAX
      || data->time_to_refactor == ULONG_MAX
      || data->number_of_compiles == ULONG_MAX
      || data->dongle_cooldown == ULONG_MAX)
  {
      printf("\033[41mError: numeric value is too large\033[0m\n");
      return (1);
  }

    // check if the values are valid
    if (valid_values(data))
      return (1);

    return (0);
}