/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:46:45 by mlaporte          #+#    #+#             */
/*   Updated: 2024/06/07 10:59:27 by mlaporte         ###   ########.fr       */
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
# include <limits.h>

typedef struct s_philo	t_philo;
typedef struct s_table	t_table;

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
	int					num;
	int					nb_meal;
	pthread_t			thd;
	pthread_mutex_t		meal_update;
	pthread_mutex_t		status;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	uint64_t			meal;
	t_table				*table;
};

struct s_table
{
	int					nb_philo;
	pthread_mutex_t		thread;
	pthread_mutex_t		**forks;
	t_philo				*philo;
	uint64_t			start_time;
	int					end;
	int					max_meal;
	pthread_mutex_t		status;
	pthread_mutex_t		write;
	int					wait_time;
	uint64_t			time_to_die;
	uint64_t			time_to_eat;
	uint64_t			time_to_sleep;
};

int			ft_thread(t_table *t);
int			init_mutex(t_table *t);
t_table		*init_t(void);
t_philo		*init_philo(t_table *t);

long int	ft_atoi(const char *str);
void		print_msg(t_table *t, int num, t_state state, long int time);
long int	ft_timestamp(long int start);
long int	get_time(void);
long int	ft_max(long int a, long int b);

void		exit_all(t_table *t);
int			check_death(t_table *t);
void		my_sleep(t_table *t, long int time);

void		put_forks(t_philo *philo, t_table *table);
int			take_forks(t_philo *philo, t_table *table);
int			all_full(t_table *t);
void		ft_eat(t_philo *p, t_table *t);

int			check_args(int argc, char **argv, t_table *t);
void		*do_philo(void *p);
void		delay(t_philo *p);
#endif