#include "philo.h"

// int guests = 0;
// pthread_mutex_t guests_mutex = PTHREAD_MUTEX_INITIALIZER;

//destroys initialized mutex objects(forks), frees the forks array and the args structure
void	*destroy_and_free(t_args *arg, int i)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(arg->forks[i]);
		free(arg->forks[i]);
		i--;
	}
	free(arg->forks);
	arg->forks = NULL;
	free(arg);
	arg = NULL;
	return (NULL);
}

//initializes the args struct and the mutex objects(forks)
t_args	*lay_the_table(char **argv)
{
	int	i;
	int	mutex_error;
	t_args	*arg;

	i = 0;
	mutex_error = 0;
	arg = (t_args *)malloc(sizeof(t_args));
	if (arg == NULL)
		return (NULL);
	arg->number_of_philosophers = ft_atoi(argv[1]);
	arg->forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * arg->number_of_philosophers);
	if (arg->forks == NULL)
		return (NULL);
	while (i < arg->number_of_philosophers)
	{
		arg->forks[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (arg->forks[i] == NULL)
			return (destroy_and_free(arg, i));
		mutex_error = pthread_mutex_init(arg->forks[i], NULL);
		if (mutex_error != 0)
			return (destroy_and_free(arg, i));
		i++;
	}
	arg->time_to_die = ft_atoi(argv[2]);
	arg->time_to_eat = ft_atoi(argv[3]);
	arg->time_to_sleep = ft_atoi(argv[4]);
	arg->number_of_times_each_philosopher_must_eat = 0;
	return (arg);
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
int	receive_guests(t_args *arg)
{
	int i;
	int	thread_error;
	pthread_t id[arg->number_of_philosophers];
	t_guest philosopher[arg->number_of_philosophers];
	
	i = 0;
	thread_error = 0;
	while (i < arg->number_of_philosophers)
	{
		philosopher[i].arg = arg;
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
	t_args	*arg;

	arg = NULL;
	if (argc < 5 || argc > 6)
	{
		printf("invalid number of arguments!\n");
		return(0);
	}
	arg = lay_the_table(argv);
	if (arg == NULL)
	{
		printf("initialization error!\n");
		return(0);
	}
	if (argc == 6)
		arg->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	// if (pthread_mutex_init(&guests_mutex, NULL) != 0)
    // {
    //     fprintf(stderr, "Mutex initialization for global guests failed\n");
	// 	destroy_and_free(arg, arg->number_of_philosophers - 1);
    //     return (1);
    // }
	if (receive_guests(arg))
	{
		printf("...the spaghetti party can start...\n");
		//start_soiree(arg);
	}
	destroy_and_free(arg, arg->number_of_philosophers - 1);
	//pthread_mutex_destroy(&guests_mutex);
	return (0);
}