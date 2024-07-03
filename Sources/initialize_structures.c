/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_structures.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeguerin <jeguerin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:42:21 by jeguerin          #+#    #+#             */
/*   Updated: 2024/07/03 15:39:30 by jeguerin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

// Initialisation des philos.
void	initializing_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].nb_of_meals = 0;
		table->philos[i].table = table;
		if (table->philos[i].id % 2 == 0)
		{
			table->philos[i].left_fork = &table->forks[i];
			table->philos[i].right_fork = &table->forks[(i + 1)
				% table->nb_of_philos];
		}
		else
		{
			table->philos[i].left_fork = &table->forks[(i + 1)
				% table->nb_of_philos];
			table->philos[i].right_fork = &table->forks[i];
		}
		table->philos[i].last_meal = 0;
		i++;
	}
}

void	initialize_table(t_table *table, char *argv[])
{
	table->nb_of_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		table->nb_of_times_philo_has_to_eat = ft_atoi(argv[5]);
	else
		table->nb_of_times_philo_has_to_eat = -1;
	table->all_eaten = 0;
	table->dead = 0;
	table->start_time = get_time();
	table->philos = malloc(sizeof(t_philosophers) * table->nb_of_philos);
	if (!table->philos)
	{
		write(1, "Memory error\n", 13);
		return ;
	}
}
