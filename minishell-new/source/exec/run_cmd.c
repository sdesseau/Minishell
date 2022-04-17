/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:45:28 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/17 13:13:02 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void    internal_command(t_cmd *cmd, t_env **env, t_export **export)
{
	if ((ft_strncmp(cmd[0].user_input[0], "env", 3)) == 0)
		env_command(cmd[0].user_input, (*env));
	else if ((ft_strncmp(cmd[0].user_input[0], "export", 6)) == 0)
		export_command(cmd[0].user_input, env, export);
    else if ((ft_strncmp(cmd[0].user_input[0], "unset", 5)) == 0)
		unset_command(cmd[0].user_input, env, export);
}
        
int     is_builtin(char *cmd)
{
	if ((ft_strncmp(cmd, "env", 3)) == 0)
        return (0);
	else if ((ft_strncmp(cmd, "export", 6)) == 0)
        return (0);
    else if ((ft_strncmp(cmd, "unset", 5)) == 0)
        return (0);
    return (1);
}

void    run_commands(t_cmd *cmd, t_env **env, t_export **export)
{
    if ((is_builtin(cmd[0].user_input[0])) == 0)
        internal_command(cmd, env, export);
    // else
    //     external_cmd(cmd, env, export);
}