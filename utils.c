#include "philo.h"

// int	detach_threads(t_guest *philosopher)
// {
// 	int	i;

// 	i = 0;
// 	while (i < philosopher->data->number_of_philosophers)
// 	{
// 		pthread_detach(philosopher[i].guest_id);
// 		i++;
// 	}
// }

//joins threads to the main processes' one 
//so this waits until all threads end(all philos have eaten or one died)
int	join_threads(t_guest *philosopher)
{
	int	i;
	int	join_return;

	i = 0;
	join_return = 0;
	while (i < philosopher->data->number_of_philosophers)
	{
		join_return = pthread_join(philosopher[i].guest_id, NULL);
		//printf("JOINED THREAD %lu\n", (unsigned long)philosopher[i].guest_id);
		if (join_return != 0)
			return (printf("Error(%d) joining threads. Exiting...\n", join_return), 1);
		i++;
	}
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int		i;
	int		sign;
	int		number;
	char	*str;

	str = (char *)nptr;
	i = 0;
	number = 0;
	sign = 1;
	if (str[i] == '\0')
		return (0);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		number = number * 10 + (str[i] - 48);
		i++;
	}
	return (number * sign);
}