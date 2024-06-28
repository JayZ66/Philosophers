/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 18:14:46 by marvin            #+#    #+#             */
/*   Updated: 2024/06/21 18:14:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

// Création des threads.
void	create_philos_threads(t_table *table)
{
	pthread_t	monitor_thread;
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL, routine, &table->philos[i]) != 0)
		{
			write(1, "Pb while creating threads\n", 26);
			return ;
		}
		i++;
	} // Créer un thread en plus sans "données" pour checker la mort avec fonction à part.
	if (pthread_create(&monitor_thread, NULL, &monitor, table) != 0)
	{
		write(1, "Pb while creating threads\n", 26);
		return ;
	}
	pthread_join(monitor_thread, NULL);
	// pthread_detach(monitor_thread);
}

// Join philos threads.
void	join_philo_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		if (pthread_join(table->philos[i].thread, NULL) != 0)
		{
			write(1, "Pb while joining threads\n", 25);
			return ;
		}
		i++;
	}
}

