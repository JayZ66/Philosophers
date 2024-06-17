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

void	*routine()
{
	printf("Test from threads\n");
	sleep(3);
	printf("Ending thread\n");
	return (NULL);
}

int	main(int argc, char **argv)
{
	int	*args;

	if (manage_errors(argc, argv) == 1)
		return (1);
	else
		args = convert_to_digit(argv); // Convert args. into integer.
	if (check_philo_data(args) == 1)
		return (1);
	pthread_t	t1;
	if (pthread_create(&t1, NULL, &routine, NULL) != 0)
		return (1);
	if (pthread_join(t1, NULL) != 0)
		return (2);
	return (0);
}
