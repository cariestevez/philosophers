/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:33:42 by cestevez          #+#    #+#             */
/*   Updated: 2024/02/05 16:16:17 by cestevez         ###   ########.fr       */
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

int	receive_guests(t_args *data)
{
	int		i;

	i = 0;
	data->philo = ft_calloc(sizeof(t_guest), data->num_philos + 1);
	if (data->philo == NULL)
		return (0);
	while (i <= data->num_philos)
	{
		data->philo[i].data = data;
		data->philo[i].num = i;
		data->philo[i].meals = 0;
		data->philo[i].eating = 0;
		data->philo[i].id = 0;
		data->philo[i].start = get_time(0);
		data->philo[i].last_meal = data->philo[i].start;
		i++;
	}
	return (1);
}

//inits & joins threads. Prints defunction time
void	start_soiree(t_args *data)
{
	if (!receive_guests(data))
		return ;
	if (!guests_to_table(data))
	{
		destroy_and_free(data, data->num_philos - 1, 1);
		return ;
	}
	if (wait_to_finish(data) != 0)
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
		return (printf("num of philoophers must be > 0!\n"), 0);
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
	start_soiree(data);
	return (destroy_and_free(data, data->num_philos - 1, 1), 0);
}
