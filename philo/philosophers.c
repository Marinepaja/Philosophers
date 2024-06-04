/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:47:29 by mlaporte          #+#    #+#             */
/*   Updated: 2024/06/04 15:02:04 by mlaporte         ###   ########.fr       */
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
        pthread_detach(t->philo[i].thd);
        pthread_mutex_destroy(t->forks[i]);
        free(t->forks[i]);
        pthread_mutex_destroy(&t->philo[i].status);
        pthread_mutex_destroy(&t->philo[i].meal_update);
        //free(&t->philo[i]);
        i++;
    }
    
    free(t->philo);
    free(t->forks);
    // free(t);
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
    // long int        res;

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
	// printf("end : %d\n", t->end);
	// printf("death : %d\n", check_death(t));	
	if (t->end && state != E_DIE)
		return ;
	pthread_mutex_lock(&t->write);
	// printf("state :%d\n", state);
	if (state == E_SLEEP)
		printf("%ld %d is sleeping\n", time, num);
	else if (state == E_FORK)
		printf("%ld %d has taken a fork\n", time, num);
	else if (state == E_EAT)
		printf("%ld %d is eating\n", time, num);
	else if (state == E_THINK)
		printf("%ld %d is thinking\n", time, num);
	else 
	{//printf("dying");
		printf("%ld %d died\n", time, num);}
	pthread_mutex_unlock(&t->write);
	return ;
}

int check_death(t_table *t)
{
    int i;

    i = 0;
	if (t->end == 1)
		return (EXIT_FAILURE);
    while (i < t->nb_philo)
	{
		pthread_mutex_lock(&t->philo[i].status);
		// printf("end = %d\n", t->end);
		if (get_time() - t->philo[i].meal > t->time_to_die  && !t->end)
		{
			print_msg(t, t->philo[i].num, E_DIE, ft_timestamp(t->start_time));
			// printf("%lu %d died\n", ft_timestamp(t->start_time), t->philo[i].num);
			// printf("last meal %lu\n", get_time() - t->philo[i].meal);
			t->end = 1;
			pthread_mutex_unlock(&t->philo[i].status);
			// clear_all(t);
			return (EXIT_FAILURE);
		}
		pthread_mutex_unlock(&t->philo[i].status);
		i++;
	}
	return (EXIT_SUCCESS);
}


void    my_sleep(t_table *t, long int time)
{
    long int    start;

    start = get_time();
    while (!check_death(t) && get_time() - start < time)
        usleep(1000);
}

void    put_forks(t_philo *philo, t_table *table)
{
    if (table->nb_philo == 1)
        {        return;}
    
    //  printf("left fork : %p\n", philo->right_fork);
    if (philo->num % 2)
    {
    pthread_mutex_unlock(philo->left_fork);
        // printf("right back");
    pthread_mutex_unlock(philo->right_fork);
    }
    else
    {
    pthread_mutex_unlock(philo->right_fork);
        // printf("right back");
    pthread_mutex_unlock(philo->left_fork);
    }
        //printf("left back");
    // usleep(table->time_to_eat);
    // pthread_mutex_unlock(philo->left_fork);
    
}

/*int    take_forks(t_philo *philo, t_table *table)
{
	if (check_death(table))
		return (EXIT_FAILURE);
    if (table->nb_philo == 1)
        {
			printf("%lu %d is thinking\n", ft_timestamp(table->start_time), philo->num);
			// my_sleep(table, table->time_to_eat);
			printf("%lu %d has taken a fork\n", ft_timestamp(table->start_time), philo->num);
			my_sleep(table, table->time_to_die + 100);
			// printf("%lu %d died\n",ft_timestamp(table->start_time), philo->num);
			table->end = 1;
        return (EXIT_FAILURE);}
    //  printf("left fork : %p\n", philo->right_fork);
    if (pthread_mutex_lock(philo->right_fork) || pthread_mutex_lock(philo->left_fork))
        return (EXIT_FAILURE) ;
    else if (!check_death(table))
    {
        printf("%lu %d has taken a fork\n", ft_timestamp(table->start_time), philo->num);
        printf("%lu %d has taken a fork\n", ft_timestamp(table->start_time), philo->num);
        philo->meal = get_time();
        printf("%lu %d is eating\n", ft_timestamp(table->start_time), philo->num);
    }
	return (EXIT_SUCCESS);
    // usleep(table->time_to_eat);
    // pthread_mutex_unlock(philo->left_fork);
    
}*/

