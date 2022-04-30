/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ft_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 16:11:10 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/29 16:31:23 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_new_name(char *command, int *index)
{
	char	*new_name;
	int		i;

	new_name = NULL;
	i = *index;
	while (command[i] && command[i] != '=')
		i++;
	new_name = ft_substr(command, 0, i);
	if (is_unset_arg_valid(new_name) == 0)
	{
		ft_putstr_fd("export: not a valid identifier\n", 2);
		free(new_name);
		return (NULL);
	}
	if (command[i])
		i++;
	*index = i;
	return (new_name);
}

char	**sort_export(t_export *export)
{
	char	**exp;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	exp = convert_list_to_tab(export);
	while (exp[i])
	{
		j = i + 1;
		while (exp[j])
		{
			if (ft_strncmp(exp[i], exp[j], ft_strlen(exp[i])) > 0)
			{
				tmp = exp[i];
				exp[i] = exp[j];
				exp[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (exp);
}
