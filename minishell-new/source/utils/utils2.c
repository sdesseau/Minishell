/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:59:19 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/21 18:59:41 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dst;

	if (s == NULL)
		return (NULL);
	if (ft_strlen(s) < start)
		return (ft_calloc(1, sizeof(char)));
	else
	{
		if (start + len > ft_strlen(s))
			len = ft_strlen(s) - start;
		dst = malloc(sizeof(char) * (len + 1));
		if (dst == NULL)
			return (NULL);
		ft_strlcpy(dst, s + start, len + 1);
		return (dst);
	}
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

char	*find_env_value(char *env_name, t_env *env)
{
	t_env	*tmp;
	int		lenght;

	tmp = env;
	lenght = ft_strlen(env_name);
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->name, env_name, lenght + 1) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
