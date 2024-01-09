/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:33:42 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/09 15:32:47 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//destroys initialized mutex objects(forks), frees the forks array and the args structure
void	*destroy_and_free(t_args *data, int i)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(data->forks[i]);
		free(data->forks[i]);
		i--;
	}
	free(data->forks);
	data->forks = NULL;
	free(data);
	data = NULL;
	return (NULL);
}

//initializes the main data struct and the mutex objects(forks)
t_args	*lay_the_table(char **argv)
{
	int	i;
	int	mutex_error;
	t_args	*data;

	i = 0;
	mutex_error = 0;
	data = (t_args *)malloc(sizeof(t_args));
	if (data == NULL)
		return (NULL);
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * data->number_of_philosophers);
	if (data->forks == NULL)
		return (NULL);
	while (i < data->number_of_philosophers)
	{
		data->forks[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (data->forks[i] == NULL)
			return (destroy_and_free(data, i));
		mutex_error = pthread_mutex_init(data->forks[i], NULL);
		if (mutex_error != 0)
			return (destroy_and_free(data, i));
		i++;
	}
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->number_of_times_each_philosopher_must_eat = 0;
	return (data);
}

//main logic
void	*start_soiree(t_guest *philosopher)
{
	struct timeval current_time;
	?? time_stamp;

	gettimeofday(&current_time, NULL);
	time_stamp = &current_time.tv_usec; 
    printf("...welcome philosopher %d...\n", philosopher->guest_id);
	while (1)
	{
		if ((gettimeofday() - time) > philosopher->data->time_to_die)
		{
			printf("%d %d died\n", gettimeofday(), philosopher->guest_id);
			break;
		}
		else if (philosopher->data->forks[philosopher->guest_id]
			&& philosopher->data->forks[philosopher->guest_id + 1])
		{
			// pthread_mutex_lock(philosopher->data->forks[philosopher->guest_id]); // takes the forks
			printf("%d %d is eating\n", gettimeofday(), philosopher->guest_id);
			//wait(philosopher->data->time_to_eat);
			//pthread_mutex_unlock(philosopher->data->forks[philosopher->guest_id]); // leaves forks on the table
			//time = gettimeofday();
			//printf("%d %d is sleeping\n", gettimeofday(), philosopher->guest_id);
			//usleep(philosopher->data->time_to_sleep);
		}
		printf("%d %d is thinking\n", gettimeofday(), philosopher->guest_id);
	}
    
	return (NULL);
}

//creates the threads
int	receive_guests(t_args *data)
{
	int i;
	int	thread_error;
	pthread_t id[data->number_of_philosophers];
	t_guest philosopher[data->number_of_philosophers];
	
	i = 1;
	thread_error = 0;
	while (i < data->number_of_philosophers)
	{
		philosopher[i].data = data;
        philosopher[i].guest_id = i;
		thread_error = pthread_create(&id[i], NULL, (void *)start_soiree, &philosopher[i]);
		printf("thread %d created\n", i);
		if (thread_error != 0)
		{
			printf("error creating a thread. Exiting...\n");
			return (0);
		}
		pthread_join(id[i], NULL);//makes this func wait until all threads end(all have eate or died);
		i++;
	}
	return (1);
}

//arg checks, initialization and freeing
int	main(int argc, char **argv)
{
	t_args	*data;

	data = NULL;
	if (argc < 5 || argc > 6)
	{
		printf("invalid number of arguments!\n");
		return(0);
	}
	data = lay_the_table(argv);
	if (data == NULL)
	{
		printf("initialization error!\n");
		return(0);
	}
	if (argc == 6)
		data->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	if (receive_guests(data))
		printf("...the spaghetti party is comming to an end...\n");
	destroy_and_free(data, data->number_of_philosophers - 1);
	return (0);
}