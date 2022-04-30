/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 15:24:19 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/29 16:24:58 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	link_new_export_var(t_export **new, char*name, t_export **export)
{
	while ((*export) != NULL)
	{
		if ((*export)->next && ft_strncmp((*export)->next->name, name,
				ft_strlen(name)) == 0)
		{
			(*new)->next = (*export)->next->next;
			free_export_var((*export)->next);
			(*export)->next = (*new);
			break ;
		}
		(*export) = (*export)->next;
	}
}

int	update_export(char *name, char *value, t_export **export)
{
	t_export	*tmp;
	t_export	*new;

	tmp = (*export);
	new = malloc(sizeof(t_export));
	put_in_export(&new, name, value);
	new->is_equal = 1;
	if ((*export) && ft_strncmp((*export)->name, name, ft_strlen(name)) == 0)
	{
		new->next = (*export)->next;
		free_export_var((*export));
		(*export) = new;
		return (0);
	}
	link_new_export_var(&new, name, export);
	if ((*export) == NULL)
		free_export_var(new);
	(*export) = tmp;
	return (0);
}

int	export_var_already_exist(char *name, t_export *exp, char *line, char *val)
{
	t_export	*tmp;

	tmp = exp;
	if (!name || !tmp)
		return (0);
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name) + 1) == 0)
		{
			if (ft_strrchr(line, '=') == NULL)
				return (1);
			update_export(name, val, &exp);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	add_export_var(char *name, char *val, char *line, t_export **export)
{
	t_export	*tmp;
	t_export	*new;

	tmp = (*export);
	if (export_var_already_exist(name, (*export), line, val) == 1)
		return (0);
	new = malloc(sizeof(t_export));
	put_in_export(&new, name, val);
	if (ft_strrchr(line, '=') == NULL)
		new->is_equal = 0;
	else
		new->is_equal = 1;
	if (!(*export))
		(*export) = new;
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
	int		i;
	int		name_len;
	char	*name;
	char	*val;

	i = 0;
	while (envp[i])
	{
		name_len = get_length_name(envp[i]);
		name = ft_substr(envp[i], 0, name_len);
		val = ft_substr(envp[i], name_len + 1, ft_strlen(&envp[i][name_len]));
		add_export_var(name, val, envp[i], export);
		free(name);
		free(val);
		i++;
	}
}
