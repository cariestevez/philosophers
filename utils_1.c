/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:39:17 by cestevez          #+#    #+#             */
/*   Updated: 2024/02/05 16:04:44 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_one_philo(t_guest *philo)
{
	pthread_mutex_lock(&philo->data->forks[0]);
	printf("%lu %d has taken a fork\n", get_time(philo->start), 1);
	usleep(philo->data->time_to_die * 1000);
	printf("%lu %d died\n", get_time(philo->start), 1);
	pthread_mutex_unlock(&philo->data->forks[0]);
}

int	guests_to_table(t_args *d)
{
	int	i;

	i = 0;
	while (i <= d->num_philos)
	{
		if (i == 0)
			pthread_create(&d->philo[i].id, NULL, waiter, (void *)&d->philo[i]);
		else
			pthread_create(&d->philo[i].id, NULL, dinner, (void *)&d->philo[i]);
		i++;
	}
	return (1);
}

//joins threads to the main processes' one (waits until all threads end)
int	wait_to_finish(t_args *data)
{
	int	i;
	int	join_return;

	i = 0;
	join_return = 0;
	while (i <= data->num_philos)
	{
		join_return = pthread_join(data->philo[i].id, NULL);
		if (join_return != 0)
		{
			printf("Error(%d) joining threads. Exiting...\n", join_return);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_usleep(uint64_t milliseconds)
{
	size_t	start;

	start = get_time(0);
	while ((get_time(0) - start) < milliseconds)
		usleep(500);
	return (0);
}

uint64_t	get_time(uint64_t start)
{
	struct timeval	current_time;
	uint64_t		sec;
	uint64_t		usec;

	gettimeofday(&current_time, NULL);
	sec = current_time.tv_sec * 1000;
	usec = current_time.tv_usec / 1000;
	if (start == 0)
		return (sec + usec);
	return (sec + usec - start);
}
