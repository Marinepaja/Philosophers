/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 00:23:24 by mlaporte          #+#    #+#             */
/*   Updated: 2024/06/07 00:23:24 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philo	*init_philo(t_table *t)
{
	int		i;
	t_philo	*p;

	i = 0;
	p = malloc(t->nb_philo * sizeof(t_philo));
	if (!p)
		return (NULL);
	while (i < t->nb_philo)
	{
		p[i].num = i + 1;
		p[i].nb_meal = 0;
		p[i].table = t;
		p[i].meal = t->start_time;
		p[i].left_fork = t->forks[i];
		if (t->nb_philo > 1)
			p[i].right_fork = t->forks[(i + 1) % t->nb_philo];
		else
			p[i].right_fork = NULL;
		if (pthread_mutex_init(&p[i].meal_update, NULL)
			|| pthread_mutex_init(&p[i].status, NULL))
			return (NULL);
		i++;
	}
	return (p);
}

int	init_mutex(t_table *t)
{
	int	i;

	i = 0;
	if (!t)
		return (1);
	t->forks = malloc(sizeof(pthread_mutex_t *) * t->nb_philo);
	if (!t->forks)
		return (1);
	while (i < t->nb_philo)
	{
		t->forks[i] = malloc(sizeof(pthread_mutex_t));
		if (!t->forks[i] || pthread_mutex_init(t->forks[i], NULL))
		{
			t->forks[i] = NULL;
			return (EXIT_FAILURE);
		}
		i++;
	}
	pthread_mutex_init(&t->status, NULL);
	pthread_mutex_init(&t->write, NULL);
	pthread_mutex_init(&t->thread, NULL);
	return (EXIT_SUCCESS);
}

t_table	*init_t(void)
{
	t_table	*t;

	t = malloc(sizeof(t_table));
	if (!t)
		return (NULL);
	t->nb_philo = 0;
	t->time_to_die = 0;
	t->time_to_eat = 0;
	t->time_to_sleep = 0;
	t->start_time = get_time();
	t->forks = NULL;
	t->end = 0;
	return (t);
}

int	ft_thread(t_table *t)
{
	int	i;

	i = 0;
	init_mutex(t);
	t->philo = init_philo(t);
	pthread_mutex_lock(&t->thread);
	while (i < t->nb_philo)
	{
		if (pthread_create(&t->philo[i].thd,
				NULL, &do_philo, (void *)&t->philo[i]))
		{
			printf("Error creating pthread %d\n", i);
			pthread_mutex_lock(&t->status);
			t->end = 1;
			pthread_mutex_unlock(&t->status);
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&t->thread);
	i = -1;
	while (++i < t->nb_philo)
		pthread_join(t->philo[i].thd, NULL);
	return (EXIT_SUCCESS);
}
