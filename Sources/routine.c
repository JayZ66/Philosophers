/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeguerin <jeguerin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:46:19 by jeguerin          #+#    #+#             */
/*   Updated: 2024/07/03 17:22:26 by jeguerin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	print_state(t_table *table, int p_id, t_type state)
{
	const char	*status;

	if (state == THINKING)
		status = "is thinking";
	else if (state == TAKING_FORK)
		status = "has taken a fork";
	else if (state == EATING)
		status = "is eating";
	else if (state == SLEEPING)
		status = "is sleeping";
	else if (state == DEAD)
		status = "died";
	else
		status = NULL;
	pthread_mutex_lock(&table->stop_mutex);
	if (table->dead == 0)
	{
		pthread_mutex_lock(&table->write_mutex);
		printf("%ld ms: %d %s\n", get_time() - table->start_time, p_id, status);
		pthread_mutex_unlock(&table->write_mutex);
		return (pthread_mutex_unlock(&table->stop_mutex), 0);
	}
	else
		return (pthread_mutex_unlock(&table->stop_mutex), 1);
	return (2);
}

void	philo_is_eating(t_philosophers *philo)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(&table->death_mutex);
	philo->last_meal = get_time();
	print_state(table, philo->id, EATING);
	pthread_mutex_unlock(&table->death_mutex);
	ft_usleep(table->time_to_eat);
	pthread_mutex_lock(&philo->table->meal_mutex);
	philo->nb_of_meals++;
	pthread_mutex_unlock(&philo->table->meal_mutex);
}

int	philo_is_sleeping(t_philosophers *philo, int death)
{
	t_table	*table;

	table = philo->table;
	if (print_state(table, philo->id, SLEEPING) == 1)
		return (1);
	ft_usleep(table->time_to_sleep);
	pthread_mutex_lock(&philo->table->stop_mutex);
	death = philo->table->dead;
	pthread_mutex_unlock(&philo->table->stop_mutex);
	return (death);
}

int	philo_takes_his_forks(t_philosophers *philo)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(philo->left_fork);
	if (print_state(table, philo->id, TAKING_FORK) == 1)
		return (pthread_mutex_unlock(philo->left_fork), 1);
	if (philo->left_fork == philo->right_fork)
	{
		ft_usleep(philo->table->time_to_die);
		pthread_mutex_unlock(philo->left_fork);
		if (print_state(table, philo->id, DEAD) == 1)
			return (1);
		pthread_mutex_lock(&table->stop_mutex);
		philo->table->dead = 1;
		pthread_mutex_unlock(&table->stop_mutex);
		return (1);
	}
	pthread_mutex_lock(philo->right_fork);
	if (print_state(table, philo->id, TAKING_FORK) == 1)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philosophers	*philo;
	t_table			*table;
	int				death;

	philo = (t_philosophers *)arg;
	table = philo->table;
	death = philo->table->dead;
	while (death == 0)
	{
		if (print_state(table, philo->id, THINKING) == 1)
			break ;
		if (philo_takes_his_forks(philo) == 1)
			break ;
		philo_is_eating(philo);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		death = philo_is_sleeping(philo, death);
		if (death == 1)
			break ;
	}
	return (NULL);
}
