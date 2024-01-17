/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:01:03 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/17 17:59:10 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat_nap_wakeup(t_guest *philo)
{
	pthread_mutex_lock(philo->data->forks[philo->num]);
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d has taken a fork\n", get_time(philo->start), philo->num + 1);
	pthread_mutex_unlock(philo->data->print_mutex);
	if (philo->num == 0)
		pthread_mutex_lock(philo->data->forks[philo->data->num_philos - 1]);
	else
		pthread_mutex_lock(philo->data->forks[philo->num - 1]);
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d has taken a fork\n", get_time(philo->start), philo->num + 1);
	printf("%lu %d is eating\n", get_time(philo->start), philo->num + 1);
	pthread_mutex_unlock(philo->data->print_mutex);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->data->forks[philo->num]);
	if (philo->num == 0)
		pthread_mutex_unlock(philo->data->forks[philo->data->num_philos - 1]);
	else
		pthread_mutex_unlock(philo->data->forks[philo->num - 1]);
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d is sleeping\n", get_time(philo->start), philo->num + 1);
	pthread_mutex_unlock(philo->data->print_mutex);
	usleep(philo->data->time_to_sleep * 1000);
	philo->wakeup_time = get_time(0);
}

void	ft_think(t_guest *philo)
{
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d is thinking\n", get_time(philo->start), philo->num + 1);
	pthread_mutex_unlock(philo->data->print_mutex);
	usleep(100);
}

void	ft_one_philo(t_guest *philo)
{
	struct timeval		current_time;

	pthread_mutex_lock(philo->data->forks[philo->num]);
	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d has taken a fork\n", get_time(philo->start), philo->num + 1);
	pthread_mutex_unlock(philo->data->print_mutex);
	usleep(philo->data->time_to_die * 1000);
	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(philo->data->timeofdeath_mutex);
	philo->data->time_of_death = get_time(philo->start);
	pthread_mutex_unlock(philo->data->timeofdeath_mutex);
	pthread_mutex_unlock(philo->data->forks[philo->num]);
}
