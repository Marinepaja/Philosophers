/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:46:45 by mlaporte          #+#    #+#             */
/*   Updated: 2024/05/19 22:36:15 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <stdint.h>
# include <unistd.h>

typedef struct s_philo  t_philo;
typedef struct s_table  t_table;

struct s_philo
{
    int             num;
    pthread_t       thd;
	pthread_mutex_t	status;
    pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
    t_table         *table;
};

struct s_table
{
    int     nb_philo;

    pthread_t      *threads;
    pthread_mutex_t      **forks;
    t_philo         *philo;
    uint64_t		start_time;
	int				end;
    pthread_mutex_t	status;
	int				wait_time;
    uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;

};

long int	ft_atoi(const char *str);

#endif