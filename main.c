#include "philo.h"

// int guests = 0;
// pthread_mutex_t guests_mutex = PTHREAD_MUTEX_INITIALIZER;

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

void *start_soiree(t_guest *philosopher)
{
	//int philosopher_id = arg->philosophers_id;
	// pthread_mutex_lock(&guests_mutex); // Lock the mutex before accessing guests
    // guests++;
    printf("...welcome philosopher %d...\n", philosopher->guest_id);
    //pthread_mutex_unlock(&guests_mutex);
	return (NULL);
}

//creates the threads
int	receive_guests(t_args *data)
{
	int i;
	int	thread_error;
	pthread_t id[data->number_of_philosophers];
	t_guest philosopher[data->number_of_philosophers];
	
	i = 0;
	thread_error = 0;
	while (i < data->number_of_philosophers)
	{
		philosopher[i].data = data;
        philosopher[i].guest_id = i;
		thread_error = pthread_create(&id[i], NULL, (void *)start_soiree, &philosopher[i]);
		printf("thread created\n");
		if (thread_error != 0)
		{
			printf("error creating a thread. Exiting...\n");
			return (0);
		}
		i++;
	}
	return (1);
}

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
	{
		printf("...the spaghetti party can start...\n");
		//start_soiree(arg);
	}
	destroy_and_free(data, data->number_of_philosophers - 1);
	//pthread_mutex_destroy(&guests_mutex);
	return (0);
}