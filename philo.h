#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

//main data structure, shared by all threads
typedef struct s_args
{
	int				number_of_philosophers;//read-only access
	pthread_mutex_t **forks;//-->mutex
	pthread_mutex_t *someone_died_mutex;//-->mutex for someone_died
	pthread_mutex_t *time_of_defunction_mutex;//-->mutex for time_of_defunction
	pthread_mutex_t *remaining_guests_mutex;//-->mutex for remaining_guests
	pthread_t		someone_died;//-->mutex	object
	int				time_of_defunction;//-->mutex object
	int				remaining_guests;//-->mutex object
	int				time_to_die;//read-only access
	int				time_to_eat;//read-only access
	int				time_to_sleep;//read-only access
	int				number_of_times_each_philosopher_must_eat;//read-only access
}   t_args;

//specific for each thread
typedef struct s_guest
{
	t_args			*data;
	int				order_of_arrival;
	pthread_t		guest_id;
	int				times_eaten;
} t_guest;

//main.c
void	*start_soiree(t_guest *philosopher);
int	receive_guests(t_args *data);

//init_and_free.c
void	*destroy_and_free(t_args *data, int i, int flag);
t_args	*lay_the_table(char **argv);
t_args	*last_preparations(t_args *data, char **argv);

//utils.c
int	join_threads(t_guest *philosopher);
int	ft_atoi(const char *nptr);