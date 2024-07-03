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
# include <stdarg.h>
# include <string.h>
# include <time.h>
# include <sys/time.h>
# include <errno.h> // perror - strerror
# include <unistd.h> // access - dup2 - execve - fork - pipe - waitpid
# include <sys/wait.h> // Wait
# include <fcntl.h>
# include <signal.h>
# include <pthread.h>

struct	s_table;

typedef enum e_type
{
	THINKING,
	TAKING_FORK,
	EATING,
	SLEEPING,
	DEAD
}	t_type;

typedef struct s_philosophers
{
	int				id;
	int				nb_of_meals;
	long int		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_table	*table;
}	t_philosophers;

typedef struct s_table
{
	int				nb_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long int		start_time;
	int				nb_of_times_philo_has_to_eat;
	int				dead;
	int				all_eaten;
	pthread_mutex_t	*forks;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	stop_mutex;
	t_philosophers	*philos;
}	t_table;

// UTILS
int		ft_isdigit(int c);
int		ft_strlen_tab(char **cmd_line);
int		is_a_digit(char **argv);
int		ft_atoi(const char *nptr);

// MANAGING TIME
long	get_time(void);
void	ft_usleep(int sleep_in_ms);


// MANAGING ARGS
int		manage_errors(int argc, char **argv);
int		*convert_to_digit(char **argv);
int		check_philo_data(char *argv[]);

// INITIALIZATION OF STRUCTURES
void	initializing_table(int *args, t_table *table);
void	initialize_table(t_table *table, char *argv[]);
void	initializing_mutexes(t_table *table);
void	initializing_philos(t_table *table);

// MANAGING THREADS
void	create_philos_threads(t_table *table);
void	join_philo_threads(t_table *table);
void	*routine(void *arg);
void	*monitor(void *arg);

void	destroy_mutexes(t_table *table);
int	check_death(t_table *table);

int	print_state(t_table *table, int p_id, t_type state);

// PRINTF
int		count_nb(int n, int base);
int		count_unsigned_nb(unsigned int n, int base);
int		count_unsigned_nb_add(unsigned long int n, int base);
int		print_format(const char spe, va_list args);
int		ft_printf(const char *str, ...);
int		ft_putchar(char c);
int		ft_putstr(char *str);
int		ft_unsigned_putnbr(unsigned int nb);
int		ft_putnb(int nb);
int		ft_putnb_hexa(unsigned int nb, char *base);
int		ft_putnb_hexa_add(void *ptr, char *base);




#endif
