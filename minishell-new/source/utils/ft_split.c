/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 16:48:21 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/21 22:21:55 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**ft_split_and_fill_array(char *str, char c, char **tab)
{
	int		i;

	i = 0;
	while (*str)
	{
		if (*str != c)
		{
			tab[i] = ft_dup(str, c);
			if (tab[i] == NULL)
				return (ft_free(tab));
			i++;
			str += ft_len_word(str, c);
		}
		else
			str++;
	}
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	*str;
	char	**tab;
	int		words;

	if (s == NULL)
		return (NULL);
	str = (char *)s;
	words = ft_count_word(str, c);
	tab = ft_calloc(words + 1, sizeof(*tab));
	if (tab == NULL)
		return (NULL);
	tab = ft_split_and_fill_array(str, c, tab);
	return (tab);
}
