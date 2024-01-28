/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:33:42 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/28 23:58:26 by cestevez         ###   ########.fr       */
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

void	*dinner(void *param)
{
	t_guest	*philo;

	philo = (t_guest *)param;
	if (philo->data->num_philos == 1)
		return (ft_one_philo(philo), (void *) NULL);
	while (hungry_and_alive(philo))
	{
		ft_think(philo);
		if (hungry_and_alive(philo) == 0)
			break ;
		ft_eat(philo);
		nap_wakeup(philo);
	}
	return (NULL);
}

int	start_soiree(t_args *data)
{
	int		i;

	i = 0;
	data->ph = ft_calloc(sizeof(t_guest), data->num_philos + 1);
	if (data->ph == NULL)
		return (0);
	printf("...philo struct allocated\n");
	while (i <= data->num_philos)
	{
		data->ph[i].data = data;
		data->ph[i].num = i;
		data->ph[i].meals = 0;
		data->ph[i].eating = 0;
		data->ph[i].id = 0;
		data->ph[i].start = get_time(0);
		data->ph[i].last_meal = data->ph[i].start;
		i++;
	}
	//--->create a create_thread func (spaghetti_party)
	i = 0;
	while (i <= data->num_philos)
	{
		if (i == 0)
			pthread_create(&data->ph[i].id, NULL, waiter, (void *)&data->ph[i]);
		else
			pthread_create(&data->ph[i].id, NULL, dinner, (void *)&data->ph[i]);
		// {
		// 	print_error(data);
		// 	return (free(philo), NULL);
		// }
		i++;
	}//<---
	return (1);
}

//inits & joins threads. Prints defunction time
void	receive_guests(t_args *data)
{
	if (!start_soiree(data))
		return ;
	//call to create threads func?
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
		return (printf("initialization error!\n"), 1);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	receive_guests(data);
	return (destroy_and_free(data, data->num_philos - 1, 1), 0);
}
