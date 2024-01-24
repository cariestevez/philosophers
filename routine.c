/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:01:03 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/25 00:55:53 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_eat(t_guest *philo)
{
	if (philo->num % 2)
	{
		//pthread_mutex_lock(philo->fork_r);
		if (philo->num == philo->data->num_philos)
			pthread_mutex_lock(&philo->data->forks[0]);
		else
			pthread_mutex_lock(&philo->data->forks[philo->num]);
		//ft_write("has taken a fork", philo->id, philo->rules);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%llu %d has taken a fork\n", get_time(philo->start), philo->num);
		pthread_mutex_unlock(&philo->data->print_mutex);	
		
		//pthread_mutex_lock(philo->fork_l);
		pthread_mutex_lock(&philo->data->forks[philo->num - 1]);
		//ft_write("has taken a fork", philo->id, philo->rules);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%llu %d has taken a fork\n", get_time(philo->start), philo->num);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
	else
	{
		//pthread_mutex_lock(philo->fork_l);
		//ft_write("has taken a fork", philo->id, philo->rules);
		//pthread_mutex_lock(philo->fork_l);
		pthread_mutex_lock(&philo->data->forks[philo->num - 1]);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%llu %d has taken a fork\n", get_time(philo->start), philo->num);
		pthread_mutex_unlock(&philo->data->print_mutex);
		
		//pthread_mutex_lock(philo->fork_r);
		//ft_write("has taken a fork", philo->id, philo->rules);
		if (philo->num == philo->data->num_philos)
			pthread_mutex_lock(&philo->data->forks[0]);
		else
			pthread_mutex_lock(&philo->data->forks[philo->num]);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%llu %d has taken a fork\n", get_time(philo->start), philo->num);
		pthread_mutex_unlock(&philo->data->print_mutex);	
	}
	//////////////mine:
	// if (philo->num == philo->data->num_philos)
	// 	pthread_mutex_lock(&philo->data->forks[0]);
	// else
	// 	pthread_mutex_lock(&philo->data->forks[philo->num]);
	// // if (hungry_and_alive(philo) == 0)
	// // 	return ;
	// pthread_mutex_lock(&philo->data->print_mutex);
	// printf("%llu %d has taken a fork\n", get_time(philo->start), philo->num);
	// pthread_mutex_unlock(&philo->data->print_mutex);	
	// pthread_mutex_lock(&philo->data->forks[philo->num - 1]);

	pthread_mutex_lock(&philo->data->lastmeal_mutex);
	philo->eating = 1;
	philo->last_meal = get_time(0);
	pthread_mutex_unlock(&philo->data->lastmeal_mutex);
	
	pthread_mutex_lock(&philo->data->print_mutex);
	//printf("%llu %d has taken a fork\n", get_time(philo->start), philo->num);
	printf("%llu %d is eating\n", get_time(philo->start), philo->num);
	pthread_mutex_unlock(&philo->data->print_mutex);

	ft_usleep(philo->data->time_to_eat);
	
	pthread_mutex_unlock(&philo->data->forks[philo->num - 1]);
	if (philo->num == philo->data->num_philos)
			pthread_mutex_unlock(&philo->data->forks[0]);
	else
			pthread_mutex_unlock(&philo->data->forks[philo->num]);
	///mine:
	// if (philo->num == philo->data->num_philos)
	// 	pthread_mutex_unlock(&philo->data->forks[0]);
	// else
	// 	pthread_mutex_unlock(&philo->data->forks[philo->num]);
	// pthread_mutex_unlock(&philo->data->forks[philo->num - 1]);
	
	pthread_mutex_lock(&philo->data->lastmeal_mutex);
	philo->eating = 0;
	philo->meals++;
	pthread_mutex_unlock(&philo->data->lastmeal_mutex);
	
	return ;
}

void	nap_wakeup(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%llu %d is sleeping\n", get_time(philo->start), philo->num);
	pthread_mutex_unlock(&philo->data->print_mutex);
	ft_usleep(philo->data->time_to_sleep);
}

void	ft_think(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%llu %d is thinking\n", get_time(philo->start), philo->num);
	pthread_mutex_unlock(&philo->data->print_mutex);
	usleep(500);
}

void	ft_one_philo(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->forks[0]);
	printf("%llu %d has taken a fork\n", get_time(philo->start), 1);
	usleep(philo->data->time_to_die * 1000);
	printf("%llu %d died\n", get_time(philo->start), 1);
	pthread_mutex_unlock(&philo->data->forks[0]);
}
