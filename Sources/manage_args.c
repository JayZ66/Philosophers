/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 10:04:55 by marvin            #+#    #+#             */
/*   Updated: 2024/06/21 10:04:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

// - The prog. will receive 4/5 arguments => DONE

// Check if it's an integer ! => DONE
// Limit to 200 philos max. => DONE
// Each nb should be bigger than 0 (except the nb of meals for each philo).
// Convert arguments into integer ! => DONE

int	is_a_digit(char **argv)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (argv[i])
	{
		if (argv[i] == NULL)
			return (1);
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][0] == '-' || argv[i][0] == '+')
				j++;
			if (ft_isdigit(argv[i][j]) == 1)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	manage_errors(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
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

int	check_philo_data(char **argv)
{
	size_t	i;

	if (ft_atoi(argv[1]) > 200)
	{
		printf("The nb of philosophers can't be higher than 200\n");
		return (1);
	}
	i = 1;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) < 0)
		{
			printf("The time can't be less than 0\n");
			return (1);
		}
		i++;
	}
	return (0);
}
