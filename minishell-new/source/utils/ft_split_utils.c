/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 22:21:12 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/21 22:21:59 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_count_word(char *str, char c)
{
	int		i;
	int		word;

	i = 0;
	word = 0;
	while (str[i])
	{
		if (str[i] != c && i == 0)
		{
			word++;
			i++;
		}
		if (str[i] != c && str[i - 1] == c)
			word++;
		i++;
	}
	return (word);
}

int	ft_len_word(char *str, char c)
{
	int		len;

	len = 0;
	while (str[len])
	{
		if (str[len] != c)
			len++;
		else
			break ;
	}
	return (len);
}

char	*ft_dup(char *src, char c)
{
	int		i;
	int		len;
	char	*dst;

	i = 0;
	len = ft_len_word(src, c);
	dst = malloc(sizeof(*src) * (len + 1));
	if (dst == NULL)
		return (NULL);
	while (src[i] && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	**ft_free(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}
