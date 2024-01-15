#include "philo.h"

//destroys initialized mutex objects(forks), frees the forks array and the args structure
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
		pthread_mutex_destroy(data->death_mutex);
		pthread_mutex_destroy(data->remaining_mutex);
		free(data->death_mutex);
		data->death_mutex = NULL;
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
	int	i;
	t_args	*data;

	i = 0;
	data = (t_args *)malloc(sizeof(t_args));
	if (data == NULL)
		return (NULL);
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * data->number_of_philosophers);
	if (data->forks == NULL)
		return (free(data), NULL);
	while (i < data->number_of_philosophers)
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
	data->death_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (data->death_mutex == NULL)
		return (NULL);
    if (pthread_mutex_init(data->death_mutex, NULL) != 0)
		return (free(data->death_mutex), NULL);
	data->remaining_guests = data->number_of_philosophers;
	data->remaining_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (data->remaining_mutex == NULL)
	{
		pthread_mutex_destroy(data->death_mutex);
		return (free(data->death_mutex), NULL);
	}
	if (pthread_mutex_init(data->remaining_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(data->death_mutex);
		free(data->death_mutex);
		return (free(data->remaining_mutex), NULL);
	}
	data->time_of_defunction = 0;
	data->time_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (data->time_mutex == NULL)
	{
		pthread_mutex_destroy(data->death_mutex);
		pthread_mutex_destroy(data->remaining_mutex);
		free(data->death_mutex);
		return (free(data->remaining_mutex), NULL);
	}
	if (pthread_mutex_init(data->time_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(data->death_mutex);
		pthread_mutex_destroy(data->remaining_mutex);
		free(data->death_mutex);
		free(data->remaining_mutex);
		return (free(data->time_mutex), NULL);
	}
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->number_of_times_each_philosopher_must_eat = 0;
	return (data);
}
