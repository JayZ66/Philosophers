/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_time.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeguerin <jeguerin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 19:36:40 by jeguerin          #+#    #+#             */
/*   Updated: 2024/07/03 14:51:32 by jeguerin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

// Converting from microseconds to millieseconds.
// Create structure for gettimeofday + using var. to stock the data.
long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

// Recode usleep with the function inside.

void	ft_usleep(int sleep_in_ms)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < sleep_in_ms)
	{
		usleep(100);
	}
}
