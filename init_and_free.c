/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:41:25 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/17 15:46:02 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//destroys init mutexes(forks), frees the forks array and the args struct
void	*destroy_and_free(t_args *data, int i, int flag)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(data->forks[i]);
		free(data->forks[i]);
		data->forks[i] = NULL;
		i--;
	}
	free(data->forks);
	data->forks = NULL;
	if (flag != 0)
	{
		pthread_mutex_destroy(data->someonedied_mutex);
		pthread_mutex_destroy(data->remaining_mutex);
		free(data->someonedied_mutex);
		data->someonedied_mutex = NULL;
		free(data->remaining_mutex);
		data->remaining_mutex = NULL;
	}
	free(data);
	data = NULL;
	return (NULL);
}

//initializes the main data struct and the mutex objects(forks)
t_args	*lay_the_table(char **argv)
{
	int		i;
	t_args	*data;

	i = 0;
	data = (t_args *)malloc(sizeof(t_args));
	if (data == NULL)
		return (NULL);
	data->num_philos = ft_atoi(argv[1]);
	data->forks = malloc(sizeof(pthread_mutex_t *) * data->num_philos);
	if (data->forks == NULL)
		return (free(data), NULL);
	while (i < data->num_philos)
	{
		data->forks[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (data->forks[i] == NULL)
			return (destroy_and_free(data, i, 0));
		if (pthread_mutex_init(data->forks[i], NULL) != 0)
			return (destroy_and_free(data, i, 0));
		i++;
	}
	if (last_preparations(data, argv) == NULL)
		return (destroy_and_free(data, i, 0));
	return (data);
}

//initializes the rest of the main data struct vars and mutex objects
t_args	*last_preparations(t_args *data, char **argv)
{
	data->someone_died = 0;
	data->someonedied_mutex = malloc(sizeof(pthread_mutex_t));
	if (data->someonedied_mutex == NULL)
		return (NULL);
	if (pthread_mutex_init(data->someonedied_mutex, NULL) != 0)
		return (free(data->someonedied_mutex), NULL);
	data->remaining = data->num_philos;
	data->remaining_mutex = malloc(sizeof(pthread_mutex_t));
	if (data->remaining_mutex == NULL)
	{
		pthread_mutex_destroy(data->someonedied_mutex);
		return (free(data->someonedied_mutex), NULL);
	}
	if (pthread_mutex_init(data->remaining_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(data->someonedied_mutex);
		free(data->someonedied_mutex);
		return (free(data->remaining_mutex), NULL);
	}
	if (last_preps_2(data, argv) == NULL)
		return (NULL);
	return (data);
}

t_args	*last_preps_2(t_args *data, char **argv)
{
	data->time_of_death = 0;
	data->timeofdeath_mutex = malloc(sizeof(pthread_mutex_t));
	if (data->timeofdeath_mutex == NULL)
	{
		pthread_mutex_destroy(data->someonedied_mutex);
		pthread_mutex_destroy(data->remaining_mutex);
		free(data->someonedied_mutex);
		return (free(data->remaining_mutex), NULL);
	}
	if (pthread_mutex_init(data->timeofdeath_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(data->someonedied_mutex);
		pthread_mutex_destroy(data->remaining_mutex);
		free(data->someonedied_mutex);
		free(data->remaining_mutex);
		return (free(data->timeofdeath_mutex), NULL);
	}
	data->time_to_die = (uint64_t)ft_atoi(argv[2]);
	data->time_to_eat = (uint64_t)ft_atoi(argv[3]);
	data->time_to_sleep = (uint64_t)ft_atoi(argv[4]);
	if (last_preps_3(data) == NULL)
		return (NULL);
	return (data);
}

t_args	*last_preps_3(t_args *data)
{
	data->times_must_eat = 0;
	data->print_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (data->print_mutex == NULL)
	{
		pthread_mutex_destroy(data->someonedied_mutex);
		pthread_mutex_destroy(data->remaining_mutex);
		pthread_mutex_destroy(data->timeofdeath_mutex);
		free(data->someonedied_mutex);
		free(data->remaining_mutex);
		return (free(data->timeofdeath_mutex), NULL);
	}
	if (pthread_mutex_init(data->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(data->someonedied_mutex);
		pthread_mutex_destroy(data->remaining_mutex);
		pthread_mutex_destroy(data->timeofdeath_mutex);
		free(data->someonedied_mutex);
		free(data->remaining_mutex);
		free(data->timeofdeath_mutex);
		return (free(data->print_mutex), NULL);
	}
	return (data);
}
