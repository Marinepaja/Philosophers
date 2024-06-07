/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:47:29 by mlaporte          #+#    #+#             */
/*   Updated: 2024/06/07 00:23:10 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int check_death(t_table *t);

void    exit_all(t_table *t)
{
    int     i;

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

void	clear_all(t_table *t)
{
	(void)t;
	return ;
}

long int	ft_max(long int a, long int b)
{
	if (a > b)
		return (a);
	return (b);
}

long int      get_time(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long int ft_timestamp(long int start)
{
    struct timeval  tv;
    long int             res;
    
    gettimeofday(&tv, NULL);
    res =(tv.tv_sec * 1000 + tv.tv_usec / 1000) - start;
    return (res); 
}

void	print_msg(t_table *t, int num, t_state state, long int time)
{
	pthread_mutex_lock(&t->status);
	if (t->end && state != E_DIE)
		{pthread_mutex_unlock(&t->status);
		return ;}
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

int check_death(t_table *t)
{
    int i;

    i = 0;
	pthread_mutex_lock(&t->status);
	if (t->end == 1)
		return (pthread_mutex_unlock(&t->status), EXIT_FAILURE);
	pthread_mutex_unlock(&t->status);

    while (i < t->nb_philo)
	{
		pthread_mutex_lock(&t->philo[i].meal_update);
		pthread_mutex_lock(&t->status);
		if (get_time() - t->philo[i].meal > t->time_to_die  && !t->end)
		{
			pthread_mutex_unlock(&t->status);
			print_msg(t, t->philo[i].num, E_DIE, ft_timestamp(t->start_time));
			pthread_mutex_lock(&t->status);
			t->end = 1;
			pthread_mutex_unlock(&t->status);
			pthread_mutex_unlock(&t->philo[i].meal_update);
			return (EXIT_FAILURE);
		}
		pthread_mutex_unlock(&t->status);
		pthread_mutex_unlock(&t->philo[i].meal_update);
		i++;
	}
	return (EXIT_SUCCESS);
}


void    my_sleep(t_table *t, long int time)
{
    long int    start;

	if (!t)
		return ;
    start = get_time();
    while (!check_death(t) && get_time() - start < time)
        usleep(1000);
}

void    put_forks(t_philo *philo, t_table *table)
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

int    take_forks(t_philo *philo, t_table *table)
{
	if (check_death(table))
		return (EXIT_FAILURE);
    if (table->nb_philo == 1)
        {
			print_msg(table, philo->num, E_THINK, ft_timestamp(table->start_time));
			print_msg(table, philo->num, E_FORK, ft_timestamp(table->start_time));
			my_sleep(table, table->time_to_die + 100);
			table->end = 1;
        return (EXIT_FAILURE);}
    if (philo->num % 2)
    {
        if (!pthread_mutex_lock(philo->right_fork))
			print_msg(table, philo->num, E_FORK, ft_timestamp(table->start_time));
        if (!pthread_mutex_lock(philo->left_fork))
			print_msg(table, philo->num, E_FORK, ft_timestamp(table->start_time));
    }
    else
    {
        if (!pthread_mutex_lock(philo->left_fork))
			print_msg(table, philo->num, E_FORK, ft_timestamp(table->start_time));
        if (!pthread_mutex_lock(philo->right_fork))
			print_msg(table, philo->num, E_FORK, ft_timestamp(table->start_time));
    }
	pthread_mutex_lock(&philo->meal_update);
    philo->meal = get_time();
	pthread_mutex_unlock(&philo->meal_update);
    print_msg(table, philo->num, E_EAT, ft_timestamp(table->start_time));
    return (EXIT_SUCCESS);
}

int     all_full(t_table *t)
{
    int     i;

    i = 0;
	
    while (i < t->nb_philo)
    {
		pthread_mutex_lock(&t->philo[i].meal_update);
        if (t->philo[i].nb_meal < t->max_meal)
            return(pthread_mutex_unlock(&t->philo[i].meal_update), EXIT_FAILURE);
		pthread_mutex_unlock(&t->philo[i].meal_update);
        i++;
    }
    return (EXIT_SUCCESS);
}

void    ft_eat(t_philo *p, t_table *t)
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

void	philo_even(t_philo *tmp)
{
	if (!check_death(tmp->table))
		print_msg(tmp->table, tmp->num, E_THINK, ft_timestamp(tmp->table->start_time));
    my_sleep(tmp->table, tmp->table->time_to_eat);
    ft_eat(tmp, tmp->table);
    if (!all_full(tmp->table))
    {      
	    pthread_mutex_lock(&tmp->table->status);
        tmp->table->end = 1;
        pthread_mutex_unlock(&tmp->table->status);
    }
	if (!check_death(tmp->table))
	{
    	print_msg(tmp->table, tmp->num, E_SLEEP, ft_timestamp(tmp->table->start_time));
        my_sleep(tmp->table, tmp->table->time_to_sleep);
	}
}

void	delay(t_philo *p)
{
	if (p->table->nb_philo % 2)
	{
		if (p->num == p->table->nb_philo)
		{
			print_msg(p->table, p->num, E_THINK, ft_timestamp(p->table->start_time));
			my_sleep(p->table, (p->table->time_to_eat - 1) * 2);
		}
		else if (p->num % 2)
		{
			print_msg(p->table, p->num, E_THINK, ft_timestamp(p->table->start_time));
			my_sleep(p->table, p->table->time_to_eat - 1);
		}
	}
	else
	{
		if (!p->num % 2)
		{
			print_msg(p->table, p->num, E_THINK, ft_timestamp(p->table->start_time));
			my_sleep(p->table, p->table->time_to_eat - 1);
		}
	}
			
}

void *do_philo(void *p)
{
    t_philo *tmp;

    tmp = p;
	delay(tmp);
    while (!check_death(tmp->table))
    {
			ft_eat(tmp, tmp->table);
			if (!all_full(tmp->table))
    	    {
    	        pthread_mutex_lock(&tmp->table->status);
        	    tmp->table->end = 1;
        	    pthread_mutex_unlock(&tmp->table->status);
        	}
			if (!check_death(tmp->table))
			{
    			print_msg(tmp->table, tmp->num, E_SLEEP, ft_timestamp(tmp->table->start_time));
        		my_sleep(tmp->table, tmp->table->time_to_sleep);		
				print_msg(tmp->table, tmp->num, E_THINK, ft_timestamp(tmp->table->start_time));
				if (tmp->table->nb_philo % 2)
					my_sleep(tmp->table, ft_max(0, 2 * tmp->table->time_to_eat - tmp->table->time_to_sleep - 1));
			}
	}
    return (0);
}

t_philo *init_philo(t_table *t)
{
	int		i;
    t_philo *p;

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
		if (pthread_mutex_init(&p[i].meal_update, NULL) ||
		pthread_mutex_init(&p[i].status, NULL))
			return (NULL);
        i++;
    }
    return (p);
    
}

int   init_mutex(t_table *t)
{
    int i;

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


t_table *init_t()
{
    t_table     *t;

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

int ft_thread(t_table *t)
{
    int		i;
	
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
    i = 0;
	while (i < t->nb_philo)
	{
		pthread_join(t->philo[i].thd, NULL);
		i++;
    }
    return(EXIT_SUCCESS);
}

int check_args(int argc, char **argv, t_table *t)
{
    if (argc == 6 && ft_atoi(argv[5]) >= 0)
        t->max_meal = ft_atoi(argv[5]);
    else if (argc == 5)
        t->max_meal = INT_MAX;
    else
        return(EXIT_FAILURE);
    if (ft_atoi(argv[1]) < 0 || ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0 || ft_atoi(argv[4]) < 0)
        return(EXIT_FAILURE);
    t->nb_philo = ft_atoi(argv[1]);
    t->time_to_die = ft_atoi(argv[2]);
    t->time_to_eat = ft_atoi(argv[3]);
    t->time_to_sleep = ft_atoi(argv[4]);
    t->end = 0;
    return(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    t_table     *t;
    
    if (argc < 5 || argc > 6)
        return(printf("Incorrect number of arguments.\n"), EXIT_FAILURE);
    t = init_t();
    if (!t)
        return(EXIT_FAILURE);
    if (check_args(argc, argv, t))
		return(free(t), printf("Arguments must be a numeric and positive value.\n"), EXIT_FAILURE);
    else
	{
        if (t->nb_philo > 200)
		    return(free(t), printf("We should not create more than 200 philosophers\n"), EXIT_FAILURE);
        ft_thread(t);
        exit_all(t);
		free(t);
    }
    return (EXIT_SUCCESS);
}