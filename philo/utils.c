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
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			return (-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		res = (res * 10) + (str[i++] - '0');
	if (str[i] && (str[i] < '0' || str[i] > '9'))
		return (-1);
	return (res);
}

long int	ft_max(long int a, long int b)
{
	if (a > b)
		return (a);
	return (b);
}

long int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long int	ft_timestamp(long int start)
{
	struct timeval	tv;
	long int		res;

	gettimeofday(&tv, NULL);
	res = (tv.tv_sec * 1000 + tv.tv_usec / 1000) - start;
	return (res);
}

void	print_msg(t_table *t, int num, t_state state, long int time)
{
	pthread_mutex_lock(&t->status);
	if (t->end && state != E_DIE)
	{
		pthread_mutex_unlock(&t->status);
		return ;
	}
	pthread_mutex_lock(&t->write);
	if (state == E_SLEEP)
		printf("%ld %d is sleeping\n", time, num);
	else if (state == E_FORK)
		printf("%ld %d has taken a fork\n", time, num);
	else if (state == E_EAT)
		printf("%ld %d is eating\n", time, num);
	else if (state == E_THINK)
		printf("%ld %d is thinking\n", time, num);
	else
		printf("%ld %d died\n", time, num);
	pthread_mutex_unlock(&t->write);
	pthread_mutex_unlock(&t->status);
	return ;
}
