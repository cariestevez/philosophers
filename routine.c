/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:01:03 by cestevez          #+#    #+#             */
/*   Updated: 2024/02/05 16:04:23 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//evens take right fork first
void	even_takes_fork(t_guest *philo)
{
	if (philo->num == philo->data->num_philos)
		pthread_mutex_lock(&philo->data->forks[0]);
	else
		pthread_mutex_lock(&philo->data->forks[philo->num]);
	ft_write(philo, 0);
	pthread_mutex_lock(&philo->data->forks[philo->num - 1]);
	ft_write(philo, 0);
}

//odds take left fork first
void	odd_takes_fork(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->num - 1]);
	ft_write(philo, 0);
	if (philo->num == philo->data->num_philos)
		pthread_mutex_lock(&philo->data->forks[0]);
	else
		pthread_mutex_lock(&philo->data->forks[philo->num]);
	ft_write(philo, 0);
}

void	ft_eat(t_guest *philo)
{
	if (philo->num % 2)
		even_takes_fork(philo);
	else
		odd_takes_fork(philo);
	pthread_mutex_lock(&philo->data->lastmeal_mutex);
	philo->eating = 1;
	philo->last_meal = get_time(0);
	pthread_mutex_unlock(&philo->data->lastmeal_mutex);
	ft_write(philo, 1);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->data->forks[philo->num - 1]);
	if (philo->num == philo->data->num_philos)
		pthread_mutex_unlock(&philo->data->forks[0]);
	else
		pthread_mutex_unlock(&philo->data->forks[philo->num]);
	pthread_mutex_lock(&philo->data->lastmeal_mutex);
	philo->eating = 0;
	philo->meals++;
	pthread_mutex_unlock(&philo->data->lastmeal_mutex);
	return ;
}

void	nap_wakeup(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->enddinner_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (philo->data->end_dinner == 0)
		printf("%lu %d is sleeping\n", get_time(philo->start), philo->num);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->enddinner_mutex);
	ft_usleep(philo->data->time_to_sleep);
}

void	ft_think(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->enddinner_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (philo->data->end_dinner == 0)
		printf("%lu %d is thinking\n", get_time(philo->start), philo->num);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->enddinner_mutex);
	usleep(500);
}
