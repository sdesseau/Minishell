/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:57:50 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/25 18:22:20 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	*free_env_var(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
	return (0);
}

int	get_length_name(char *envp)
{
	int	len;

	len = 0;
	while (envp[len] != '\0' && envp[len] != '=')
		len++;
	return (len);
}

void	put_in_env(t_env **new, char *name, char *value)
{
	(*new)->name = ft_strdup(name);
	(*new)->value = ft_strdup(value);
	(*new)->next = NULL;
}
