/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:10:52 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/19 23:01:58 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd[i].index != -1)
	{
		stop_loop(cmd[i]);
		i++;
	}
	free(cmd);
}

void	shell_loop(t_env *env, t_export *export)
{
	char		*str;
	t_cmd	*cmd;
	int			i;

	str = NULL;
	cmd = NULL;
	while (1)
	{
		i = 0;
		str = readline("\033[33m$ ➜\033[00m ");
		if (str && *str)
			add_history(str);
		cmd = parsing(str, cmd, env);
		if (str[0] && cmd)
		{
            // handle_exit_command(cmd[0]);
			run_commands(cmd, &env, &export);
			// ft_execute_builtins(cmd, &env, &export);
			free_cmd(cmd);
			// printf("mgmmh\n");
		}
		free(str);
	}
}
