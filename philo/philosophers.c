/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:47:29 by mlaporte          #+#    #+#             */
/*   Updated: 2024/05/21 15:23:47 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    put_forks(t_philo *philo, t_table *table)
{
    if (table->nb_philo == 1)
        {printf("%d has died\n", philo->num);
        return;}
     printf("left fork : %p\n", philo->right_fork);
    if (!pthread_mutex_unlock(philo->right_fork))
        printf("right back");
    if (!pthread_mutex_unlock(philo->left_fork))
        printf("left back");
    // usleep(table->time_to_eat);
    // pthread_mutex_unlock(philo->left_fork);
    
}

void    take_forks(t_philo *philo, t_table *table)
{
    if (table->nb_philo == 1)
        {printf("%d has died\n", philo->num);
        return;}
    //  printf("left fork : %p\n", philo->right_fork);
    if (pthread_mutex_lock(philo->right_fork))
        {table->end = 1;
        return ;}
    if (!pthread_mutex_lock(philo->left_fork))
        printf("took left f");
    // usleep(table->time_to_eat);
    // pthread_mutex_unlock(philo->left_fork);
    
}

void    ft_eat(t_philo *p, t_table *t)
{
    take_forks(p, t);
    printf("%d took a fork\n", p->num);
    usleep(t->time_to_eat);
    put_forks(p, t);
}

void *do_philo(void *p)
{
    t_philo *tmp;
    // int i;

    // i = 0;
    tmp = p;
    
    printf("hello, %d\n", tmp->num);
    while (!tmp->table->end)
    {
    if (tmp->num % 2 == 1)
        ft_eat(tmp, tmp->table);
    // printf("hello, %d\n", tmp->num);
        if (tmp->table->nb_philo == 20)
            tmp->table->end = 1;
        
    }
    return (0);
}

t_philo *init_philo(t_table *t)
{
     int i;
    t_philo *p;

    i = 0;
    
    
    p = malloc(t->nb_philo * sizeof(t_philo));

    while (i < t->nb_philo)
    {
        p[i].num = i + 1;
        // p = &t->philo[i];
        printf("philo : %d\n", p[i].num);
        p[i].table = t;
        p[i].left_fork = t->forks[i];
        p[i].right_fork = t->forks[(i + 1) % t->nb_philo];
        printf("left r : %p\n", p[i].right_fork);
        printf("left f : %p\n", p[i].left_fork);
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
			t->forks[i] = NULL;
			return (EXIT_FAILURE);
		}
        i++;
    }
    return (EXIT_SUCCESS);
}


int init_t(t_table *t)
{
    t = malloc(sizeof(t_table));
	if (!t)
		return (EXIT_FAILURE);
	t->nb_philo = 0;
	t->time_to_die = 0;
	t->time_to_eat = 0;
    t->time_to_sleep = 0;
	// t->max_eat = -1;
	t->forks = NULL;
	t->end = 0;


    // t->threads = malloc(t->nb_philo *sizeof(pthread_t));
   
    // printf("%d\n", t->nb_philo);

    
    return (0);
}

int ft_thread(t_table *t)
{
    int i = 0;
    t_philo    *p;
    
    init_mutex(t);
    t->philo = init_philo(t);// p = malloc(t->nb_philo * sizeof(t_philo));
    p = NULL;
    // (void)p;
    while (i < t->nb_philo)
    {
        p = &t->philo[i];

        printf("%d\n", p->  num);
        // p->table = t;
    
        // p[i].num = i;
        // t->philo[i].left_fork = &t->forks[i];
        // t->philo[i].right_fork = &t->forks[(i + 1) % t->nb_philo];
        // printf("num : %d, i= %d, nb = %d\n", p->num, i, t->nb_philo);
        if (pthread_create(&p->thd, NULL, &do_philo, p))
        {
            printf("Error creating pthread %d\n", i);
            pthread_mutex_lock(&t->status);
            t->end = 1;
            pthread_mutex_unlock(&t->status);
        }
        pthread_join(p->thd, NULL);
        //  printf("num thread : %d, %d\n", p->num, t->philo->num);
        i++;
    }

    return(EXIT_SUCCESS);
}

int check_args(int argc, char **argv, t_table *t)
{
    (void)argc;
    // t = malloc (sizeof(t_table));
    if (ft_atoi(argv[1]) < 0 || ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0 || ft_atoi(argv[4]) < 0)
        return(EXIT_FAILURE);
    t->nb_philo = ft_atoi(argv[1]);
    printf("atoi : %d\n", t->nb_philo);
    t->time_to_die = ft_atoi(argv[2]);

    // printf("t : %ld, %d\n", t->time_to_die, t->time_to_die <= 0);
    t->time_to_eat = ft_atoi(argv[3]);
    t->time_to_sleep = ft_atoi(argv[4]);

    // if (t->nb_philo <= 0 || t->time_to_die <= 0 || 
        // t->time_to_eat <= 0 || t->time_to_sleep <= 0)
            // return(EXIT_FAILURE);  
    // t->philo = malloc(t->nb_philo * sizeof(t_philo));  
    return(0);
}

int main(int argc, char **argv)
{
    //p_thread_mutex_t    mutex;
    t_table     t;
    
    if (argc < 5 || argc > 6)
        return(printf("Incorrect number of arguments.\n"));
    if (init_t(&t))
        return (EXIT_FAILURE);
    if (!check_args(argc, argv, &t))
    {
        printf("num : %d\n", t.nb_philo);
        if (t.nb_philo > 200)
		    return(printf("We should not create more than 200 philosophers\n"));

        printf("num : %d\n", t.nb_philo);
        // init_philo(t);
        ft_thread(&t);
    }
    else
        write(2, "Arguments must be a numeric and positive value.\n", 48);
    return (0);
}