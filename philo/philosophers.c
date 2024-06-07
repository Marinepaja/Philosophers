/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:47:29 by mlaporte          #+#    #+#             */
/*   Updated: 2024/06/07 10:40:09 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	delay(t_philo *p)
{
	if (p->table->nb_philo % 2)
	{
		if (p->num == p->table->nb_philo)
		{
			print_msg(p->table, p->num, E_THINK,
				ft_timestamp(p->table->start_time));
			my_sleep(p->table, (p->table->time_to_eat - 1) * 2);
		}
		else if (p->num % 2)
		{
			print_msg(p->table, p->num, E_THINK,
				ft_timestamp(p->table->start_time));
			my_sleep(p->table, p->table->time_to_eat - 1);
		}
	}
	else
	{
		if (!p->num % 2)
		{
			print_msg(p->table, p->num, E_THINK,
				ft_timestamp(p->table->start_time));
			my_sleep(p->table, p->table->time_to_eat - 1);
		}
	}
}

long unsigned int	think_time(t_philo *p)
{
	long int	res;

	res = ft_max(0,
			2 * p->table->time_to_eat - p->table->time_to_sleep - 1);
	return (res);
}

void	*do_philo(void *tmp)
{
	t_philo	*p;

	p = tmp;
	delay(p);
	while (!check_death(p->table))
	{
		ft_eat(p, p->table);
		if (!all_full(p->table))
		{
			pthread_mutex_lock(&p->table->status);
			p->table->end = 1;
			pthread_mutex_unlock(&p->table->status);
		}
		if (!check_death(p->table))
		{
			print_msg(p->table, p->num, E_SLEEP,
				ft_timestamp(p->table->start_time));
			my_sleep(p->table, p->table->time_to_sleep);
			print_msg(p->table, p->num, E_THINK,
				ft_timestamp(p->table->start_time));
			if (p->table->nb_philo % 2)
				my_sleep(p->table, think_time(p));
		}
	}
	return (0);
}

int	check_args(int argc, char **argv, t_table *t)
{
	if (argc == 6 && ft_atoi(argv[5]) >= 0)
		t->max_meal = ft_atoi(argv[5]);
	else if (argc == 5)
		t->max_meal = INT_MAX;
	else
		return (EXIT_FAILURE);
	if (ft_atoi(argv[1]) < 0 || ft_atoi(argv[2]) < 0
		|| ft_atoi(argv[3]) < 0 || ft_atoi(argv[4]) < 0)
		return (EXIT_FAILURE);
	t->nb_philo = ft_atoi(argv[1]);
	t->time_to_die = ft_atoi(argv[2]);
	t->time_to_eat = ft_atoi(argv[3]);
	t->time_to_sleep = ft_atoi(argv[4]);
	t->end = 0;
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_table	*t;

	if (argc < 5 || argc > 6)
		return (printf("Incorrect number of arguments.\n"), EXIT_FAILURE);
	t = init_t();
	if (!t)
		return (EXIT_FAILURE);
	if (check_args(argc, argv, t))
		return (free(t),
			printf("Arguments must be a numeric and positive value.\n"),
			EXIT_FAILURE);
	else
	{
		if (t->nb_philo > 200)
			return (free(t),
				printf("We should not create more than 200 philosophers\n"),
				EXIT_FAILURE);
		ft_thread(t);
		exit_all(t);
		free(t);
	}
	return (EXIT_SUCCESS);
}
