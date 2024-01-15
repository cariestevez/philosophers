/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:33:42 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/15 19:38:22 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//main logic
void	*start_soiree(t_guest *philosopher)
{
	int					counter;
	int					times_eaten;
	struct timeval		current_time;
	long unsigned int	wakeup_time_milli;

	counter = 0;
	times_eaten = 0;
	wakeup_time_milli = 0;
    printf("...welcome philosopher %lu...\n", (unsigned long)philosopher->guest_id);
	while (1)
	{
	//check if someone else died. If so, exits
		printf("philo %lu trying to lock someone_died\n", (unsigned long)philosopher->guest_id);
		pthread_mutex_lock(philosopher->data->death_mutex);
		printf("philo %lu locked someone_died\n", (unsigned long)philosopher->guest_id);
		if (philosopher->data->someone_died != 0)
		{
			printf("philo %lu found out someone_died\n", (unsigned long)philosopher->guest_id);
			return (pthread_mutex_unlock(philosopher->data->death_mutex), NULL);
		}
		pthread_mutex_unlock(philosopher->data->death_mutex);
		printf("philo %lu unlocked someone_died\n", (unsigned long)philosopher->guest_id);
	//check if this philosopher is dead. If so, exits
		gettimeofday(&current_time, NULL);//exception for 1st loop bc wakeup_time == 0?
		if (counter > 0 && (((current_time.tv_usec / 1000) - wakeup_time_milli) > (unsigned long)philosopher->data->time_to_die))
		{
			printf("philo %lu trying to lock someone_died because HE'S DYING\n", (unsigned long)philosopher->guest_id);
			pthread_mutex_lock(philosopher->data->death_mutex);
			printf("philo %lu locked someone_died\n", (unsigned long)philosopher->guest_id);
			philosopher->data->someone_died = philosopher->guest_id;
			printf("philo %lu trying to lock time_of_defunction\n", (unsigned long)philosopher->guest_id);
			pthread_mutex_lock(philosopher->data->time_mutex);
			printf("philo %lu locked time_of_defunction\n", (unsigned long)philosopher->guest_id);
			philosopher->data->time_of_defunction = current_time.tv_usec / 1000;
			pthread_mutex_unlock(philosopher->data->death_mutex);
			printf("philo %lu unlocked time_of_defunction\n", (unsigned long)philosopher->guest_id);
			printf("philo %lu unlocking time_of_defunction before returning...\n", (unsigned long)philosopher->guest_id);
			return (pthread_mutex_unlock(philosopher->data->time_mutex), NULL);
		}
	//check if the philo has eaten enough. If so, exits
		if (philosopher->data->number_of_times_each_philosopher_must_eat != 0 && times_eaten == philosopher->data->number_of_times_each_philosopher_must_eat)
		{
			printf("philo %lu trying to lock remaining_guests\n", (unsigned long)philosopher->guest_id);
			pthread_mutex_lock(philosopher->data->remaining_mutex);
			printf("philo %lu locked remaining_guests\n", (unsigned long)philosopher->guest_id);
			philosopher->data->remaining_guests--;
			printf("philo %lu unlocking remaining_guests before returning...\n", (unsigned long)philosopher->guest_id);
			return (pthread_mutex_unlock(philosopher->data->remaining_mutex), NULL);
		}
		//problem when entering here and muting one fork but the other isn't available yet?!
		//will stay blocked in here?
		else
		{
			
			pthread_mutex_lock(philosopher->data->forks[philosopher->order_of_arrival]); // takes the fork on its right
			if (philosopher->order_of_arrival == 0)
				pthread_mutex_lock(philosopher->data->forks[philosopher->data->number_of_philosophers]);
			else
				pthread_mutex_lock(philosopher->data->forks[philosopher->order_of_arrival - 1]);
			gettimeofday(&current_time, NULL);
			printf("%d %lu is eating\n", current_time.tv_usec * 1000000, (unsigned long)philosopher->guest_id);
			usleep(philosopher->data->time_to_eat);
			pthread_mutex_unlock(philosopher->data->forks[philosopher->order_of_arrival]);
			if (philosopher->order_of_arrival == 0)
				pthread_mutex_unlock(philosopher->data->forks[philosopher->data->number_of_philosophers]);
			else
				pthread_mutex_unlock(philosopher->data->forks[philosopher->order_of_arrival - 1]);
			gettimeofday(&current_time, NULL);
			printf("%d %lu is sleeping\n", current_time.tv_usec * 1000000, (unsigned long)philosopher->guest_id);
			usleep(philosopher->data->time_to_sleep);
			gettimeofday(&current_time, NULL);
			wakeup_time_milli = current_time.tv_usec * 1000000;
			printf("%d %lu is thinking\n", current_time.tv_usec * 1000000, (unsigned long)philosopher->guest_id);
		}
		counter++;
	} 
	return (NULL);
}

//creates the threads
int	receive_guests(t_args *data)
{
	int i;
//	pthread_t thread_id[data->number_of_philosophers];
	t_guest philosopher[data->number_of_philosophers];
	
	i = 0;
	while (i < data->number_of_philosophers)
	{
		philosopher[i].data = data;
		philosopher[i].order_of_arrival = i;
		philosopher[i].guest_id = 0;
		philosopher[i].times_eaten = 0;
		if (pthread_create(&philosopher[i].guest_id, NULL, (void *)start_soiree, &philosopher[i]) != 0)
			return (printf("error creating a thread. Exiting...\n"), 0);
		//printf("thread %d with id %lu created\n", i, (long unsigned)philosopher[i].guest_id);
		i++;
	}
	if (join_threads(philosopher) != 0)
		return (0);
	while (1)//maybe do both checks in the same condition?
	{
		pthread_mutex_lock(data->death_mutex);
		if (data->someone_died != 0)
		{
			pthread_mutex_lock(data->time_mutex);
			printf("%d %lu died\n", data->time_of_defunction, (unsigned long)data->someone_died);
			pthread_mutex_unlock(data->time_mutex);
			return (pthread_mutex_unlock(data->death_mutex), 1);
		}
		pthread_mutex_unlock(data->death_mutex);
		pthread_mutex_lock(data->remaining_mutex);
		if (data->remaining_guests == 0)
			return (pthread_mutex_unlock(data->remaining_mutex), 1);
		pthread_mutex_unlock(data->remaining_mutex);
	}
	//return (0);
}

//arg checks, initialization and freeing
int	main(int argc, char **argv)
{
	t_args	*data;

	data = NULL;
	if (argc < 5 || argc > 6)
		return(printf("invalid number of arguments!\n"), 0);
	//other arg checks like times_must_eat > 0 etc?
	data = lay_the_table(argv);
	if (data == NULL)
		return(printf("initialization error!\n"), 0);
	if (argc == 6)
		data->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	if (receive_guests(data))
		printf("...the spaghetti party is comming to an end. Thanks everybody for comming...\n");
	return (destroy_and_free(data, data->number_of_philosophers - 1, 1), 0);
}