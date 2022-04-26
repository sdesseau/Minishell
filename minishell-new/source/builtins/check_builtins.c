/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 16:11:22 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/26 15:34:55 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_check_builtins(char *cmd)
{
	if ((ft_strncmp(cmd, "env", 4)) == 0)
		return (0);
	else if ((ft_strncmp(cmd, "export", 7)) == 0)
		return (0);
	else if ((ft_strncmp(cmd, "unset", 6)) == 0)
		return (0);
	else if ((ft_strncmp(cmd, "echo", 5)) == 0)
		return (0);
	else if ((ft_strncmp(cmd, "pwd", 4)) == 0)
		return (0);
	else if ((ft_strncmp(cmd, "exit", 5)) == 0)
		return (0);
	else if ((ft_strncmp(cmd, "cd", 3)) == 0)
		return (0);
	return (1);
}

int	ft_execute_builtins(t_cmd cmd, t_env **env, t_export **export)
{
	if (ft_strncmp(cmd.user_input[0], "echo", 5) == 0)
		return (ft_echo(cmd.user_input));
	if (ft_strncmp(cmd.user_input[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd.user_input[0], "exit", 5) == 0)
		return (ft_exit(cmd.user_input));
	if (ft_strncmp(cmd.user_input[0], "cd", 3) == 0)
		ft_cd(cmd.user_input, env);
	if ((ft_strncmp(cmd.user_input[0], "env", 4)) == 0)
		return (env_command(cmd.user_input, (*env)));
	else if ((ft_strncmp(cmd.user_input[0], "export", 7)) == 0)
		return (export_command(cmd.user_input, env, export));
	else if ((ft_strncmp(cmd.user_input[0], "unset", 6)) == 0)
		return (unset_command(cmd.user_input, env, export));
	return (0);
}
