/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:01:03 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/22 19:01:18 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat_nap_wakeup(t_guest *philo)
{
	if (philo->num == philo->data->num_philos)
		pthread_mutex_lock(philo->data->forks[0]);
	else
		pthread_mutex_lock(philo->data->forks[philo->num]);
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d has taken a fork\n", get_time(philo->start), philo->num);
	pthread_mutex_unlock(philo->data->print_mutex);	
	pthread_mutex_lock(philo->data->forks[philo->num - 1]);
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d has taken a fork\n", get_time(philo->start), philo->num + 1);
	printf("%lu %d is eating\n", get_time(philo->start), philo->num + 1);
	pthread_mutex_unlock(philo->data->print_mutex);
	
	pthread_mutex_lock(philo->data->lastmeal_mutex);
	philo->last_meal = get_time(0);
	philo->times_eaten++;
	pthread_mutex_unlock(philo->data->lastmeal_mutex);
	
	usleep(philo->data->time_to_eat * 1000);
	if (philo->num == philo->data->num_philos)
		pthread_mutex_unlock(philo->data->forks[0]);
	else
		pthread_mutex_unlock(philo->data->forks[philo->num]);
	pthread_mutex_unlock(philo->data->forks[philo->num - 1]);
	
	//check if himself died also here? in case time to die is < time to eat
	pthread_mutex_lock(philo->data->someonedied_mutex);
	if (philo->data->someonedied_mutex)
		return (pthread_mutex_unlock(philo->data->someonedied_mutex), 0);
	pthread_mutex_unlock(philo->data->someonedied_mutex);
	
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d is sleeping\n", get_time(philo->start), philo->num + 1);
	pthread_mutex_unlock(philo->data->print_mutex);
	usleep(philo->data->time_to_sleep * 1000);
	
	return (1);
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
	pthread_mutex_lock(philo->data->forks[1]);
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d has taken a fork\n", get_time(philo->start), 1);
	pthread_mutex_unlock(philo->data->print_mutex);
	usleep(philo->data->time_to_die * 1000);
	pthread_mutex_lock(philo->data->print_mutex);
	printf("%lu %d died\n", get_time(philo->start), 1);
	pthread_mutex_unlock(philo->data->print_mutex);
	pthread_mutex_unlock(philo->data->forks[philo->num]);
}
