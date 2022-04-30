/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_putfd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 22:00:53 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/21 22:02:19 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_putchar_fd(char c, int fd)
{
	if (fd >= 0)
		write(fd, &c, 1);
}

void	ft_putstr_fd(char *str, int fd)
{
	if (fd >= 0)
	{
		while (str && *str)
		{
			ft_putchar_fd(*str, fd);
			str++;
		}
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			ft_putchar_fd('-', fd);
			n = n * (-1);
		}
		if (n > 9)
			ft_putnbr_fd((n / 10), fd);
		ft_putchar_fd(((n % 10) + 48), fd);
	}
}
