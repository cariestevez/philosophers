/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:58:52 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/22 17:39:11 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_i_died(t_guest *philo)
{
	if ((get_time(0) - philo->last_meal) > philo->data->time_to_die)
	{
		pthread_mutex_lock(philo->data->someonedied_mutex);
		philo->data->someone_died = philo->num;
		pthread_mutex_unlock(philo->data->someonedied_mutex);
		pthread_mutex_lock(philo->data->timeofdeath_mutex);
		philo->data->time_of_death = get_time(philo->start);
		pthread_mutex_unlock(philo->data->timeofdeath_mutex);
		return (1);
	}
	return (0);
}

int	ft_someone_died(t_guest *philo)
{
	pthread_mutex_lock(philo->data->someonedied_mutex);
	if (philo->data->someone_died != 0)
	{
		pthread_mutex_unlock(philo->data->someonedied_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->data->someonedied_mutex);
	return (0);
}

int	ft_enough_spaghetti(t_guest *philo, int times_eaten, int round)
{
	if (philo->data->times_must_eat != 0
		&& times_eaten >= philo->data->times_must_eat)
	{
		pthread_mutex_lock(philo->data->remaining_mutex);
		if (times_eaten == philo->data->times_must_eat
			&& ((philo->num % 2 == 0 && round % 2 == 0)
				|| (philo->num % 2 != 0 && round % 2 != 0)))
			philo->data->remaining--;
		if (philo->data->remaining <= 0)
		{
			pthread_mutex_unlock(philo->data->remaining_mutex);
			return (1);
		}
		pthread_mutex_unlock(philo->data->remaining_mutex);
	}
	return (0);
}
