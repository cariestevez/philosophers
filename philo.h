/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:39:45 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/24 22:58:19 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_guest t_guest;

//main data structure, shared by all threads
typedef struct s_args
{
	int				num_philos;
	int				end_dinner;
	int				times_must_eat;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	enddinner_mutex;
	pthread_mutex_t	lastmeal_mutex;
	t_guest			*philos;
}	t_args;

//specific for each thread(=philosopher)
typedef struct s_guest
{
	int				num;
	int				eating;
	int				meals;
	uint64_t		start;
	uint64_t		last_meal;
	pthread_t		id;
	t_args			*data;
}	t_guest;

//main.c
int			hungry_and_alive(t_guest *philo);
void		*dinner(t_guest *philosopher);
t_guest		*start_soiree(t_args *data);
t_guest		*receive_guests(t_args *data);

//init_and_free.c
void		*destroy_and_free(t_guest *philo, t_args *data, int i, int flag);
t_args		*lay_the_table(char **argv);
t_args		*last_preparations(t_args *data, char **argv);
t_args		*last_preps_2(t_args *data, char **argv);
t_args		*last_preps_3(t_args *data, char **argv);

//routine.c
void		ft_eat(t_guest *philo);
void		nap_wakeup(t_guest *philo);
void		ft_think(t_guest *philo);
void		ft_one_philo(t_guest *philo);

//monitoring.c
void		*waiter(t_guest *philo);
int			ft_someone_died(t_guest *philo);
int			ft_enough_spaghetti(t_guest *philo);

//utils.c
void		print_error(t_args *data);
int			ft_usleep(uint64_t milliseconds);
uint64_t	get_time(uint64_t start);
int			join_threads(t_guest *philosopher);
int			ft_atoi(const char *nptr);
int			is_valid(const char *nptr);