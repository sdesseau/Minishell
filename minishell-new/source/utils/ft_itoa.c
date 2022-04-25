/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 13:11:14 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/25 17:01:50 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int		ft_len(long n)
{
	int		len;

	len = 1;
	if (n < 0)
	{
		n *= -1;
		len++;
	}
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void		ft_write(long n, int len, char *str)
{
	int		start;

	start = 0;
	str[len] = '\0';
	len--;
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
		start = 1;
	}
	while (len >= start)
	{
		str[len] = n % 10 + 48;
		n /= 10;
		len--;
	}
}

char			*ft_itoa(long n)
{
	int		count_char;
	char	*str;

	count_char = ft_len(n);
	if ((str = (char *)malloc(sizeof(char) * count_char + 1)) == NULL)
		return (NULL);
	ft_write(n, count_char, str);
	return (str);
}
