/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 14:30:36 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/26 15:58:16 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (str[i] != '\0')
		i++;
	while (i >= 0)
	{
		if (str[i] == c)
			return (&str[i]);
		i--;
	}
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;
	int		i;
	int		y;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	new = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!(new))
		return (NULL);
	i = 0;
	y = 0;
	while (s1[i] != '\0')
	{
		new[i] = s1[i];
		i++;
	}
	while (s2[y] != '\0')
	{
		new[i + y] = s2[y];
		y++;
	}
	new[i + y] = '\0';
	return (new);
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

char	*ft_strdup(const char *s1)
{
	unsigned int	i;
	char			*dest;

	i = 0;
	dest = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (dest == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
