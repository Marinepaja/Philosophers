/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:08:23 by mlaporte          #+#    #+#             */
/*   Updated: 2024/04/30 14:08:23 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long int	ft_atoi(const char *str)
{
	long int	i;
	long int	res;

	i = 0;
	res = 0;
	// printf("str : %s\n", str);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			return(-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		res = (res * 10) + (str[i++] - '0');
	if (str[i] && (str[i] < '0' || str[i] > '9'))
		return(-1);
	// printf("res : %ld\n", res);
	return (res);
}


int	end_sim(t_table *t)
{
	pthread_mutex_lock(&t->status);
	if (t->end)
	{
		pthread_mutex_unlock(&t->status);
		return(EXIT_FAILURE);
	}
	pthread_mutex_unlock(&t->status);
	return (EXIT_SUCCESS);
}