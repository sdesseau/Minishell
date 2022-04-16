/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:52:39 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 17:06:57 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	put_in_env(t_env **new, char *name, char *value)
{
	(*new)->name = ft_strdup(name);
	(*new)->value = ft_strdup(value);
	
        printf("env name >> %s\n", (*new)->name);
        printf("env value >> %s\n", (*new)->value);
	(*new)->next = NULL;
}

void	add_env_var(char *name, char *value, t_env **env)
{
	t_env	*tmp;
	t_env	*new;

	tmp = (*env);
	// if (env_var_already_exist(name, env) == 1)   -> quand export sera fait
	// 	update_env(name, value, env);
	new = malloc(sizeof(t_env));
	put_in_env(&new, name, value);
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

int     get_length_name(char *envp)
{
    int		len;

	len = 0;
	while (envp[len] != '\0' && envp[len] != '=')
		len++;
	return (len);
}

void    recup_env(char **envp, t_env **env)
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
		val = ft_substr(envp[i], name_len+ 1, ft_strlen(&envp[i][name_len]));
		add_env_var(name, val, env);
		free(name);
		free(val);
		i++;
	}
}