/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 13:11:14 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/22 17:11:07 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char		*negatif_itoa(unsigned int j, int len)
{
	char			*str;
	unsigned int	jcpy;

	jcpy = j;
	while (j / 10 != 0)
	{
		j = j / 10;
		len++;
	}
	if (!(str = malloc(sizeof(char) * (len + 3))))
		return (NULL);
	len++;
	str[len + 1] = '\0';
	while (len > 0)
	{
		str[len] = ((jcpy % 10) + '0');
		len--;
		jcpy = jcpy / 10;
	}
	str[0] = '-';
	return (str);
}

static char		*positif_itoa(long int j, int len)
{
	char		*str;
	long int	jcpy;

	jcpy = j;
	while (j / 10 != 0)
	{
		j = j / 10;
		len++;
	}
	if (!(str = malloc(sizeof(char) * (len + 2))))
		return (NULL);
	len++;
	str[len] = '\0';
	while (len > 0)
	{
		str[len - 1] = ((jcpy % 10) + '0');
		len--;
		jcpy = jcpy / 10;
	}
	return (str);
}

char			*ft_itoa(int n)
{
	int	len;

	len = 0;
	if (n < 0)
		return (negatif_itoa(-n, len));
	else
		return (positif_itoa(n, len));
}
