/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:33:42 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/25 00:14:12 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	hungry_and_alive(t_guest *philo)
{
	int	flag;

	flag = 1;
	pthread_mutex_lock(&philo->data->enddinner_mutex);
	if (philo->data->end_dinner != 0)
		flag = 0;
	pthread_mutex_unlock(&philo->data->enddinner_mutex);
	return (flag);
}

void	*dinner(t_guest *philo)
{
	int	round;

	round = 0;
	if (philo->data->num_philos == 1)
		return (ft_one_philo(philo), (void *) NULL);
	while (hungry_and_alive(philo))
	{
		if ((philo->num % 2 == 0 && round % 2 != 0)
			|| (philo->num % 2 != 0 && round % 2 == 0))
			ft_think(philo);
		else
		{
			ft_eat(philo);
			if (hungry_and_alive(philo) == 0)
				return (NULL);
			nap_wakeup(philo);
		}
		round++;
	}
	return (NULL);
}

t_guest	*start_soiree(t_args *data)
{
	int		i;
	t_guest	*philo;

	i = 0;
	philo = (t_guest *)malloc(sizeof(t_guest) * data->num_philos + 1);
	if (philo == NULL)
		return (NULL);
	printf("...philo struct allocated\n");
	data->philos = philo;
	while (i <= data->num_philos)
	{
		philo[i].data = data;
		philo[i].num = i;
		philo[i].meals = 0;
		philo[i].eating = 0;
		philo[i].id = 0;
		philo[i].start = get_time(0);
		philo[i].last_meal = philo[i].start;
		if (i == 0)
			pthread_create(&philo[i].id, NULL, (void *)waiter, &philo[i]);
		else
			pthread_create(&philo[i].id, NULL, (void *)dinner, &philo[i]);
		// {
		// 	print_error(data);
		// 	return (free(philo), NULL);
		// }
		i++;
	}
	return (philo);
}

//inits & joins threads. Prints defunction time
t_guest	*receive_guests(t_args *data)
{
	t_guest			*philo;

	philo = start_soiree(data);
	if (philo == NULL)
		return (NULL);
	if (join_threads(philo) != 0)
	{
		destroy_and_free(philo, data, data->num_philos - 1, 1);
		return (NULL);
	}
	return (philo);
}

int	main(int argc, char **argv)
{
	int		i;
	t_args	*data;
	t_guest	*philo;

	i = 1;
	data = NULL;
	philo = NULL;
	if (argc < 5 || argc > 6)
		return (printf("invalid number of arguments!\n"), 0);
	if (ft_atoi(argv[i]) == 0)
		return (printf("num of philosophers must be > 0!\n"), 0);
	while (i < argc)
	{
		if (is_valid(argv[i]) == 0)
			return (printf("argument of invalid type!\n"), 0);
		i++;
	}
	data = lay_the_table(argv);
	if (data == NULL)
		return (printf("initialization error!\n"), 1);
	if (argc == 6)
		data->times_must_eat = ft_atoi(argv[5]);
	philo = receive_guests(data);
	if (philo == NULL)
		return (1);
	return (destroy_and_free(philo, data, data->num_philos - 1, 1), 0);
}
