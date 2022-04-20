/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 15:24:19 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/17 13:03:32 by sdesseau         ###   ########.fr       */
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

int	export_var_already_exist(char *name, t_export *export)
{
	t_export	*tmp;

	tmp = export;
	if (!name || !tmp)
		return (0);
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name) + 1) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	add_export_var(char *name, char *val, char *line, t_export **export)
{
	t_export	*tmp;
	t_export	*new;

	tmp = (*export);
	if (export_var_already_exist(name, (*export)) == 1)
	{
		if (ft_strrchr(line, '=') == NULL)
			return (0);
		update_export(name, val, export);
		return (0);
	}
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
