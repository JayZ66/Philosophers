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

// - The prog. will receive 4/5 arguments => DONE

// Check if it's an integer ! => DONE
// Limit to 200 philos max. => DONE
// Each nb should be bigger than 0 (except the nb of meals for each philo). => DONE
// Convert arguments into integer ! => DONE

/*
- 1 — The number of philosophers
- 2 — The time a philosopher will die if he doesn’t eat
- 3 — The time it takes a philosopher to eat
- 4 — The time it takes a philosopher to sleep
- 5 — Number of times all the philosophers need to eat before terminating the program (OPTIONAL)
*/

int	is_a_digit(char **argv)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (argv[i])
	{
		if (argv[i] ==  NULL)
			return (1);
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][0] == '-' || argv[i][0] == '+')
				j++;
			if (ft_isdigit(argv[i][j]) == 1) // Check si suffit pour un signe + rien derrière ou pas digit.
				return (1);
			j++;
		}
		i++;	
	}
	return (0);
}

int	manage_errors(int argc, char **argv)
{
	if (argc < 4 || argc > 5)
	{
		printf("Invalid number of arguments\n");
		return (1);
	}
	else if (is_a_digit(argv) == 1)
	{
		printf("Argument needs to be a digit\n");
		return (1);
	}
	return (0);
}

int	*convert_to_digit(char **argv)
{
	int		*args;
	size_t	i;
	size_t	j;

	i = 1;
	args = malloc(sizeof(int) * (ft_strlen_tab(argv) + 1));
	if (!args)
	{
		printf("Problem with memory allocation\n");
		return (NULL);
	}
	j = 0;
	while (argv[i])
	{
		args[j] = ft_atoi(argv[i]);
		j++;
		i++;
	}
	args[j] = '\0';
	return (args);
}

//argv[0] => Max. 200 philosophers.
// Everything need to be higher than 0 (except optional part).
int	check_philo_data(int *args)
{
	size_t	i;

	if (args[0] > 200)
	{
		printf("The nb of philosophers can't be higher than 200\n");
		return (1);
	}
	i = 0;
	while (args[i])
	{
		if (args[i] < 0)
		{
			printf("The time can't be less than 0\n");
			return (1);
		}
		i++;
	}
	return (0);
}

// void	*routine()
// {
	
// }

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
	// For forks need to make a loop for : one philo = one_fork
	size_t	i;

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
	size_t	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		table->philos[i].id = i;
		table->philos[i].nb_of_meals = 0;
		table->philos[i].table = &table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->nb_of_philos];
	}

}

// Création des threads.

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
	return (0);
}
