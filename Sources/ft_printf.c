/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 18:02:45 by marvin            #+#    #+#             */
/*   Updated: 2024/06/23 18:02:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	print_format(const char spe, va_list args)
{
	int	count;

	count = 0;
	if (spe == 'c')
		count += ft_putchar(va_arg(args, int));
	else if (spe == 's')
		count += ft_putstr(va_arg(args, char *));
	else if (spe == 'd')
		count += ft_putnb(va_arg(args, int));
	else if (spe == 'x')
		count += ft_putnb_hexa(va_arg(args, unsigned int), "0123456789abcdef");
	else if (spe == 'X')
		count += ft_putnb_hexa(va_arg(args, unsigned int), "0123456789ABCDEF");
	else if (spe == 'i')
		count += ft_putnb(va_arg(args, int));
	else if (spe == 'u')
		count += ft_unsigned_putnbr(va_arg(args, unsigned int));
	else if (spe == '%')
		count += ft_putchar('%');
	else if (spe == 'p')
		count += ft_putnb_hexa_add(va_arg(args, void *), "0123456789abcdef");
	return (count);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		count;
	va_list	args;

	va_start(args, str);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == 37)
		{
			i++;
			count += print_format(str[i], args);
		}
		else
			count += write(1, &str[i], 1);
		i++;
	}
	va_end(args);
	return (count);
}

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
	{
		write(1, "(null)", 6);
		i = 6;
		return (i);
	}
	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (i);
}
