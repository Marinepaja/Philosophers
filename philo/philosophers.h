/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:46:45 by mlaporte          #+#    #+#             */
/*   Updated: 2024/06/01 18:28:17 by mlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <sys/time.h>
# include <stdio.h>
# include <pthread.h>
# include <stdint.h>
# include <unistd.h>

typedef struct s_philo  t_philo;
typedef struct s_table  t_table;

typedef enum e_state
{
	E_INIT_STATE = 0,
	E_SLEEP = 10,
	E_FORK,
	E_EAT,
	E_THINK,
	E_DIE,
}	t_state;


struct s_philo
{
    int             num;
    pthread_t       thd;
    pthread_mutex_t	meal_update;
	pthread_mutex_t	status;
    pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
    uint64_t		meal;
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
    pthread_mutex_t	write;
	int				wait_time;
    uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;

};

long int	ft_atoi(const char *str);

#endif