/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:50:25 by marvin            #+#    #+#             */
/*   Updated: 2024/06/17 12:50:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <errno.h> // perror - strerror
# include <unistd.h> // access - dup2 - execve - fork - pipe - waitpid
# include <sys/wait.h> // Wait
# include <fcntl.h>
# include <signal.h>
# include <pthread.h>

struct s_table;

typedef struct s_philosophers
{
	int				id;
	int				nb_of_meals;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_table *table;
} t_philosophers;

typedef struct s_table
{
	int				nb_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_of_times_philo_has_to_eat;
	int				dead;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	eat_mutex;
	t_philosophers *philos;
} t_table;


// UTILS
int	ft_isdigit(int c);
int	ft_strlen_tab(char **cmd_line);
int	is_a_digit(char **argv);
int	ft_atoi(const char *nptr);

// MANAGING ARGS
int	manage_errors(int argc, char **argv);
int	*convert_to_digit(char **argv);
int	check_philo_data(int *args);

// INITIALIZATION OF STRUCTURES
void	initializing_table(int *args, t_table *table);
void	initializing_mutexes(t_table *table);
void	initializing_philos(t_table *table);


// MANAGING THREADS
void	create_philos_threads(t_table *table);
void	join_philo_threads(t_table *table, int *args);
void	*routine();

#endif
