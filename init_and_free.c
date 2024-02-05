/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:41:25 by cestevez          #+#    #+#             */
/*   Updated: 2024/02/05 16:17:02 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//destroys init mutexes(forks), frees the forks array and the args struct
void	destroy_and_free(t_args *data, int i, int flag)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i--;
	}
	free(data->forks);
	data->forks = NULL;
	if (flag != 0)
	{
		pthread_mutex_destroy(&data->enddinner_mutex);
		pthread_mutex_destroy(&data->lastmeal_mutex);
		pthread_mutex_destroy(&data->print_mutex);
	}
	free(data->philo);
	data->philo = NULL;
	free(data);
	data = NULL;
}

//initializes the main data struct and the mutex objects(forks)
t_args	*lay_the_table(char **argv)
{
	int		i;
	t_args	*data;

	i = 0;
	data = (t_args *)ft_calloc(sizeof(t_args), 1);
	if (data == NULL)
		return (NULL);
	data->num_philos = ft_atoi(argv[1]);
	data->forks = ft_calloc(sizeof(pthread_mutex_t), data->num_philos);
	if (data->forks == NULL)
		return (free(data), NULL);
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (destroy_and_free(data, i, 0), NULL);
		i++;
	}
	if (last_preparations(data, argv) == NULL)
		return (destroy_and_free(data, i, 0), NULL);
	return (data);
}

//initializes the rest of the main data struct vars and mutex objects
t_args	*last_preparations(t_args *data, char **argv)
{
	data->end_dinner = 0;
	if (pthread_mutex_init(&data->enddinner_mutex, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&data->lastmeal_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->enddinner_mutex);
		return (NULL);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->enddinner_mutex);
		pthread_mutex_destroy(&data->lastmeal_mutex);
		return (NULL);
	}
	if (last_preps_2(data, argv) == NULL)
		return (NULL);
	return (data);
}

t_args	*last_preps_2(t_args *data, char **argv)
{
	data->time_to_die = (uint64_t)ft_atoi(argv[2]);
	data->time_to_eat = (uint64_t)ft_atoi(argv[3]);
	data->time_to_sleep = (uint64_t)ft_atoi(argv[4]);
	data->times_must_eat = 0;
	data->philo = NULL;
	return (data);
}
