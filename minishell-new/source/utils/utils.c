/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 14:30:36 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/24 16:32:16 by sdesseau         ###   ########.fr       */
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
	unsigned int	nbr;

	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			ft_putchar_fd('-', fd);
			n = n * (-1);
		}
		nbr = n;
		if (n > 9)
			ft_putnbr_fd((n / 10), fd);
		ft_putchar_fd(((n % 10) + 48), fd);
	}
}

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	if (c == '\0')
		return (&str[i]);
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*dest;
	unsigned int	i;

	i = 0;
	if (!s1 && !s2)
		return (NULL);
	dest = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (dest == NULL)
		return (NULL);
	if (s1)
	{
		while (*s1 != '\0')
			dest[i++] = *s1++;
	}
	if (s2)
	{
		while (*s2 != '\0')
			dest[i++] = *s2++;
		dest[i] = '\0';
	}
	return (dest);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	if (dstsize < (size_t)ft_strlen(dst))
		return (dstsize + (size_t)ft_strlen(src));
	if (dstsize == 0)
		return ((size_t)ft_strlen(src));
	while (dst[i] != '\0')
		i++;
	if (dstsize <= i)
		return (dstsize + (size_t)ft_strlen(src));
	k = i;
	while (src[j] && i < dstsize - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (k + (size_t)ft_strlen(src));
}
