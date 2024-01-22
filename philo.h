/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:39:45 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/17 18:24:55 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

//main data structure, shared by all threads
typedef struct s_args
{
	int				num_philos;
	int				someone_died;
	int				remaining;
	uint64_t		time_of_death;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				times_must_eat;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	**forks;
	pthread_mutex_t	*someonedied_mutex;
	pthread_mutex_t	*timeofdeath_mutex;
	pthread_mutex_t	*remaining_mutex;
}	t_args;

//specific for each thread(=philosopher)
typedef struct s_guest
{
	int				num;
	int				times_eaten;
	uint64_t		start;
	uint64_t		wakeup_time;
	pthread_t		id;
	t_args			*data;
}	t_guest;

//main.c
void		*soiree(t_guest *philosopher);
void		receive_guests(t_args *data);

//init_and_free.c
void		*destroy_and_free(t_args *data, int i, int flag);
t_args		*lay_the_table(char **argv);
t_args		*last_preparations(t_args *data, char **argv);
t_args		*last_preps_2(t_args *data, char **argv);
t_args		*last_preps_3(t_args *data);

//routine.c
void		eat_nap_wakeup(t_guest *philo);
void		ft_think(t_guest *philo);
void		ft_one_philo(t_guest *philo);

//monitoring.c
int			ft_i_died(t_guest *philo);
int			ft_someone_died(t_guest *philo);
int			ft_enough_spaghetti(t_guest *philo, int times_eaten, int round);

//utils.c
uint64_t	get_time(uint64_t start);
int			join_threads(t_guest *philosopher);
int			ft_atoi(const char *nptr);
int			is_valid(const char *nptr);