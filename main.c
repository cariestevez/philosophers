/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:33:42 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/25 21:45:48 by cestevez         ###   ########.fr       */
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
	//printf("Guest %d finish the while loop with flag %d\n", philo->num, flag);
	return (flag);
}

void	*dinner(void *param)
{
	t_guest	*philo;

	philo = (t_guest *)param;
	//printf("Guest %d ID: %lu\n", philo->num, philo->id);
	if (philo->data->num_philos == 1)
		return (ft_one_philo(philo), (void *) NULL);
	while (hungry_and_alive(philo))
	{
		ft_think(philo);
		if (hungry_and_alive(philo) == 0)
			break;
		ft_eat(philo);
		nap_wakeup(philo);
	}
	//printf("Guest %d finish the while loop\n", philo->num);
	return (NULL);
}

int	start_soiree(t_args *data)
{
	int		i;

	i = 0;
	data->philos = ft_calloc(sizeof(t_guest), data->num_philos + 1);
	if (data->philos == NULL)
		return (0);
	printf("...philo struct allocated\n");
	while (i <= data->num_philos)
	{
		data->philos[i].data = data;
		data->philos[i].num = i;
		data->philos[i].meals = 0;
		data->philos[i].eating = 0;
		data->philos[i].id = 0;
		data->philos[i].start = get_time(0);
		data->philos[i].last_meal = data->philos[i].start;
		i++;
	}
	
	i = 0;
	while (i <= data->num_philos)
	{
		//printf("%lu\n", data->philos[3].id);
		//printf("Philo id was %lu\n", philo[2].id);
		if (i == 0)
			pthread_create(&data->philos[i].id, NULL, waiter, (void *)&data->philos[i]);
		else
			pthread_create(&data->philos[i].id, NULL, dinner, (void *)&data->philos[i]);
		// {
		// 	print_error(data);
		// 	return (free(philo), NULL);
		// }
		i++;
	}
	return (1);
}

//inits & joins threads. Prints defunction time
void	receive_guests(t_args *data)
{
	if (!start_soiree(data))
		return ;
	if (join_threads(data) != 0)
	{
		destroy_and_free(data, data->num_philos - 1, 1);
		return ;
	}
	return ;
}

int	main(int argc, char **argv)
{
	int		i;
	t_args	*data;
//	t_guest	*philo;

	i = 1;
	data = NULL;
//	philo = NULL;
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
	receive_guests(data);
	//if (philo == NULL)
		//return (1);
	return (destroy_and_free(data, data->num_philos - 1, 1), 0);
}
