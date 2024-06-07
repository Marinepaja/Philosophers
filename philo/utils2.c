/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:15:16 by mlaporte          #+#    #+#             */
/*   Updated: 2024/06/07 10:47:17 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	exit_all(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->nb_philo)
	{
		pthread_mutex_destroy(t->forks[i]);
		free(t->forks[i]);
		pthread_mutex_destroy(&t->philo[i].status);
		pthread_mutex_destroy(&t->philo[i].meal_update);
		i++;
	}
	free(t->philo);
	free(t->forks);
}

int	all_full(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->nb_philo)
	{
		pthread_mutex_lock(&t->philo[i].meal_update);
		if (t->philo[i].nb_meal < t->max_meal)
			return (pthread_mutex_unlock(&t->philo[i].meal_update),
				EXIT_FAILURE);
		pthread_mutex_unlock(&t->philo[i].meal_update);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	death_update(t_table *t, int i)
{
	pthread_mutex_unlock(&t->status);
	print_msg(t, t->philo[i].num, E_DIE, ft_timestamp(t->start_time));
	pthread_mutex_lock(&t->status);
	t->end = 1;
	pthread_mutex_unlock(&t->status);
	pthread_mutex_unlock(&t->philo[i].meal_update);
}

int	check_death(t_table *t)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&t->status);
	if (t->end == 1)
		return (pthread_mutex_unlock(&t->status), EXIT_FAILURE);
	pthread_mutex_unlock(&t->status);
	while (i < t->nb_philo)
	{
		pthread_mutex_lock(&t->philo[i].meal_update);
		pthread_mutex_lock(&t->status);
		if (get_time() - t->philo[i].meal > t->time_to_die && !t->end)
		{
			death_update(t, i);
			return (EXIT_FAILURE);
		}
		pthread_mutex_unlock(&t->status);
		pthread_mutex_unlock(&t->philo[i].meal_update);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	my_sleep(t_table *t, long int time)
{
	long int	start;

	if (!t)
		return ;
	start = get_time();
	while (!check_death(t) && get_time() - start < time)
		usleep(1000);
}
