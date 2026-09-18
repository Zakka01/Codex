/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zahrabar <zahrabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 16:05:03 by zahrabar          #+#    #+#             */
/*   Updated: 2026/09/17 18:37:33 by zahrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./codexion.h"

int main(int ac, char **av){
    t_data data;
    
    if (!get_args(ac, av, &data))
      return (0);
  
    if (!initializer(&data))
      return (0);

    printf("%s", "Done\n");
}