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

long long get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long) (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

// Créer la routine (en castant le param. avec notre struct).
void	*routine(void *arg)
{
	t_philosophers	*philo = (t_philosophers *)arg;
	// t_table			*table = philo->table;
	// struct timeval tv;
	int				i;

	i = 0;
	while (1)
	{
		// THINKING
		pthread_mutex_lock(&philo->table->write_mutex); // SEGFAULT !!!
		// ft_printf("ID : %d\n", philo->id);
		write(1, "OK\n", 3);
		pthread_mutex_unlock(&philo->table->write_mutex);
		// pthread_mutex_lock(&table->write_mutex);
		// ft_printf("%d is thinking\n", philo->id); // WARNING TIME STAMP TO PUT !!
		// pthread_mutex_unlock(&table->write_mutex);
		// // TAKING LEFT FORK
		// pthread_mutex_lock(philo->left_fork);
		// pthread_mutex_lock(&table->write_mutex);
		// ft_printf("%d has taken a fork\n", philo->id);
		// pthread_mutex_unlock(&table->write_mutex);
		// // TAKING RIGHT FORK
		// pthread_mutex_lock(philo->right_fork);
		// pthread_mutex_lock(&table->write_mutex);
		// ft_printf("%d has taken a fork\n", philo->id);
		// pthread_mutex_unlock(&table->write_mutex);
		// // EATING
		// pthread_mutex_lock(&table->write_mutex);
		// ft_printf("%d is eating\n", philo->id);
		// pthread_mutex_unlock(&table->write_mutex);
		// philo->nb_of_meals++;
		// gettimeofday(&tv, NULL);
		// philo->last_meal = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
		// usleep(table->time_to_eat);

		// // LEAVE THE FORK
		// pthread_mutex_unlock(philo->left_fork);
		// pthread_mutex_unlock(philo->right_fork);

		// // SLEEPING
		// pthread_mutex_lock(&table->write_mutex);
		// ft_printf("%d is sleeping\n", philo->id);
		// pthread_mutex_unlock(&table->write_mutex);
		// usleep(table->time_to_sleep);
		i++;
		if (i == 5)
			break ;
	}
	return (NULL);
}
// Fork : first left, then right

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

// void	initializing_philos_mutexes(t_table *table)
// {
// 	int	i;

// 	i = 0;
// 	while (i < table->nb_of_philos)
// 	{
// 		pthread_mutex_init(&table->philos[i])
// 	}
// }

// Initialisation des philos.
void	initializing_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		table->philos[i].id = i;
		table->philos[i].nb_of_meals = 0;
		table->philos[i].table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->nb_of_philos];
		i++;
	}
}

// Détruire mutex (end)
void	destroy_mutexes(t_table *table) // CHECK IF RIGHT WAY TO DO IT !
{
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->death_mutex);
	pthread_mutex_destroy(&table->write_mutex);
}

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
	initializing_philos(&table);
	initializing_mutexes(&table);
	// initializing_philos_mutexes(&table);
	// Creating threads.
	create_philos_threads(&table);
	join_philo_threads(&table, args);
	destroy_mutexes(&table);
	free(args);
	free(table.philos);
	free(table.forks);
	return (0);
}