int    take_forks(t_philo *philo, t_table *table)
{
	if (check_death(table))
		return (EXIT_FAILURE);
    if (table->nb_philo == 1)
        {
			print_msg(table, philo->num, E_THINK, ft_timestamp(table->start_time));
			// my_sleep(table, table->time_to_eat);
			print_msg(table, philo->num, E_FORK, ft_timestamp(table->start_time));
			my_sleep(table, table->time_to_die + 100);
			// printf("%lu %d died\n",ft_timestamp(table->start_time), philo->num);
			table->end = 1;
        return (EXIT_FAILURE);}
    //  printf("left fork : %p\n", philo->right_fork);
    if (philo->num % 2)
    {
        pthread_mutex_lock(philo->right_fork);
        pthread_mutex_lock(philo->left_fork);
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(philo->right_fork);
    }
    {
        print_msg(table, philo->num, E_FORK, ft_timestamp(table->start_time));
        print_msg(table, philo->num, E_FORK, ft_timestamp(table->start_time));
		pthread_mutex_lock(&philo->meal_update);
        philo->meal = get_time();
		pthread_mutex_unlock(&philo->meal_update);
        print_msg(table, philo->num, E_EAT, ft_timestamp(table->start_time));
    }
	return (EXIT_SUCCESS);
}

int     all_full(t_table *t)
{
    int     i;

    i = 0;
    while (i < t->nb_philo)
    {
        if (t->philo[i].nb_meal < t->max_meal)
            return(EXIT_FAILURE);
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
        pthread_mutex_lock(&p->status);
        p->nb_meal += 1;
        if (!all_full(t))
        {
            pthread_mutex_lock(&t->status);
            t->end = 1;
            pthread_mutex_unlock(&t->status);
        }
        pthread_mutex_unlock(&p->status);
		if (!check_death(t))
		{
    	print_msg(t, p->num, E_SLEEP, ft_timestamp(t->start_time));}
	}
    // printf("ti;e to eat %lu\n", t->time_to_eat);
    
    // printf("after eat time : %ld\n", ft_timestamp(t->start_time));
    
}

void *do_philo(void *p)
{
    t_philo *tmp;
	// long int think;

    // think = 0;
    tmp = p;
    
    // printf("hello, %d\n", tmp->num);
	// think = ft_max(0, tmp->table->time_to_eat * 2 - tmp->table->time_to_sleep - 1);
    while (!check_death(tmp->table))
    {
    if (tmp->num % 2 == 1)
    {
        // printf("coucou");
        ft_eat(tmp, tmp->table);
        my_sleep(tmp->table, tmp->table->time_to_sleep);			

		if (!check_death(tmp->table))
		{
			print_msg(tmp->table, tmp->num, E_THINK, ft_timestamp(tmp->table->start_time));
        	// printf("%lu %d is thinking\n", ft_timestamp(tmp->table->start_time), tmp->num);
		}
					my_sleep(tmp->table, 0);
        
    }
    else
    {
		if (!check_death(tmp->table))
			print_msg(tmp->table, tmp->num, E_THINK, ft_timestamp(tmp->table->start_time));
        	// printf("%lu %d is thinking\n", ft_timestamp(tmp->table->start_time), tmp->num);
        my_sleep(tmp->table, 5);
        ft_eat(tmp, tmp->table);
        my_sleep(tmp->table, tmp->table->time_to_sleep);
    }
    
    
    // printf("hello, %d\n", tmp->num);
    if (tmp->table->nb_philo == 20)
        tmp->table->end = 1;
        
    }
    tmp->table->end = 1;
    return (0);
}

