/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:10:52 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/26 16:34:34 by mprigent         ###   ########.fr       */
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

void	ft_shlvl(t_env *env)
{
	size_t	i;
	char	*tmp;

	tmp = find_env_value("SHLVL", env);
	if (tmp)
	{
		i = (ft_atoi(tmp) + 1);
		tmp = ft_itoa(i);
		update_env("SHLVL", tmp, &env);
		free(tmp);
	}
}

void	shell_loop(t_env *env, t_export *export)
{
	char	*str;
	t_cmd	*cmd;
	int		i;

	str = NULL;
	cmd = NULL;
	ft_shlvl(env);
	while (1)
	{
		i = 0;
		if (!g_exit_code)
			str = readline("\033[33m$ ➜\033[00m ");
		// else if (g_exit_code == 0)
		// 	str = readline("\033[32m$ ➜ 0\033[00m ");
		// else
		// 	str = readline("\033[31m$ ➜ 1\033[00m ");
		if (str && *str)
			add_history(str);
		cmd = parsing(str, cmd, env, export);
		if (str[0] && cmd)
		{
			run_commands(cmd, &env, &export);
			free_cmd(cmd);
		}
		free(str);
	}
}
