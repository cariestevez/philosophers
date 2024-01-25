/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:39:17 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/25 21:40:57 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error(t_args *data)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("error creating thread. Exiting...\n");
	pthread_mutex_unlock(&data->print_mutex);
}

int	ft_usleep(uint64_t milliseconds)
{
	size_t	start;

	start = get_time(0);
	while ((get_time(0) - start) < milliseconds)
		usleep(500);
	return (0);
}

uint64_t	get_time(uint64_t start)
{
	struct timeval	current_time;
	uint64_t		sec;
	uint64_t		usec;

	gettimeofday(&current_time, NULL);
	sec = current_time.tv_sec * 1000;
	usec = current_time.tv_usec / 1000;
	if (start == 0)
		return (sec + usec);
	return (sec + usec - start);
}

//joins threads to the main processes' one (waits until all threads end)
int	join_threads(t_args *data)
{
	int	i;
	int	join_return;

	i = 0;
	join_return = 0;
	while (i <= data->num_philos)
	{
		//printf("Entered %d times, joining thread: %lu\n", i, data->philos[i].id);
		join_return = pthread_join(data->philos[i].id, NULL);
		if (join_return != 0)
		{
			printf("Error(%d) joining threads. Exiting...\n", join_return);
			return (1);
		}
		//printf("Threads joined %d times\n", i + 1);
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

int	is_valid(const char *nptr)
{
	int		i;
	char	*str;

	str = (char *)nptr;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			return (0);
		i++;
	}
	while (str[i] != '\0' && (str[i] >= 48 && str[i] <= 57))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = malloc(nmemb * size);
	if (!ptr)
		return (ptr);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	str = s;
	i = 0;
	while (i < n)
	{
		str[i] = '\0';
		i++;
	}
}