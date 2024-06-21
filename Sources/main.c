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

void	*routine()
{
	printf("OK CA MARCHE !");
	exit(EXIT_FAILURE);
}

void	initializing_table(int *args, t_table *table)
{
	table->nb_of_philos = args[0];
	table->time_to_die = args[1];
	table->time_to_eat = args[2];
	table->time_to_sleep = args[3];
	if (args[4])
		table->nb_of_times_philo_has_to_eat = args[4];
	table->dead = 0;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_of_philos);
	table->philos = malloc(sizeof(t_philosophers) * table->nb_of_philos);
	if (!table->forks || !table->philos)
	{
		write(1, "Memory error\n", 13);
		free(args);
		free(table->philos);
		free(table->forks);
	}
}

// Initialisation des mutex
void	initializing_mutexes(t_table *table)
{
	// For forks need to make a loop : one philo = one_fork
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	// Initialize other mutexes.
	pthread_mutex_init(&table->death_mutex, NULL); // + mutex pour check mort chq philo so warning for not reading at the same time.
	pthread_mutex_init(&table->write_mutex, NULL);
	// pthread_mutex_init(&table->eat_mutex, NULL);
}


// Initialisation des philos.
void	initializing_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		table->philos[i].id = i;
		table->philos[i].nb_of_meals = 0;
		table->philos[i].table = table; // Check if reference is needed (&)
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->nb_of_philos];
		i++;
	}
}

// Création des threads.
void	create_philos_threads(t_table *table)
{
	// t_philosophers *philo;
	int	i;

	// philo = table->philos;
	i = 0;
	while (i < table->nb_of_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL, &routine, NULL) != 0)
		{
			write(1, "Pb while creating threads\n", 26);
			return ;
		}
		i++;
	} // Créer un thread en plus sans "données" pour checker la mort avec fonction à part.
} // Creating pthread_join directly after.


// Créer la routine (en castant le param. avec notre struct).

// Join philos threads. (end)

// Détruire mutex (end)

int	main(int argc, char **argv)
{
	int	*args;
	t_table	table;

	if (manage_errors(argc, argv) == 1)
		return (1);
	else
		args = convert_to_digit(argv); // Convert args. into integer.
	if (check_philo_data(args) == 1) // Do i keep argv var. ?
		return (1);
	// Initializing
	initializing_table(args, &table);
	initializing_mutexes(&table);
	initializing_philos(&table);
	// Creating threads.
	create_philos_threads(&table);
	return (0);
}
