/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:33:42 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/22 17:39:16 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*soiree(t_guest *philo)
{
	int					round;
	int					times_eaten;

	round = 0;
	times_eaten = 0;
	if (philo->data->num_philos == 1)
		return (ft_one_philo(philo), (void *) NULL);
	while (1)
	{
		if (ft_i_died(philo) || ft_someone_died(philo)
			|| ft_enough_spaghetti(philo, times_eaten, round))
			return (NULL);
		if ((philo->num % 2 == 0 && round % 2 == 0)
			|| (philo->num % 2 != 0 && round % 2 != 0))
		{
			eat_nap_wakeup(philo);
			times_eaten++;
		}
		else
			ft_think(philo);
		round++;
	}
	return (NULL);
}

void	print_error(t_args *data)
{
	pthread_mutex_lock(data->print_mutex);
	printf("error creating thread. Exiting...\n");
	pthread_mutex_unlock(data->print_mutex);
}

t_guest	*init_philos(t_args *data)
{
	int		i;
	t_guest	*philo;

	i = 0;
	philo = (t_guest *)malloc(sizeof(t_guest) * data->num_philos);
	if (philo == NULL)
		return (NULL);
	while (i < data->num_philos)
	{
		philo[i].data = data;
		philo[i].num = i;
		philo[i].id = 0;
		philo[i].times_eaten = 0;
		philo[i].start = get_time(0);
		philo[i].last_meal = philo[i].start;
		if (pthread_create(&philo[i].id, NULL, (void *)soiree, &philo[i]) != 0)
		{
			print_error(data);
			return (free(philo), NULL);
		}
		i++;
	}
	return (philo);
}

//inits & joins threads. Prints defunction time
void	receive_guests(t_args *data)
{
	int				i;
	t_guest			*philo;

	i = 0;
	philo = init_philos(data);
	if (join_threads(philo) != 0)
		return ;
	if (data->time_of_death != 0)
		printf("%lu %d died\n", data->time_of_death, data->someone_died + 1);
	return ;
}

int	main(int argc, char **argv)
{
	int		i;
	t_args	*data;

	i = 1;
	data = NULL;
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
		return (printf("initialization error!\n"), 0);
	if (argc == 6)
		data->times_must_eat = ft_atoi(argv[5]);
	receive_guests(data);
	return (destroy_and_free(data, data->num_philos - 1, 1), 0);
}
