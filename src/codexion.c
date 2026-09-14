/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 16:05:03 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/14 15:22:38 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "./codexion.h"


int get_args(int ac, char **av, t_data *data)
{
    int     i;

    if (ac != 9)
    {
        printf("Error: arguments are not valid\n");
        return 0;
    }

    i = 1;
    while (i <= 8)
    {
        // check if the scheduler is valid
        if (i == 8){
          if (strcmp(av[i], "fifo") != 0 && strcmp(av[i], "edf") != 0) {
            printf("Error: invalid scheduler\n");
            return 0;
          }
        }

        // check if the numbers are valid
        else if (!valid_numbers(av[i]))
        {
            printf("Error: invalid numeric argument\n");
            return 0;
        }
        i++;
    }

    data->number_of_coders = ft_atoi(av[1]);
    data->time_to_burnout = ft_atoi(av[2]);
    data->time_to_compile = ft_atoi(av[3]);
    data->time_to_debug = ft_atoi(av[4]);
    data->time_to_refactor = ft_atoi(av[5]);
    data->number_of_compiles = ft_atoi(av[6]);
    data->dongle_cooldown = ft_atoi(av[7]);
    data->scheduler = av[8];

    // check if the values are valid
    if (!valid_values(data))
      return (0);

    return (1);
}


int main(int ac, char **av){
    t_data data;

    if (!get_args(ac, av, &data))
      return (0);

    if (!initializer(&data))
      return (0);

    printf("%s", "Done\n");
    return (1);
}