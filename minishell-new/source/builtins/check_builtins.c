/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 16:11:22 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/17 15:39:10 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int ft_check_builtins(char *cmd)
{
	if ((ft_strncmp(cmd, "env", 3)) == 0)
        return (0);
	else if ((ft_strncmp(cmd, "export", 6)) == 0)
        return (0);
    else if ((ft_strncmp(cmd, "unset", 5)) == 0)
        return (0);
	else if ((ft_strncmp(cmd, "echo", 5)) == 0)
		return (0);
	else if ((ft_strncmp(cmd, "pwd", 3)) == 0)
		return (0);
	else if ((ft_strncmp(cmd, "exit", 5)) == 0)
		return (0);
    return (1);
}

int	ft_execute_builtins(t_cmd *cmd, t_env **env, t_export **export)
{
    if (ft_strncmp(cmd[0].user_input[0], "echo", 5) == 0)
		return (ft_echo(cmd[0].user_input));
	if (ft_strncmp(cmd[0].user_input[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd[0].user_input[0], "exit", 5) == 0)
		ft_exit(cmd[0].user_input);
	if ((ft_strncmp(cmd[0].user_input[0], "env", 3)) == 0)
		env_command(cmd[0].user_input, (*env));
	else if ((ft_strncmp(cmd[0].user_input[0], "export", 6)) == 0)
		export_command(cmd[0].user_input, env, export);
    else if ((ft_strncmp(cmd[0].user_input[0], "unset", 5)) == 0)
		unset_command(cmd[0].user_input, env, export);
    return (0);
}