t_philo *init_philo(t_table *t)
{
     int i;
    t_philo *p;

    i = 0;
    
    
    p = malloc(t->nb_philo * sizeof(t_philo));
    if (!p)
        return (NULL);
    while (i < t->nb_philo)
    {
        p[i].num = i + 1;
        p[i].nb_meal = 0;
        // p = &t->philo[i];
        // printf("philo : %d\n", p[i].num);
		pthread_mutex_init(&p[i].meal_update, NULL);
        pthread_mutex_init(&p[i].status, NULL);
        p[i].table = t;
        p[i].meal = t->start_time;
        p[i].left_fork = t->forks[i];
        p[i].right_fork = t->forks[(i + 1) % t->nb_philo];
        // printf("left r : %p\n", p[i].right_fork);
        // printf("left f : %p\n", p[i].left_fork);
        //  printf("num thread : %d, %d\n", p->num, t->philo->num)
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
			printf("failed");
			t->forks[i] = NULL;
			return (EXIT_FAILURE);
		}
        i++;
    }
	pthread_mutex_init(&t->status, NULL);
    pthread_mutex_init(&t->write, NULL);
	
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

    // printf("end : %d\n", t->end);


    // t->threads = malloc(t->nb_philo *sizeof(pthread_t));
   
    // printf("%d\n", t->nb_philo);

    
    return (t);
}

int ft_thread(t_table *t)
{
    int i = 0;
    // t_philo    *p;
    
    init_mutex(t);
    t->philo = init_philo(t);// p = malloc(t->nb_philo * sizeof(t_philo));
    // p = NULL;
    // (void)p;
    while (i < t->nb_philo)
    {
        // p = &t->philo[i];

        // printf("%d\n", p->  num);
        // p->table = t;
    
        // p[i].num = i;
        // t->philo[i].left_fork = &t->forks[i];
        // t->philo[i].right_fork = &t->forks[(i + 1) % t->nb_philo];
        // printf("num : %d, i= %d, nb = %d\n", p->num, i, t->nb_philo);
        if (pthread_create(&t->philo[i].thd,
				NULL, &do_philo, (void *)&t->philo[i]))
        {
            printf("Error creating pthread %d\n", i);
            pthread_mutex_lock(&t->status);
            t->end = 1;
            pthread_mutex_unlock(&t->status);
        }
        // pthread_join(p->thd, NULL);
        //  printf("num thread : %d, %d\n", p->num, t->philo->num);
        i++;}
        i = 0;
	while (!check_death(t) && i < t->nb_philo)
	{
		if (pthread_join(t->philo[i].thd, NULL))
			printf("noooo");
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
    // printf("atoi : %d\n", t->nb_philo);
    t->time_to_die = ft_atoi(argv[2]);

    // printf("t : %ld, %d\n", t->time_to_die, t->time_to_die <= 0);
    t->time_to_eat = ft_atoi(argv[3]);
    t->time_to_sleep = ft_atoi(argv[4]);
    t->end = 0;

    // if (t->nb_philo <= 0 || t->time_to_die <= 0 || 
        // t->time_to_eat <= 0 || t->time_to_sleep <= 0)
            // return(EXIT_FAILURE);  
    // t->philo = malloc(t->nb_philo * sizeof(t_philo));  
    return(0);
}

int main(int argc, char **argv)
{
    //p_thread_mutex_t    mutex;
    t_table     *t;
    
    // printf("time : %ld\n", ft_timestamp());
    
    if (argc < 5 || argc > 6)
        return(printf("Incorrect number of arguments.\n"));
    t = init_t();
    if (!t)
        return(EXIT_FAILURE);
    // printf("end : %d\n", t->end);
    // printf("time : %lu\n", t->start_time);
    // printf("die : %lu\n", t.time_to_die);
    if (!check_args(argc, argv, t))
    {
        // printf("num : %d\n", t.nb_philo);
        // printf("end : %d\n", t.end);
        if (t->nb_philo > 200)
		    return(printf("We should not create more than 200 philosophers\n"));
        ft_thread(t);
        exit_all(t);
        // printf("num : %d\n", t->nb_philo);
        // init_philo(t);
    }
    else
        write(2, "Arguments must be a numeric and positive value.\n", 48);
    free(t);
    
    return (0);
}