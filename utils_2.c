/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 15:09:32 by cestevez          #+#    #+#             */
/*   Updated: 2024/02/05 15:51:14 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_write(t_guest *philo, int eating)
{
	pthread_mutex_lock(&philo->data->enddinner_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (philo->data->end_dinner == 0 && eating == 0)
		printf("%lu %d has taken a fork\n", get_time(philo->start), philo->num);
	else if (philo->data->end_dinner == 0 && eating == 1)
		printf("%lu %d is eating\n", get_time(philo->start), philo->num);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->enddinner_mutex);
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
