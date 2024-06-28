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

void print_status(t_philosophers *philo, const char *status, const char *color) {
    long timestamp = get_current_time();
    pthread_mutex_lock(&philo->table->write_mutex);
    if (!philo->table->dead) {
        printf("%s%ld %d %s%s\n", color, timestamp, philo->id, status, COLOR_RESET);
    }
    pthread_mutex_unlock(&philo->table->write_mutex);
}

void	*monitor(void *arg)
{
	// t_philosophers	*philo;
	t_table			*table;
	int				i;

	table = (t_table *)arg;
	while (!table->dead)
	{
		// if (check_death(table) == 1)
		// {
		// 	ft_usleep(table->time_to_die);
		// 	pthread_mutex_lock(&table->write_mutex);
		// 	ft_printf("%d ms: %d died\n", get_current_time() - table->start_time, table->philos[i].id);
		// 	pthread_mutex_unlock(&table->write_mutex);
		// 	return (NULL);
		// }
		i = 0;
		while (i < table->nb_of_philos)
		{
			// pthread_mutex_lock(&table->write_mutex);
			// printf("Start time : %d\n", table->start_time);
			// printf("Current time : %ld\n", get_current_time());
			// printf("Last eat time : %ld\n", philo->last_meal);
			// printf("time : %ld\n", time);
			// printf("Time - philo : %ld\n", time - philo->last_meal);
			// pthread_mutex_unlock(&table->write_mutex);
			pthread_mutex_lock(&table->death_mutex);
			if ((get_current_time() - table->philos[i].last_meal) > table->time_to_die)
			{
				table->dead = 1;
				pthread_mutex_unlock(&table->death_mutex);
				pthread_mutex_lock(&table->write_mutex);
				ft_printf("%d ms: %d died\n", get_current_time() - table->start_time, table->philos[i].id);
				pthread_mutex_unlock(&table->write_mutex);
			}
			pthread_mutex_unlock(&table->death_mutex);
			// pthread_mutex_unlock(&table->death_mutex);
			// if (table->dead == 1)
			// {
			// 	// ft_usleep(table->time_to_die);
			// 	break ;
			// }
			// pthread_mutex_lock(&table->meal_mutex);
			if (table->nb_of_times_philo_has_to_eat != -1 && (table->philos[i].nb_of_meals >= table->nb_of_times_philo_has_to_eat))
			{
				table->all_eaten++;
				pthread_mutex_lock(&table->death_mutex);
				printf("Philo : %d - Nb who eated full : %d\n", table->philos[i].id, table->all_eaten);
				printf("Nb of meals : %d of %d\n", table->philos[i].nb_of_meals, table->philos[i].id);
				pthread_mutex_unlock(&table->death_mutex);
			}
			// printf("Nb times to eat : %d\n", table->nb_of_times_philo_has_to_eat);
			// printf("Nb of meals : %d of %d\n", table->philos[i].nb_of_meals, table->philos[i].id);
			if (table->all_eaten == table->nb_of_philos)
			{
				printf("OK\n");
				// pthread_mutex_unlock(&table->meal_mutex);
				pthread_mutex_lock(&table->death_mutex);
				table->dead = 1;
				pthread_mutex_unlock(&table->death_mutex);
				break ;
			}
			// pthread_mutex_unlock(&table->meal_mutex);
			i++;
		}
		usleep(1000);
	}
	return (arg);
}

