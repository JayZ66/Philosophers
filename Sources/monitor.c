/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeguerin <jeguerin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:47:31 by jeguerin          #+#    #+#             */
/*   Updated: 2024/07/03 17:24:29 by jeguerin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	is_philo_dead(t_table *table, int i)
{
	pthread_mutex_lock(&table->death_mutex);
	if (table->philos[i].last_meal != 0 && (get_time()
			- table->philos[i].last_meal)
		> table->time_to_die)
	{
		print_state(table, table->philos[i].id, DEAD);
		pthread_mutex_lock(&table->stop_mutex);
		table->dead = 1;
		pthread_mutex_unlock(&table->stop_mutex);
	}
	pthread_mutex_lock(&table->stop_mutex);
	if (table->dead == 1)
	{
		pthread_mutex_unlock(&table->stop_mutex);
		pthread_mutex_unlock(&table->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&table->stop_mutex);
	pthread_mutex_unlock(&table->death_mutex);
	return (0);
}

int	is_philo_full(t_table *table, int all_eaten, int i)
{
	if (table->nb_of_times_philo_has_to_eat > 0)
	{
		pthread_mutex_lock(&table->meal_mutex);
		if (table->philos[i].nb_of_meals
			< table->nb_of_times_philo_has_to_eat)
			all_eaten = 0;
		pthread_mutex_unlock(&table->meal_mutex);
	}
	return (all_eaten);
}

int	philo_are_full(t_table *table, int all_eaten)
{
	if (all_eaten == 1 && table->nb_of_times_philo_has_to_eat > 0)
	{
		pthread_mutex_lock(&table->death_mutex);
		if (table->dead == 0)
		{
			pthread_mutex_unlock(&table->death_mutex);
			pthread_mutex_lock(&table->write_mutex);
			printf("All philosophers ate\n");
			pthread_mutex_unlock(&table->write_mutex);
		}
		else
			pthread_mutex_unlock(&table->death_mutex);
		pthread_mutex_lock(&table->stop_mutex);
		table->dead = 1;
		pthread_mutex_unlock(&table->stop_mutex);
		return (1);
	}
	else
		return (0);
}

int	update_death(t_table *table, int death)
{
	pthread_mutex_lock(&table->stop_mutex);
	death = table->dead;
	pthread_mutex_unlock(&table->stop_mutex);
	usleep(1000);
	return (death);
}

void	*monitor(void *arg)
{
	t_table			*table;
	int				i;
	int				death;
	int				all_eaten;

	table = (t_table *)arg;
	death = table->dead;
	while (death == 0)
	{
		i = 0;
		all_eaten = 1;
		while (i < table->nb_of_philos)
		{
			if (is_philo_dead(table, i) == 1)
				break ;
			all_eaten = is_philo_full(table, all_eaten, i);
			if (check_death(table) == 1)
				break ;
			i++;
		}
		if (philo_are_full(table, all_eaten) == 1)
			break ;
		death = update_death(table, death);
	}
	return (NULL);
}
