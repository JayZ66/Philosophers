/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_time.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeguerin <jeguerin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:36:40 by jeguerin          #+#    #+#             */
/*   Updated: 2024/07/01 16:18:45 by jeguerin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

// Converting from microseconds to millieseconds.
// Create structure for gettimeofday + using var. to stock the data.
long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

// Recode usleep with the function inside.

void	ft_usleep(int sleep_in_ms)
{
	long	start;

<<<<<<< Updated upstream
    start = get_current_time();
    while ((get_current_time() - start) < sleep_in_ms)
    {
        usleep(50);
    }
}
=======
	start = get_current_time();
	while ((get_current_time() - start) < sleep_in_ms)
	{
		usleep(100);
	}
}

// void    print_state(t_table *table, char *status)
// {

// }
>>>>>>> Stashed changes