void	*routine(void *arg)
{
	t_philosophers	*philo;
	t_table			*table;
	// struct timeval	current;

	philo = (t_philosophers *)arg;
	table = philo->table;
	while (!table->dead)
	{
		// pthread_mutex_lock(&table->death_mutex);
		// if (table->dead == 1)
		// {
		// 	ft_usleep(table->time_to_die);
		// 	pthread_mutex_unlock(&table->death_mutex);
		// 	break ;
		// }
		// pthread_mutex_unlock(&table->death_mutex);
		// THINKING
		pthread_mutex_lock(&table->write_mutex);
		ft_printf("%d ms: %d is thinking\n", get_current_time() - table->start_time, philo->id);
		pthread_mutex_unlock(&table->write_mutex);
		// TAKING LEFT FORK
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			pthread_mutex_lock(&table->write_mutex);
			ft_printf("%d ms: %d has taken a fork\n", get_current_time() - table->start_time, philo->id);
			pthread_mutex_unlock(&table->write_mutex);
			// if (check_death(table) == 1)
			// {
			// 	ft_usleep(table->time_to_die);
			// 	pthread_mutex_unlock(philo->left_fork);
			// 	break ;
			// }
			pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(&table->write_mutex);
			ft_printf("%d ms: %d has taken a fork\n", get_current_time() - table->start_time, philo->id);
			pthread_mutex_unlock(&table->write_mutex);
		}
		else // Works without but longer !
		{
			pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(&table->write_mutex);
			ft_printf("%d ms: %d has taken a fork\n", get_current_time() - table->start_time, philo->id);
			pthread_mutex_unlock(&table->write_mutex);
			if (table->nb_of_philos == 1)
			{
				ft_usleep(table->time_to_die);
				table->dead = 1;
				pthread_mutex_unlock(philo->right_fork);
				return (NULL);
			}
			pthread_mutex_lock(philo->left_fork);
			pthread_mutex_lock(&table->write_mutex);
			ft_printf("%d ms: %d has taken a fork\n", get_current_time() - table->start_time, philo->id);
			pthread_mutex_unlock(&table->write_mutex);
		}
		// EATING
		pthread_mutex_lock(&table->death_mutex);
		// philo->nb_of_meals++;
		philo->last_meal = get_current_time();
		pthread_mutex_lock(&table->write_mutex);
		ft_printf("%d ms: %d is eating\n", get_current_time() - table->start_time, philo->id);
		pthread_mutex_unlock(&table->write_mutex);
		pthread_mutex_unlock(&table->death_mutex);
		ft_usleep(table->time_to_eat);

		// pthread_mutex_lock(&philo->table->meal_mutex);
        philo->nb_of_meals++;
        // pthread_mutex_unlock(&philo->table->meal_mutex);
		// LEAVE THE FORK
		if (philo->id % 2 == 0)
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
		}
		else
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
		}
		// pthread_mutex_lock(&table->death_mutex);
		// if (table->dead == 1)
		// {
		// 	ft_usleep(table->time_to_die);
		// 	pthread_mutex_unlock(&table->death_mutex);
		// 	break ;
		// }
		// pthread_mutex_unlock(&table->death_mutex);
		// SLEEPING
		pthread_mutex_lock(&table->write_mutex);
		ft_printf("%d ms: %d is sleeping\n", get_current_time() - table->start_time, philo->id);
		pthread_mutex_unlock(&table->write_mutex);
		ft_usleep(table->time_to_sleep);
		// usleep(1000);
	}
	return (arg);
}


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
	table->all_eaten = 0;
	table->start_time = get_current_time();
	table->philos = malloc(sizeof(t_philosophers) * table->nb_of_philos);
	if (!table->philos)
	{
		write(1, "Memory error\n", 13);
		// free(args);
		// free(table->philos);
		// free(table->forks);
		exit(1);
	}
}

// Initialisation des mutex
void	initializing_mutexes(t_table *table)
{
	// For forks need to make a loop : one philo = one_fork
	int	i;

	i = 0;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_of_philos);
	if (!table->forks)
	{
		write(1, "Memory error\n", 13);
		// free(args);
		free(table->philos);
		exit(1);
	}
	while (i < table->nb_of_philos)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	// Initialize other mutexes.
	pthread_mutex_init(&table->meal_mutex, NULL);
	pthread_mutex_init(&table->write_mutex, NULL);
	pthread_mutex_init(&table->death_mutex, NULL);
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
		table->philos[i].table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->nb_of_philos];
		table->philos[i].last_meal = get_current_time();
		// pthread_mutex_init(&table->philos[i].meal_mutex, NULL);
		i++;
	}
}

// Détruire mutex (end)
void	destroy_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->write_mutex);
	pthread_mutex_destroy(&table->meal_mutex);
	pthread_mutex_destroy(&table->death_mutex);
}

int	main(int argc, char **argv)
{
	int		*args;
	t_table	table;
	// pthread_t	monitor_thread;

	// table = malloc(sizeof(t_table));
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
	// if (pthread_create(&monitor_thread, NULL, &monitor, &table) != 0)
	// {
	// 	write(1, "Pb while creating threads\n", 26);
	// 	return (1);
	// }
	// pthread_join(monitor_thread, NULL);
	join_philo_threads(&table);
	free(args);
	free(table.philos);
	destroy_mutexes(&table);
	free(table.forks);
	return (0);
}
