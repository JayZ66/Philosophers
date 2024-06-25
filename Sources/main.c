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

int	check_death(t_table *table)
{
	pthread_mutex_lock(&table->death_mutex);
	if (table->dead == 1)
	{
        pthread_mutex_unlock(&table->death_mutex);
        return (1);
    }
    pthread_mutex_unlock(&table->death_mutex);
	return (0);
}

void	*monitor(void *arg)
{
	t_philosophers	*philo;
	t_table			*table;
	int				i;
	long			time;

	table = (t_table *)arg;
	while (1)
	{
		if (check_death(table) == 1)
			return (NULL);
		i = 0;
		while (i < table->nb_of_philos)
		{
			philo = &table->philos[i];
			pthread_mutex_lock(&table->meal_mutex);
			time = get_current_time();
			if ((time - philo->last_meal) > table->time_to_die)
			{
				pthread_mutex_unlock(&table->meal_mutex);
				pthread_mutex_lock(&table->death_mutex);
				table->dead = 1;
				pthread_mutex_unlock(&table->death_mutex);
				pthread_mutex_lock(&table->write_mutex);
				ft_printf("%d : %d died\n", get_current_time(), philo->id);
				pthread_mutex_unlock(&table->write_mutex);
				break ;
			}
			pthread_mutex_lock(&table->meal_mutex);
			if (philo->nb_of_meals >= table->nb_of_times_philo_has_to_eat)
			{
				pthread_mutex_unlock(&table->meal_mutex);
				pthread_mutex_lock(&table->death_mutex);
				table->dead = 1;
				pthread_mutex_unlock(&table->death_mutex);
				break ;
			}
			pthread_mutex_unlock(&table->meal_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

// Créer la routine (en castant le param. avec notre struct).
void	*routine(void *arg)
{
	t_philosophers	*philo;
	t_table			*table;

	philo = (t_philosophers *)arg;
	table = philo->table;
	while (1)
	{
		if (check_death(table) == 1)
			return (NULL);
		// THINKING
		pthread_mutex_lock(&table->write_mutex);
		ft_printf("%d : %d is thinking\n", get_current_time(), philo->id);
		pthread_mutex_unlock(&table->write_mutex);
		// TAKING LEFT FORK
		pthread_mutex_lock(philo->left_fork); // CHECK AUTRE SENS FORKS !!
		pthread_mutex_lock(&table->write_mutex);
		ft_printf("%d : %d has taken a fork\n", get_current_time(), philo->id);
		pthread_mutex_unlock(&table->write_mutex);
		if (table->nb_of_philos == 1)
		{
			usleep(table->time_to_die * 1000);
			pthread_mutex_unlock(philo->left_fork);
			return (NULL);
		}
		// TAKING RIGHT FORK
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&table->write_mutex);
		ft_printf("%d : %d has taken a fork\n", get_current_time(), philo->id);
		pthread_mutex_unlock(&table->write_mutex);
		// EATING
		pthread_mutex_lock(&table->write_mutex);
		ft_printf("%d : %d is eating\n", get_current_time(), philo->id);
		pthread_mutex_unlock(&table->write_mutex);
		pthread_mutex_lock(&table->meal_mutex);
		philo->nb_of_meals++;
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(&table->meal_mutex);
		usleep(table->time_to_eat * 1000);
		// LEAVE THE FORK
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		// SLEEPING
		pthread_mutex_lock(&table->write_mutex);
		ft_printf("%d : %d is sleeping\n", get_current_time(), philo->id);
		pthread_mutex_unlock(&table->write_mutex);
		usleep(table->time_to_sleep * 1000);
	}
	return (arg);
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
	else
		table->nb_of_times_philo_has_to_eat = -1;
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
	pthread_mutex_init(&table->death_mutex, NULL);
	pthread_mutex_init(&table->meal_mutex, NULL);
	pthread_mutex_init(&table->write_mutex, NULL);
}

// Initialisation des philos.
void	initializing_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].nb_of_meals = 0;
		table->philos[i].last_meal = get_current_time();
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
	pthread_mutex_destroy(&table->meal_mutex);
}

int	main(int argc, char **argv)
{
	int		*args;
	t_table	table;

	// table = malloc(sizeof(t_table));
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
	// Creating threads.
	create_philos_threads(&table);
	join_philo_threads(&table);
	destroy_mutexes(&table);
	free(args);
	free(table.forks);
	free(table.philos);
	return (0);
}
