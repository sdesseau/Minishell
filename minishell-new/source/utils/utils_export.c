/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:55:25 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/21 19:01:56 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	put_in_export(t_export **new, char *name, char *value)
{
	(*new)->name = ft_strdup(name);
	(*new)->value = ft_strdup(value);
	(*new)->next = NULL;
}

void	*free_export_var(t_export *export)
{
	free(export->name);
	free(export->value);
	free(export);
	return (0);
}

int	is_equal_sign(char *name)
{
	int		j;

	j = 0;
	while (name[j])
	{
		if (ft_strchr(name, '=') == 0)
			return (0);
		j++;
	}
	return (1);
}
int	ft_lenlist(t_export *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

char	**convert_list_to_tab(t_export *export)
{
	t_export	*tmp;
	char		**tab;
	int			i;
	int			len;

	i = 0;
	len = ft_lenlist(export);
	tmp = export;
	tab = (char **)calloc(sizeof(char *), (len + 1));
	if (!tab)
		return (NULL);
	while (tmp)
	{
		tab[i] = ft_strdup(tmp->name);
		tmp = tmp->next;
		i++;
	}
	return (tab);
}
