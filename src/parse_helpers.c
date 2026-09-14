/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 16:05:51 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/12 13:57:43 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int	ft_atoi(const char	*str)
{
	int			i;
	long long	res;
	int			sign;

	i = 0;
	res = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && isdigit((int)str[i]))
	{
		res = res * 10 + (str[i] - 48);
		if (res > 9223372036854775807LL && sign == 1)
			return (0);
		if (res > 9223372036854775807LL && sign == -1)
			return (-1);
		i++;
	}
	return ((int)(res * sign));
}


int valid_values(t_data *data)
{
    if (data->number_of_coders < 1 || data->number_of_coders > 200)
    {
      printf("Error: number of coders is not valid\n");
      return (0);
    }

    if (data->time_to_burnout <= 0 || data->time_to_compile <= 0 || data->time_to_debug <= 0 || data->time_to_refactor <= 0)
    {
      printf("Error: time to burnout, compile, debug, or refactor is not valid (must be greater than 0)\n");
      return (0);
    }
    return (1);
}


int valid_numbers(char *av)
{
    int j;

    j = 0;
    while (av[j]){
      if (av[j] < '0' || av[j] > '9')
        return 0;
      j++;
    }
    return 1;
}
