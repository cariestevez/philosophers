/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:39:17 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/17 18:24:30 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
int	join_threads(t_guest *philosopher)
{
	int	i;
	int	join_return;

	i = 0;
	join_return = 0;
	while (i < philosopher->data->num_philos)
	{
		join_return = pthread_join(philosopher[i].id, NULL);
		if (join_return != 0)
		{
			printf("Error(%d) joining threads. Exiting...\n", join_return);
			return (1);
		}
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
