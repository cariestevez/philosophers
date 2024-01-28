/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:01:03 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/28 23:59:29 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_eat(t_guest *ph)
{
	if (ph->num % 2)
	{
		//----->take right fork func
		if (ph->num == ph->data->num_philos)
			pthread_mutex_lock(&ph->data->forks[0]);
		else
			pthread_mutex_lock(&ph->data->forks[ph->num]);
		
		//-->create print function
		pthread_mutex_lock(&ph->data->enddinner_mutex);
		pthread_mutex_lock(&ph->data->print_mutex);
		if (ph->data->end_dinner == 0)
			printf("%llu %d has taken a fork\n", get_time(ph->start), ph->num);
		pthread_mutex_unlock(&ph->data->print_mutex);
		pthread_mutex_unlock(&ph->data->enddinner_mutex);//<--
		//<------


		//--> take left fork func
		pthread_mutex_lock(&ph->data->forks[ph->num - 1]);

		//-->create print function
		pthread_mutex_lock(&ph->data->enddinner_mutex);
		pthread_mutex_lock(&ph->data->print_mutex);
		if (ph->data->end_dinner == 0)
			printf("%llu %d has taken a fork\n", get_time(ph->start), ph->num);
		pthread_mutex_unlock(&ph->data->print_mutex);
		pthread_mutex_unlock(&ph->data->enddinner_mutex);//<--
		//<---
	}
	else
	{
		//--> take left fork func
		pthread_mutex_lock(&ph->data->forks[ph->num - 1]);
		
		//-->create print function
		pthread_mutex_lock(&ph->data->enddinner_mutex);
		pthread_mutex_lock(&ph->data->print_mutex);
		if (ph->data->end_dinner == 0)
			printf("%llu %d has taken a fork\n", get_time(ph->start), ph->num);
		pthread_mutex_unlock(&ph->data->print_mutex);
		pthread_mutex_unlock(&ph->data->enddinner_mutex);//<--
		//<---

//----->take right fork func
		if (ph->num == ph->data->num_philos)
			pthread_mutex_lock(&ph->data->forks[0]);
		else
			pthread_mutex_lock(&ph->data->forks[ph->num]);
		
		//-->create print function
		pthread_mutex_lock(&ph->data->enddinner_mutex);
		pthread_mutex_lock(&ph->data->print_mutex);
		if (ph->data->end_dinner == 0)
			printf("%llu %d has taken a fork\n", get_time(ph->start), ph->num);
		pthread_mutex_unlock(&ph->data->print_mutex);
		pthread_mutex_unlock(&ph->data->enddinner_mutex);//<--
		//<------
	}
	pthread_mutex_lock(&ph->data->lastmeal_mutex);
	ph->eating = 1;
	ph->last_meal = get_time(0);//25lines
	pthread_mutex_unlock(&ph->data->lastmeal_mutex);

//-->padd to rint func
	pthread_mutex_lock(&ph->data->enddinner_mutex);
	pthread_mutex_lock(&ph->data->print_mutex);
	if (ph->data->end_dinner == 0)
		printf("%llu %d is eating\n", get_time(ph->start), ph->num);
	pthread_mutex_unlock(&ph->data->print_mutex);
	pthread_mutex_unlock(&ph->data->enddinner_mutex);//<--

	ft_usleep(ph->data->to_die);

	pthread_mutex_unlock(&ph->data->forks[ph->num - 1]);
	if (ph->num == ph->data->num_philos)
		pthread_mutex_unlock(&ph->data->forks[0]);
	else
		pthread_mutex_unlock(&ph->data->forks[ph->num]);

	pthread_mutex_lock(&ph->data->lastmeal_mutex);
	ph->eating = 0;
	ph->meals++;
	pthread_mutex_unlock(&ph->data->lastmeal_mutex);
	return ;
}

void	nap_wakeup(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->enddinner_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (philo->data->end_dinner == 0)
		printf("%llu %d is sleeping\n", get_time(philo->start), philo->num);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->enddinner_mutex);
	ft_usleep(philo->data->to_sleep);
}

void	ft_think(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->enddinner_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (philo->data->end_dinner == 0)
		printf("%llu %d is thinking\n", get_time(philo->start), philo->num);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->enddinner_mutex);
	usleep(500);
}

void	ft_one_philo(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->forks[0]);
	printf("%llu %d has taken a fork\n", get_time(philo->start), 1);
	usleep(philo->data->to_die * 1000);
	printf("%llu %d died\n", get_time(philo->start), 1);
	pthread_mutex_unlock(&philo->data->forks[0]);
}
