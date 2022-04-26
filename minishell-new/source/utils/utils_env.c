/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:57:50 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/26 15:56:10 by mprigent         ###   ########.fr       */
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

char	*find_env_value(char *env_name, t_env *env)
{
	t_env	*tmp;
	int		lenght;

	tmp = env;
	lenght = ft_strlen(env_name);
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->name, env_name, lenght + 1) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
