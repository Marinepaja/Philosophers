/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:12:44 by mlaporte          #+#    #+#             */
/*   Updated: 2024/06/07 10:44:45 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	put_forks(t_philo *philo, t_table *table)
{
	if (table->nb_philo == 1)
		return ;
	if (philo->num % 2)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}

void	uneven_philo(t_philo *philo, t_table *table)
{
	if (!pthread_mutex_lock(philo->right_fork))
		print_msg(table, philo->num, E_FORK,
			ft_timestamp(table->start_time));
	if (!pthread_mutex_lock(philo->left_fork))
		print_msg(table, philo->num, E_FORK,
			ft_timestamp(table->start_time));
}

void	even_philo(t_philo *philo, t_table *table)
{
	if (!pthread_mutex_lock(philo->left_fork))
		print_msg(table, philo->num, E_FORK,
			ft_timestamp(table->start_time));
	if (!pthread_mutex_lock(philo->right_fork))
		print_msg(table, philo->num, E_FORK,
			ft_timestamp(table->start_time));
}

int	take_forks(t_philo *philo, t_table *table)
{
	if (check_death(table))
		return (EXIT_FAILURE);
	if (table->nb_philo == 1)
	{
		print_msg(table, philo->num, E_THINK, ft_timestamp(table->start_time));
		print_msg(table, philo->num, E_FORK, ft_timestamp(table->start_time));
		my_sleep(table, table->time_to_die + 100);
		table->end = 1;
		return (EXIT_FAILURE);
	}
	if (philo->num % 2)
		uneven_philo(philo, table);
	else
		even_philo(philo, table);
	pthread_mutex_lock(&philo->meal_update);
	philo->meal = get_time();
	pthread_mutex_unlock(&philo->meal_update);
	print_msg(table, philo->num, E_EAT, ft_timestamp(table->start_time));
	return (EXIT_SUCCESS);
}

void	ft_eat(t_philo *p, t_table *t)
{
	if (check_death(t))
		return ;
	if (p->nb_meal < t->max_meal)
	{
		take_forks(p, t);
		my_sleep(t, t->time_to_eat);
		put_forks(p, t);
		pthread_mutex_lock(&p->meal_update);
		p->nb_meal += 1;
		pthread_mutex_unlock(&p->meal_update);
	}
}
