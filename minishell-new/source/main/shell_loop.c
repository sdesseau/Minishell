/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:10:52 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/29 16:33:04 by mprigent         ###   ########.fr       */
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
	char	*str;
	t_cmd	*cmd;

	str = NULL;
	cmd = NULL;
	while (1)
	{
		str = readline("\033[33m🐚 ➜\033[00m ");
		if (str && *str)
			add_history(str);
		cmd = parsing(str, cmd, env, export);
		if (str && str[0] && cmd)
		{
			run_commands(cmd, &env, &export);
			free_cmd(cmd);
		}
		free(str);
	}
}
