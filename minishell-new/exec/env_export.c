/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:52:39 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 14:35:55 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
    
    while (envp[i])
    {
        name_length = get_length_name(envp[i]);
		name = ft_substr(envp[i], 0, name_length);
		value = ft_substr(envp[i], name_length + 1,
				ft_strlen(&envp[i][name_length]));
		if (add_env_variable(name, value, env) == 1)
			printf("error %d\n", __LINE__);
		free(name);
		free(value);
		i++;
	}
    }
}