#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_args
{
	int	number_of_philosophers;
	pthread_mutex_t **forks;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
}   t_args;

typedef struct s_guest
{
	t_args	*data;
	int		guest_id;
	//int		dead_count;
} t_guest;

//main.c

//ft_atoi.c
int	ft_atoi(const char *nptr);