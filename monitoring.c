/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:58:52 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/28 23:39:41 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*waiter(void *param)
{
	t_guest	*philo;

	philo = (t_guest *) param;
	if (philo->data->num_philos == 1)
		return (NULL);
	ft_usleep(100);
	while (1)
	{
		if (ft_someone_died(philo) || ft_enough_spaghetti(philo))
			break ;
	}
	write(1, "waiter finishing\n", 18);
	return (NULL);
}

int	ft_someone_died(t_guest *philo)
{
	int	i;

	i = 1;
	while (i <= philo->data->num_philos)
	{
		pthread_mutex_lock(&philo->data->lastmeal_mutex);
		if ((get_time(0) - philo->data->ph[i].last_meal) > philo->data->to_die
			&& philo[i].eating == 0)
		{
			pthread_mutex_lock(&philo->data->print_mutex);
			printf("%llu %d died\n", get_time(philo->start), i);
			pthread_mutex_unlock(&philo->data->print_mutex);
			pthread_mutex_lock(&philo->data->enddinner_mutex);
			philo->data->end_dinner = i;
			pthread_mutex_unlock(&philo->data->enddinner_mutex);
			pthread_mutex_unlock(&philo->data->lastmeal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&philo->data->lastmeal_mutex);
		i++;
	}
	return (0);
}

int	ft_enough_spaghetti(t_guest *philo)
{
	int	i;
	int	enough_spaghetti;

	i = 1;
	enough_spaghetti = 0;
	if (philo->data->must_eat == 0)
		return (0);
	pthread_mutex_lock(&philo->data->lastmeal_mutex);
	while (i <= philo->data->num_philos)
	{
		if (philo[i].meals >= philo->data->must_eat)
			enough_spaghetti++;
		i++;
	}
	pthread_mutex_unlock(&philo->data->lastmeal_mutex);
	if (enough_spaghetti == philo->data->num_philos)
	{
		pthread_mutex_lock(&philo->data->enddinner_mutex);
		philo->data->end_dinner = i;
		pthread_mutex_unlock(&philo->data->enddinner_mutex);
		return (1);
	}
	return (0);
}
