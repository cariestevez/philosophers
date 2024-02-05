/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:39:45 by cestevez          #+#    #+#             */
/*   Updated: 2024/02/05 16:10:34 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_guest	t_guest;

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
	t_guest			*philo;
}	t_args;

//specific for each thread(=philoopher)
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
void		start_soiree(t_args *data);
int			receive_guests(t_args *data);
void		*dinner(void *param);
int			hungry_and_alive(t_guest *philo);

//init_and_free.c
void		destroy_and_free(t_args *data, int i, int flag);
t_args		*lay_the_table(char **argv);
t_args		*last_preparations(t_args *data, char **argv);
t_args		*last_preps_2(t_args *data, char **argv);

//routine.c
void		even_takes_fork(t_guest *philo);
void		odd_takes_fork(t_guest *philo);
void		ft_eat(t_guest *philo);
void		nap_wakeup(t_guest *philo);
void		ft_think(t_guest *philo);

//monitoring.c
void		*waiter(void *param);
int			ft_someone_died(t_guest *philo);
int			ft_enough_spaghetti(t_guest *philo);

//utils_1.c
void		ft_one_philo(t_guest *philo);
int			guests_to_table(t_args *d);
int			wait_to_finish(t_args *data);
int			ft_usleep(uint64_t milliseconds);
uint64_t	get_time(uint64_t start);

//utils_2.c
void		ft_write(t_guest *philo, int eating);
int			is_valid(const char *nptr);
int			ft_atoi(const char *nptr);
void		*ft_calloc(size_t nmemb, size_t size);
void		ft_bzero(void *s, size_t n);
