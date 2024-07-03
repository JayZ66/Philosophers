/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:49:06 by marvin            #+#    #+#             */
/*   Updated: 2024/06/17 12:49:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

// void	initializing_table(int *args, t_table *table)
// {
// 	table->nb_of_philos = args[0];
// 	table->time_to_die = args[1];
// 	table->time_to_eat = args[2];
// 	table->time_to_sleep = args[3];
// 	if (args[4])
// 		table->nb_of_times_philo_has_to_eat = args[4];
// 	else
// 		table->nb_of_times_philo_has_to_eat = -1;
// 	table->dead = 0;
// 	table->all_eaten = 0;
// 	table->start_time = get_current_time();
// 	table->philos = malloc(sizeof(t_philosophers) * table->nb_of_philos);
// 	if (!table->philos)
// 	{
// 		write(1, "Memory error\n", 13);
// 		return ;
// 	}
// }

// Initializing
// Creating threads
// Destroying threads & mutexes
int	main(int argc, char **argv)
{
	t_table	table;

	if (manage_errors(argc, argv) == 1 || check_philo_data(argv) == 1)
		return (1);
	initialize_table(&table, argv);
	initializing_mutexes(&table);
	initializing_philos(&table);
	create_philos_threads(&table);
	join_philo_threads(&table);
	free(table.philos);
	destroy_mutexes(&table);
	free(table.forks);
	return (0);
}
