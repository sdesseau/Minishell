/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 15:24:19 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 17:06:31 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	put_in_export(t_export **new, char *name, char *value)
{
	(*new)->name = ft_strdup(name);
	(*new)->value = ft_strdup(value);
	(*new)->next = NULL;
}

int	add_export_var(char *name, char *val, char *line, t_export **export)
{
	t_export	*tmp;
	t_export	*new;

	tmp = (*export);
	// if (export_variable_already_exist(name, (*export)) == 1) quand export sera fait
	// {
	// 	if (ft_strrchr(content, '=') == NULL)
	// 		return (0);
	// 	update_export(name, val, export);
	// 	return (0);
	// }
	new = malloc(sizeof(t_export));
	put_in_export(&new, name, val);
	if (ft_strrchr(line, '=') == NULL)
		new->is_equal = 0;
	else
		new->is_equal = 1;
	if (!(*export))
	{
		(*export) = new;
		return (0);
	}
	else
	{
		while ((*export)->next)
			(*export) = (*export)->next;
		(*export)->next = new;
		(*export) = tmp;
	}
	return (0);
}

void	recup_export(char **envp, t_export **export)
{
	int     i;
    int     name_len;
    char    *name;
    char    *val;

	i = 0;
	while (envp[i])
	{
        name_len = get_length_name(envp[i]);
		name = ft_substr(envp[i], 0, name_len);
		val = ft_substr(envp[i], name_len + 1, ft_strlen(&envp[i][name_len]));
		if (add_export_var(name, val, envp[i], export) == 1)
			printf("error %d\n", __LINE__);
		free(name);
		free(val);
		i++;
	}
}