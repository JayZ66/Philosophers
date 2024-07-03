/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeguerin <jeguerin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:30:59 by jeguerin          #+#    #+#             */
/*   Updated: 2024/07/03 14:26:04 by jeguerin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

// long	get_current_time(void)
// {
// 	struct timeval time;

// 	gettimeofday(&time, NULL);
// 	return (time.tv_sec * 1000 + time.tv_usec / 1000);
// }

// void	ft_usleep(long mill)
// {
// 	long	start;

// 	start = get_current_time();
// 	while (get_current_time() - start < mill)
// 	{
// 		usleep(10);
// 	}
// }

// int	check_philo_data2(char *argv[])
// {
// 	size_t	i;

// 	if (ft_atoi(argv[1]) > 200)
// 	{
// 		printf("The nb of philosophers can't be higher than 200\n");
// 		return (1);
// 	}
// 	i = 1;
// 	while (argv[i])
// 	{
// 		if (ft_atoi(argv[i]) < 0)
// 		{
// 			printf("The time can't be less than 0\n");
// 			return (1);
// 		}
// 		i++;int	check_philo_data2(char *argv[])
// {
// 	size_t	i;

// 	if (ft_atoi(argv[1]) > 200)
// 	{
// 		printf("The nb of philosophers can't be higher than 200\n");
// 		return (1);
// 	}
// 	i = 1;
// 	while (argv[i])
// 	{
// 		if (ft_atoi(argv[i]) < 0)
// 		{
// 			printf("The time can't be less than 0\n");
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

// 	}
// 	return (0);
// }

void	initialize_mutex(t_table *table)
{
	int	i;

	i = 0;
	table->forks = malloc(sizeof(t_table) * table->nb_of_philos);
	if (!table->forks)
		return ;
	while (i < table->nb_of_philos)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&table->death_mutex, NULL);
	pthread_mutex_init(&table->meal_mutex, NULL);
	pthread_mutex_init(&table->write_mutex, NULL);
}

void	*monitor2(void *args)
{
	t_table	*table = (t_table *)args;
	int	all_eaten;
	int	death;
	int	i;

	death = table->dead;
	while (death == 0)
	{
		i = 0;
		all_eaten = 1;
		while (i < table->nb_of_philos)
		{
			pthread_mutex_lock(&table->death_mutex);
			if (get_current_time() - table->philos[i].last_meal > table->time_to_die)
			{
				ft_usleep(table->time_to_die);
				pthread_mutex_lock(&table->write_mutex);
				printf("%ld ms: %d died\n", get_current_time() - table->start_time, table->philos[i].id);
				pthread_mutex_unlock(&table->write_mutex);
				table->dead = 1;
				pthread_mutex_unlock(&table->death_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&table->death_mutex);
			if (table->nb_of_times_philo_has_to_eat > 0)
			{
				pthread_mutex_lock(&table->death_mutex);
				if (table->philos[i].nb_of_meals < table->nb_of_times_philo_has_to_eat)
					all_eaten = 0;
				pthread_mutex_unlock(&table->death_mutex);
			}
			i++;
		}
		if (all_eaten && table->nb_of_times_philo_has_to_eat > 0)
		{
			pthread_mutex_lock(&table->write_mutex);
			printf("Philosophers ate the minimun of meals\n");
			pthread_mutex_unlock(&table->write_mutex);
			pthread_mutex_lock(&table->death_mutex);
			table->dead = 1;
			pthread_mutex_unlock(&table->death_mutex);
			return (NULL);
		}
		pthread_mutex_lock(&table->death_mutex);
		death = table->dead;
		pthread_mutex_unlock(&table->death_mutex);
		ft_usleep(1000);
	}
	return (NULL);
}

void	initialize_philo(t_table *table)
{
	int	i;

	i = 0;
	table->philos = malloc(sizeof(t_philosophers) * table->nb_of_philos);
	if (!table->philos)
		return ;
	while (i < table->nb_of_philos)
	{
		table->philos[i].id = i +1;
		table->philos[i].nb_of_meals = 0;
		table->philos[i].table = table;
		table->philos[i].last_meal = get_current_time();
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[i + 1 % table->nb_of_philos];
		i++;
	}
}

void	*routine2(void *args)
{
	t_philosophers	*philo = (t_philosophers *)args;
	int	death;
	
	death = philo->table->dead;
	while (death == 0)
	{
		pthread_mutex_lock(&philo->table->write_mutex);
		printf("%ld ms: %d is thinking\n", get_current_time() - philo->table->start_time, philo->id);
		pthread_mutex_unlock(&philo->table->write_mutex);
		if (death == 1)
			break ;
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			pthread_mutex_lock(&philo->table->write_mutex);
			printf("%ld ms: %d has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
			pthread_mutex_unlock(&philo->table->write_mutex);
			if (death == 1)
			{
				pthread_mutex_unlock(philo->left_fork);
				break ;
			}
			pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(&philo->table->write_mutex);
			printf("%ld ms: %d has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
			pthread_mutex_unlock(&philo->table->write_mutex);
		}
		else
		{
			if (death == 1)
				break ;
			pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(&philo->table->write_mutex);
			printf("%ld ms: %d has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
			pthread_mutex_unlock(&philo->table->write_mutex);
			if (philo->table->nb_of_philos == 1)
			{
				// ft_usleep(philo->table->time_to_die);
				pthread_mutex_lock(&philo->table->write_mutex);
				printf("%ld ms: %d died\n", get_current_time() - philo->table->start_time, philo->id);
				pthread_mutex_unlock(&philo->table->write_mutex);
				pthread_mutex_lock(&philo->table->death_mutex);
				philo->table->dead = 1;
				pthread_mutex_unlock(&philo->table->death_mutex);
				pthread_mutex_unlock(philo->right_fork);
				break ;
			}
			pthread_mutex_lock(philo->left_fork);
			pthread_mutex_lock(&philo->table->write_mutex);
			printf("%ld ms: %d has taken a fork\n", get_current_time() - philo->table->start_time, philo->id);
			pthread_mutex_unlock(&philo->table->write_mutex);
		}

		pthread_mutex_lock(&philo->table->write_mutex);
		printf("%ld ms: %d is eating\n", get_current_time() - philo->table->start_time, philo->id);
		pthread_mutex_unlock(&philo->table->write_mutex);
		pthread_mutex_lock(&philo->table->death_mutex);
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(&philo->table->death_mutex);
		pthread_mutex_lock(&philo->table->meal_mutex);
		philo->nb_of_meals++;
		pthread_mutex_unlock(&philo->table->meal_mutex);
		ft_usleep(philo->table->time_to_eat);

		if (philo->table->nb_of_philos % 2 == 0)
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
		}
		else
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
		}

		pthread_mutex_lock(&philo->table->write_mutex);
		printf("%ld ms: %d is sleeping\n", get_current_time() - philo->table->start_time, philo->id);
		pthread_mutex_unlock(&philo->table->write_mutex);
		ft_usleep(philo->table->time_to_sleep);
		pthread_mutex_lock(&philo->table->death_mutex);
		death = philo->table->dead;
		pthread_mutex_unlock(&philo->table->death_mutex);
	}
	return (NULL);
}

void	create_threads(t_table *table)
{
	pthread_t	monitor_thread;
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL, routine2, &table->philos[i]) != 0)
		{
			printf("Failed creation of threads\n");
			return ;
		}
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor2, table) != 0)
	{
		printf("Failed creation of monitor thread\n");
		return ;
	}
	pthread_join(monitor_thread, NULL);
}

void	join_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_of_philos)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
}

// int	main(int argc, char *argv[])
// {
// 	t_table	table;

// 	// Gestion args.
// 	if (manage_errors(argc, argv) == 1 || check_philo_data2(argv) == 1)
// 		return (1);
// 	// Initializing structures
// 	// Initializing mutexes
// 	initialize_table(&table, argv);
// 	initialize_mutex(&table);
// 	initialize_philo(&table);
// 	// Creating Threads + routine
// 	create_threads(&table);
// 	// Destroying threads
// 	join_threads(&table);
// 	// Destroying mutexes
// 	destroy_mutexes(&table);
// 	// Free everything
// }