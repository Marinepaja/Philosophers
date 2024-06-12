/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:47:29 by mlaporte          #+#    #+#             */
/*   Updated: 2024/06/11 23:57:10 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// void	delay(t_philo *p)
// {
// 	if (p->table->nb_philo % 2)
// 	{
// 		if (p->num == p->table->nb_philo)
// 		{
// 			print_msg(p->table, p->num, E_THINK,
// 				ft_timestamp(p->table->start_time));
// 			my_sleep(p->table, (p->table->time_to_eat - 1) * 2);
// 		}
// 		else if (p->num % 2)
// 		{
// 			print_msg(p->table, p->num, E_THINK,
// 				ft_timestamp(p->table->start_time));
// 			my_sleep(p->table, p->table->time_to_eat - 1);
// 		}
// 	}
// 	else
// 	{
// 		if (!p->num % 2)
// 		{
// 			print_msg(p->table, p->num, E_THINK,
// 				ft_timestamp(p->table->start_time));
// 			my_sleep(p->table, p->table->time_to_eat - 1);
// 		}
// 	}
// }

// long unsigned int	think_time(t_philo *p)
// {
// 	long int	res;

// 	res = ft_max(0,
// 			2 * p->table->time_to_eat - p->table->time_to_sleep - 1);
// 	return (res);
// }

// void	*do_philo(void *tmp)
// {
// 	t_philo	*p;

// 	p = tmp;
// 	delay(p);
// 	while (!check_death(p->table))
// 	{
// 		ft_eat(p, p->table);
// 		if (!all_full(p->table))
// 		{
// 			pthread_mutex_lock(&p->table->status);
// 			p->table->end = 1;
// 			pthread_mutex_unlock(&p->table->status);
// 		}
// 		if (!check_death(p->table))
// 		{
// 			print_msg(p->table, p->num, E_SLEEP,
// 				ft_timestamp(p->table->start_time));
// 			my_sleep(p->table, p->table->time_to_sleep);
// 			print_msg(p->table, p->num, E_THINK,
// 				ft_timestamp(p->table->start_time));
// 			if (p->table->nb_philo % 2)
// 				my_sleep(p->table, think_time(p));
// 		}
// 	}
// 	return (0);
// }



long long int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// int	check_death(t_table *t)
// {
// 	int	i;

// 	i = 0;
// 	pthread_mutex_lock(&t->status);
// 	if (t->end == 1)
// 		return (pthread_mutex_unlock(&t->status), EXIT_FAILURE);
// 	pthread_mutex_unlock(&t->status);
// 	while (i < t->nb_philo)
// 	{
// 		pthread_mutex_lock(&t->philo[i].meal_update);
// 		pthread_mutex_lock(&t->status);
// 		if (get_time() - t->philo[i].meal > t->time_to_die && !t->end)
// 		{
// 			// death_update(t, i);
// 			return (EXIT_FAILURE);
// 		}
// 		pthread_mutex_unlock(&t->status);
// 		pthread_mutex_unlock(&t->philo[i].meal_update);
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }

int	check_death(t_philo *ph)
{
	int	res;

	pthread_mutex_lock(&ph->table->status);
	if (ph->table->end)
		res = 1;
	else
		res = 0;
	pthread_mutex_unlock(&ph->table->status);
	return (res);
}
// int	my_sleep(t_table *t, long long int time)
// {
// 	long long int	start;

// 	if (!t)
// 		return (EXIT_FAILURE);
// 	start = get_time();
// 	while (!check_death(t) && get_time() - start < time)
// 		usleep(1000);
// 	return (EXIT_SUCCESS);
// }

int	my_sleep(long long int ms, t_philo *ph)
{
	long long int	nttd;
	long long int	new_ms;

	pthread_mutex_lock(&ph->meal_update);
	nttd = ph->table->start_time + (ph->nb_meal + 1) * ph->table->time_to_die;
	pthread_mutex_unlock(&ph->meal_update);
	new_ms = nttd - get_time();
	if (new_ms < 0)
	{
		return (EXIT_FAILURE);
	}
	else if (new_ms > ms)
	{
		if (usleep(ms * 1000) == -1)
			return (EXIT_FAILURE);
	}
	else
	{
		usleep(new_ms * 1000);
		if (check_death(ph))
			return (EXIT_FAILURE);
		usleep((ms - new_ms) * 1000);
	}
	return (EXIT_SUCCESS);
}


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

int	check_args(int argc, char **argv)
{
	if (argc == 6 && ft_atoi(argv[5]) < 0)
		return (EXIT_FAILURE);
	// else if (argc == 5)
	// 	t->max_meal = INT_MAX;
	// else
	// 	return (EXIT_FAILURE);
	if (ft_atoi(argv[1]) < 0 || ft_atoi(argv[2]) < 0
		|| ft_atoi(argv[3]) < 0 || ft_atoi(argv[4]) < 0)
		return (EXIT_FAILURE);
	// t->nb_philo = ft_atoi(argv[1]);
	// t->time_to_die = ft_atoi(argv[2]);
	// t->time_to_eat = ft_atoi(argv[3]);
	// t->time_to_sleep = ft_atoi(argv[4]);
	// t->end = 0;
	return (EXIT_SUCCESS);
}

int	init_philo(t_table *t)
{
	int		i;
	// t_philo	*p;

	i = 0;
	// p = malloc(t->nb_philo * sizeof(t_philo));
	// if (!p)
	// 	return (NULL);
	while (i < t->nb_philo)
	{
		t->philo[i].num = i + 1;
		t->philo[i].nb_meal = 0;
		t->philo[i].table = t;
		t->philo[i].meal = get_time();
		t->philo[i].left_fork = &t->forks[i];
		if (t->nb_philo > 1)
			t->philo[i].right_fork = &t->forks[(i + 1) % t->nb_philo];
		else
			t->philo[i].right_fork = NULL;
		if (pthread_mutex_init(&t->philo[i].meal_update, NULL)
			|| pthread_mutex_init(&t->philo[i].status, NULL))
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	init_t(t_table *t, char **argv, int argc)
{
	// t = malloc(sizeof(t_table));
	// if (!t)
	// 	return (EXIT_FAILURE);
	t->nb_philo = ft_atoi(argv[1]);
	// printf("nb : %d\n", t->nb_philo);
	t->time_to_die = ft_atoi(argv[2]);
	t->time_to_eat = ft_atoi(argv[3]);
	t->time_to_sleep = ft_atoi(argv[4]);
	t->thread = malloc(sizeof(pthread_t) * t->nb_philo);
	t->forks = malloc(sizeof(pthread_mutex_t) * t->nb_philo);
	if (!t->forks)
		return (1);
	t->philo = malloc(sizeof(t_philo) * t->nb_philo);
	if (!t->philo)
		return (EXIT_FAILURE);
	if (argc == 6)
		t->max_meal = ft_atoi(argv[5]);
	else
		t->max_meal = INT_MAX;
	t->end = 0;
	return (EXIT_SUCCESS);
}

void	print_msg(t_table *t, int num, t_state state, long int time)
{
	pthread_mutex_lock(&t->write);
	// pthread_mutex_lock(&t->status);
	
	if (state == E_DIE)
	// {
		printf("%ld %d died\n", time, num);
	// 	pthread_mutex_unlock(&t->status);
	// 	pthread_mutex_unlock(&t->write);
	// 	t->end = 1;
	// 	return ;
	// }
	
	// if (t->end && state != E_DIE)
	// {
	// 	pthread_mutex_unlock(&t->status);
	// 	pthread_mutex_unlock(&t->write);
	// 	return ;
	// }
	// pthread_mutex_unlock(&t->status);
	// pthread_mutex_lock(&t->write);
	if (state == E_SLEEP)
		printf("%ld %d is sleeping\n", time, num);
	else if (state == E_FORK)
		printf("%ld %d has taken a fork\n", time, num);
	else if (state == E_EAT)
		printf("%ld %d is eating\n", time, num);
	else if (state == E_THINK)
		printf("%ld %d is thinking\n", time, num);

	// pthread_mutex_unlock(&t->status);
	pthread_mutex_unlock(&t->write);
	// pthread_mutex_unlock(&t->status);
	return ;
}
int	init_mutex(t_table *t)
{
	int	i;

	i = 0;
	if (!t)
		return (1);
	// t->forks = malloc(sizeof(pthread_mutex_t *) * t->nb_philo);
	// if (!t->forks)
	// 	return (1);
	while (i < t->nb_philo)
	{
		// t->forks[i] = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(&t->forks[i], NULL))
		{
			// t->forks[i] = NULL;
			return (EXIT_FAILURE);
		}
		i++;
	}
	pthread_mutex_init(&t->status, NULL);
	pthread_mutex_init(&t->write, NULL);
	// pthread_mutex_init(&t->thread, NULL);
	return (EXIT_SUCCESS);
}

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
			get_time() - table->start_time);
	if (!pthread_mutex_lock(philo->left_fork))
		print_msg(table, philo->num, E_FORK,
			get_time() - table->start_time);
}

void	even_philo(t_philo *philo, t_table *table)
{
	if (!pthread_mutex_lock(philo->left_fork))
		print_msg(table, philo->num, E_FORK,
			get_time() - table->start_time);
	if (!pthread_mutex_lock(philo->right_fork))
		print_msg(table, philo->num, E_FORK,
			get_time() - table->start_time);
}

int	take_forks(t_philo *philo, t_table *table)
{
	if (check_death(philo))
		return (EXIT_FAILURE);
	if (table->nb_philo == 1)
	{
		print_msg(table, philo->num, E_THINK, get_time() - table->start_time);
		print_msg(table, philo->num, E_FORK, get_time() - table->start_time);
		my_sleep(table->time_to_die + 100, philo);
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
	print_msg(table, philo->num, E_EAT, get_time() - table->start_time);
	return (EXIT_SUCCESS);
}

int	philo_forks(t_philo *ph)
{
	if (ph->num % 2 == 0)
		pthread_mutex_lock(ph->right_fork);
	else
		pthread_mutex_lock(ph->left_fork);
	if (!check_death(ph))
		print_msg(ph->table, ph->num, E_FORK, get_time() - ph->table->start_time);
	if (ph->num % 2 == 0)
	{
		if (pthread_mutex_lock(ph->left_fork) != 0)
			return (pthread_mutex_unlock(ph->right_fork), 1);
		if (!check_death(ph))
			print_msg(ph->table, ph->num, E_FORK, get_time() - ph->table->start_time);
	}
	else
	{
		if (pthread_mutex_lock(ph->right_fork) != 0)
			return (pthread_mutex_unlock(ph->left_fork), 1);
		if (!check_death(ph))
			print_msg(ph->table, ph->num, E_FORK, get_time() - ph->table->start_time);
	}
	return (0);
}

int	eat_sleep(t_philo *ph)
{
	if (check_death(ph))
	{
		pthread_mutex_unlock(ph->left_fork);
		pthread_mutex_unlock(ph->right_fork);
		pthread_mutex_unlock(&ph->status);
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&ph->status);
	if (my_sleep(ph->table->time_to_eat, ph))
	{
		pthread_mutex_unlock(ph->left_fork);
		pthread_mutex_unlock(ph->right_fork);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_eat(t_philo *p, t_table *t)
{
	// sleep(2);
	// printf("eating\n");
	if (check_death(p))
		return (EXIT_FAILURE);
	// printf("here\n");
	if (philo_forks(p))
		return (EXIT_FAILURE);
	if (check_death(p))
	{
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		return (EXIT_FAILURE);
	}
	
	print_msg(t, p->num, E_EAT, get_time() - t->start_time);
	// pthread_mutex_lock(&p->table->write);
	// printf("%lld %d is eating\n", get_time() - p->data->t0, ph->id);
	// pthread_mutex_unlock(&ph->data->mprint);
	pthread_mutex_lock(&p->status);
	p->meal = get_time();
	if (eat_sleep(p))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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



long int	ft_max(long int a, long int b)
{
	if (a > b)
		return (a);
	return (b);
}

long unsigned int	ft_think(t_philo *p)
{

	long long int	time_to_think;

	if (p->table->nb_philo % 2 == 0)
		time_to_think = (p->table->time_to_eat - p->table->time_to_sleep);
	else
		time_to_think = (2 * p->table->time_to_eat - p->table->time_to_sleep);
	if (time_to_think < 0)
		time_to_think = 0;
	print_msg(p->table, p->num, E_THINK,
		get_time() - p->table->start_time);
	// pthread_mutex_lock(&p->table->write);
	// printf("%lld %d is thinking\n", get_time() - p->table->start_time, p->num);
	// pthread_mutex_unlock(&p->table->write);
	if (my_sleep(time_to_think, p))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);

	// long int	res;

	// if (p->table->nb_philo % 2)
	// 	res = ft_max(0, 2 *p->table->time_to_eat - p->table->time_to_sleep);
	// else
	// 	res = ft_max(0, p->table->time_to_eat - p->table->time_to_sleep );
	// print_msg(p->table, p->num, E_THINK,
	// 	get_time() - p->table->start_time);
	// printf("ttt%lu\n", res);
	// my_sleep(res, p);
	// return (EXIT_SUCCESS);
}

int	routine(t_philo *p)
{
	// printf("starting %d\n", p->num);
	if (ft_eat(p, p->table))
		return (EXIT_FAILURE);
	// if (!all_full(p->table))
	// {
	// 	pthread_mutex_lock(&p->table->status);
	// 	p->table->end = 1;
	// 	pthread_mutex_unlock(&p->table->status);
	// }
	//printf("%d\n", check_death(p));
	pthread_mutex_lock(&p->meal_update);
	p->nb_meal++;
	// pthread_mutex_unlock(&p->meal_update);
	
	if (p->nb_meal >= p->table->max_meal && p->table->max_meal != INT_MAX)
	{
		pthread_mutex_unlock(&p->meal_update);
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&p->meal_update);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	if (check_death(p))
		return (EXIT_FAILURE);
	print_msg(p->table, p->num, E_SLEEP,
		get_time() - p->table->start_time);
	if (my_sleep(p->table->time_to_sleep, p))
		return (EXIT_FAILURE);
	if (check_death(p) || ft_think(p))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


void	*do_philo(void *tmp)
{
	t_philo	*p;

	p = tmp;
	// printf("t0 : %lu\n", p->table->start_time);
	// printf("t0 : %lu\n", get_time());
	if (p->table->nb_philo == 1)
		return (print_msg(p->table, p->num, E_FORK, get_time() - p->table->start_time), NULL);
	// printf("num : %llu\n", get_time() - p->table->start_time);
	if (!p->num % 2)
		if (my_sleep(p->table->time_to_eat, p))
			return (NULL);
	// /*delay(p);
	while (1)
	{
		// printf("hey I'm %d\n", p->num);
		if (routine(p))
			return (NULL);
		// ft_eat(p, p->table);
		// if (!all_full(p->table))
		// {
		// 	pthread_mutex_lock(&p->table->status);
		// 	p->table->end = 1;
		// 	pthread_mutex_unlock(&p->table->status);
		// }
		// if (!check_death(p->table))
		// {
		// 	print_msg(p->table, p->num, E_SLEEP,
		// 		ft_timestamp(p->table->start_time));
		// 	my_sleep(p->table, p->table->time_to_sleep);
		// 	print_msg(p->table, p->num, E_THINK,
		// 		ft_timestamp(p->table->start_time));
		// 	if (p->table->nb_philo % 2)
		// 		my_sleep(p->table, think_time(p));
		// }
	}
	// printf("eot\n");
	return (0);
}

int	test_dead(t_table *t, int i)
{
	
	pthread_mutex_lock(&t->philo[i].status);
	
	if (get_time() - t->philo[i].meal >= t->time_to_die)
	{
		// printf("ttd : %llu\n", t->time_to_die);
		// // printf("ttd : %llu\n", get_time() - t->philo[i].meal);
		pthread_mutex_unlock(&t->philo[i].status);
		pthread_mutex_lock(&t->status);
		t->end = 1;
		pthread_mutex_unlock(&t->status);
		print_msg(t, t->philo[i].num, E_DIE, get_time() - t->start_time);
		// pthread_mutex_lock(&t->write);
		// printf("%lld %d died\n", get_time() - t->start_time,
		// 	t->philo[i].num);
		// pthread_mutex_unlock(&t->write);
		
		return (1);
	}
	// printf("here\n");
	pthread_mutex_unlock(&t->philo[i].status);
	return (0);
}

void	is_dead(t_table *t)
{
	int	i;

	i = 0;
	while (1)
	{
		usleep(100);
		i = 0;
		// printf("max_meal : %d\n", t->max_meal);
		while (i < t->nb_philo)
		{
			pthread_mutex_lock(&t->philo[i].meal_update);
			if (t->philo[i].nb_meal >= t->max_meal)
			{
				pthread_mutex_unlock(&t->philo[i].meal_update);
				return ;
			}
			pthread_mutex_unlock(&t->philo[i].meal_update);
			if (test_dead(t, i))
				return ;
			i++;
		}
	}
	return ;
}


int	ft_create_thd(t_table *t)
{
	int	i;

	i = 0;
	t->start_time = get_time();

	// printf("num : %llu\n", t->start_time);
	while (i < t->nb_philo)
	{
		if (pthread_create(&t->thread[i],
				NULL, &do_philo, &t->philo[i]))
		{
			printf("Error creating pthread %d\n", i);
			// pthread_mutex_lock(&t->status);
			// t->end = 1;
			// pthread_mutex_unlock(&t->status);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

void	free_data(t_table *data)
{
	if (data->thread)
		free(data->thread);
	if (data->forks)
		free(data->forks);
	if (data->philo)
		free(data->philo);
}

void	ft_destroy(t_table *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philo[i].status);
		pthread_mutex_destroy(&data->philo[i].meal_update);
		i++;
	}
	pthread_mutex_destroy(&data->status);
	pthread_mutex_destroy(&data->write);
}

int	ft_thread(t_table *t)
{
	int	i;

	// printf("coucou\n");
	if (ft_create_thd(t))
		return (free_data(t), EXIT_FAILURE);
	is_dead(t);
	// t->philo = init_philo(t);
	// pthread_mutex_lock(&t->thread)free_data(&t),;
	// printf("thread created\n");
	// sleep(5);
	// pthread_mutex_unlock(&t->thread);
	// t->start_time = get_time();
	i = -1;
	while (++i < t->nb_philo)
		pthread_join(t->thread[i], NULL);
	ft_destroy(t);
	free_data(t);
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_table	t;

	if (argc < 5 || argc > 6)
		return (printf("Incorrect number of arguments.\n"), EXIT_FAILURE);
	// t = init_t();
	// if (!t)
		// return (EXIT_FAILURE);
	if (check_args(argc, argv))
		return (printf("Arguments must be a numeric and positive value.\n"),
			EXIT_FAILURE);
	if (ft_atoi(argv[1]) > 200)
			return (printf("We should not create more than 200 philosophers\n"),
				EXIT_FAILURE);
	if (init_t(&t, argv, argc))
		return (free_data(&t), EXIT_FAILURE);
	// printf("num main %d\n", t.nb_philo);
	if (init_philo(&t))
		return (free_data(&t), EXIT_FAILURE);
	if (init_mutex(&t))
		return (free_data(&t), EXIT_FAILURE);
	if (ft_thread(&t))
		return (EXIT_FAILURE);
	// printf("finished\n");
	
		// ft_thread(t);
		// exit_all(t);
		// free(t);

	return (EXIT_SUCCESS);
}
