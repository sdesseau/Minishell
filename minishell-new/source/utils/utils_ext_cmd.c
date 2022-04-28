/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ext_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 22:29:12 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/28 18:21:11 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_tab(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	if (tab)
		free(tab);
}

int	ft_size_env(t_env *lst)
{
	int		i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

char	*ft_convert_env(char *name, char *value)
{
	char	*final_str;
	int		name_size;
	int		value_size;
	int		size_final_str;

	if (!name || !value)
		return (NULL);
	final_str = NULL;
	name_size = ft_strlen(name);
	value_size = ft_strlen(value);
	size_final_str = name_size + value_size + 1;
	final_str = (char *)malloc(sizeof(char) * size_final_str + 1);
	if (!final_str)
		return (NULL);
	ft_strlcpy(final_str, name, name_size + 1);
	ft_strlcat(final_str, "=", size_final_str + 1);
	ft_strlcat(final_str, value, size_final_str + 1);
	return (final_str);
}

char	**ft_conv_env_to_tab(t_env *env)
{
	t_env	*temp;
	char	**tab;
	int		len;
	int		i;

	i = 0;
	len = ft_size_env(env);
	temp = env;
	tab = (char **)calloc(sizeof(char *), (len + 1));
	if (tab == NULL)
		return (NULL);
	while (temp != NULL)
	{
		tab[i] = ft_convert_env(temp->name, temp->value);
		temp = temp->next;
		i++;
	}
	return (tab);
}
