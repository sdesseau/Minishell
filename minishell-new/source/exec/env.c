/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:52:39 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/29 16:25:08 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	link_new_env_var(t_env **new, char*name, t_env **env)
{
	while ((*env) != NULL)
	{
		if ((*env)->next && ft_strncmp((*env)->next->name,
				name, ft_strlen(name)) == 0)
		{
			(*new)->next = (*env)->next->next;
			free_env_var((*env)->next);
			(*env)->next = (*new);
			break ;
		}
		(*env) = (*env)->next;
	}
}

int	update_env(char *name, char *val, t_env **env)
{
	t_env	*tmp;
	t_env	*new;

	tmp = (*env);
	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	put_in_env(&new, name, val);
	if ((*env) && ft_strncmp((*env)->name, name, ft_strlen(name)) == 0)
	{
		new->next = (*env)->next;
		free_env_var((*env));
		(*env) = new;
		return (0);
	}
	link_new_env_var(&new, name, env);
	if (!(*env))
		free_env_var(new);
	(*env) = tmp;
	return (0);
}

int	env_var_already_exist(char *name, t_env *env)
{
	t_env	*tmp;

	tmp = env;
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

void	add_env_var(char *name, char *val, t_env **env)
{
	t_env	*tmp;
	t_env	*new;

	tmp = (*env);
	if (env_var_already_exist(name, (*env)) == 1)
		update_env(name, val, env);
	new = malloc(sizeof(t_env));
	put_in_env(&new, name, val);
	if (!(*env))
		(*env) = new;
	else
	{
		while ((*env)->next)
			(*env) = (*env)->next;
		(*env)->next = new;
		(*env) = tmp;
	}
}

void	recup_env(char **envp, t_env **env)
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
		add_env_var(name, val, env);
		free(name);
		free(val);
		i++;
	}
}
