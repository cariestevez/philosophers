/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:33:42 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/16 01:08:26 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	eat_nap_wakeup(t_guest *philosopher)
{
	struct timeval		current_time;

	pthread_mutex_lock(philosopher->data->forks[philosopher->order_of_arrival - 1]); // takes the fork on its right
	//printf("philo %lu took fork %d\n", (unsigned long)philosopher->guest_id, philosopher->order_of_arrival);
	if (philosopher->order_of_arrival == 0)
	{
		//write(1, "philo 0 trying to get both forks\n", 33);
		pthread_mutex_lock(philosopher->data->forks[philosopher->data->number_of_philosophers - 1]);
		//printf("philo %lu took fork %d\n", (unsigned long)philosopher->guest_id, philosopher->data->number_of_philosophers - 1);
	}
	else
	{
		//write(1, "other even trying to get both forks\n", 36);
		pthread_mutex_lock(philosopher->data->forks[philosopher->order_of_arrival - 1]);
		//printf("philo %lu took fork %d\n", (unsigned long)philosopher->guest_id, philosopher->order_of_arrival - 1);
	}
	gettimeofday(&current_time, NULL);
	printf("%d %d is eating\n", current_time.tv_usec, philosopher->order_of_arrival + 1);
	usleep(philosopher->data->time_to_eat);
	pthread_mutex_unlock(philosopher->data->forks[philosopher->order_of_arrival]);
	if (philosopher->order_of_arrival == 0)
		pthread_mutex_unlock(philosopher->data->forks[philosopher->data->number_of_philosophers - 1]);
	else
		pthread_mutex_unlock(philosopher->data->forks[philosopher->order_of_arrival - 1]);
	gettimeofday(&current_time, NULL);
	printf("%d %d is sleeping\n", current_time.tv_usec, philosopher->order_of_arrival + 1);
	usleep(philosopher->data->time_to_sleep);
	gettimeofday(&current_time, NULL);
	return (current_time.tv_usec);
}

//main logic
void	*start_soiree(t_guest *philosopher)
{
	int					round;
	int					times_eaten;
	struct timeval		current_time;
	long unsigned int	wakeup_time;

	round = 0;
	times_eaten = 0;
	wakeup_time = 0;
   // printf("...welcome philosopher %lu order of arrival: %d...\n", (unsigned long)philosopher->guest_id, philosopher->order_of_arrival);
	while (1)
	{
	//check if this philosopher is dead. If so, exits
		gettimeofday(&current_time, NULL);
		wakeup_time = current_time.tv_usec;
		//write(1, "check 1\n", 8);
		if (current_time.tv_usec - wakeup_time > (unsigned long)philosopher->data->time_to_die)
		{
			//printf("philo %lu trying to lock someone_died because HE'S DYING\n", (unsigned long)philosopher->guest_id);
			pthread_mutex_lock(philosopher->data->someone_died_mutex);
			//printf("philo %lu locked someone_died\n", (unsigned long)philosopher->guest_id);
			philosopher->data->someone_died = philosopher->guest_id;
			//printf("philo %lu trying to lock time_of_defunction\n", (unsigned long)philosopher->guest_id);
			pthread_mutex_lock(philosopher->data->time_of_defunction_mutex);
			//printf("philo %lu locked time_of_defunction\n", (unsigned long)philosopher->guest_id);
			philosopher->data->time_of_defunction = current_time.tv_usec;
			pthread_mutex_unlock(philosopher->data->someone_died_mutex);
			//printf("philo %lu unlocked someone_died and unlocking time_of_defunction before returning...\n", (unsigned long)philosopher->guest_id);
			return (pthread_mutex_unlock(philosopher->data->time_of_defunction_mutex), NULL);
		}
	//check if someone else died. If so, exits
		//printf("philo %lu trying to lock someone_died\n", (unsigned long)philosopher->guest_id);
		//write(1, "check 2\n", 8);
		pthread_mutex_lock(philosopher->data->someone_died_mutex);
		//printf("philo %lu locked someone_died\n", (unsigned long)philosopher->guest_id);
		if (philosopher->data->someone_died != 0)
		{
			//printf("philo %lu found out someone_died. Unlocking someone_died and returning...\n", (unsigned long)philosopher->guest_id);
			return (pthread_mutex_unlock(philosopher->data->someone_died_mutex), NULL);
		}
		pthread_mutex_unlock(philosopher->data->someone_died_mutex);
		//printf("philo %lu unlocked someone_died\n", (unsigned long)philosopher->guest_id);
	//check if the philo has eaten enough. If so, exits
		//write(1, "check 3\n", 8);
		if (philosopher->data->number_of_times_each_philosopher_must_eat != 0 && times_eaten == philosopher->data->number_of_times_each_philosopher_must_eat)
		{
			//printf("philo %lu trying to lock remaining_guests\n", (unsigned long)philosopher->guest_id);
			pthread_mutex_lock(philosopher->data->remaining_guests_mutex);
			//printf("philo %lu locked remaining_guests\n", (unsigned long)philosopher->guest_id);
			philosopher->data->remaining_guests--;
			//printf("philo %lu unlocking remaining_guests before returning...\n", (unsigned long)philosopher->guest_id);
			return (pthread_mutex_unlock(philosopher->data->remaining_guests_mutex), NULL);
		}
		else
		{
			if ((philosopher->order_of_arrival % 2 == 0 && round % 2 == 0)
				|| (philosopher->order_of_arrival % 2 != 0 && round % 2 != 0))
			{
				wakeup_time = eat_nap_wakeup(philosopher);
				times_eaten++;
			}
			else
				printf("%d %d is thinking\n", current_time.tv_usec, philosopher->order_of_arrival);
		}
		round++;
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
		philosopher[i].order_of_arrival = i + 1;
		philosopher[i].guest_id = 0;
		philosopher[i].times_eaten = 0;
		if (pthread_create(&philosopher[i].guest_id, NULL, (void *)start_soiree, &philosopher[i]) != 0)
			return (printf("error creating a thread. Exiting...\n"), 0);
		//printf("thread %d with id %lu created\n", i, (long unsigned)philosopher[i].guest_id);
		i++;
	}
	if (join_threads(philosopher) != 0)
		return (0);
	while (1)
	{
		pthread_mutex_lock(data->someone_died_mutex);
		pthread_mutex_lock(data->remaining_guests_mutex);
		write(1, "MAIN\n", 5);
		if (data->someone_died != 0 || data->remaining_guests == 0)
		{
			if (data->someone_died != 0)
			{
				pthread_mutex_lock(data->time_of_defunction_mutex);
				printf("%d %lu died\n", data->time_of_defunction, (unsigned long)data->someone_died);
				pthread_mutex_unlock(data->time_of_defunction_mutex);
			}
			pthread_mutex_unlock(data->someone_died_mutex);
			return (pthread_mutex_unlock(data->remaining_guests_mutex), 1);
		}
		pthread_mutex_unlock(data->someone_died_mutex);
		pthread_mutex_unlock(data->remaining_guests_mutex);
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