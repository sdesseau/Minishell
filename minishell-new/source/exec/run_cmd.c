/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:45:28 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/26 15:44:50 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exec_single_cmd(t_cmd cmd, t_env **env, t_export **export, int tmp)
{
	dup2(cmd.fd_stdin, STDIN_FILENO);
	if (cmd.nb_chevrons > 0)
		cmd.fd_stdout = output(cmd.path, tmp);
	else
		cmd.fd_stdout = dup(tmp);
	if ((ft_check_builtins(cmd.user_input[0])) == 0)
	{
		dup2(cmd.fd_stdout, 1);
		g_exit_code = ft_execute_builtins(cmd, env, export);
		close(cmd.fd_stdin);
		close(cmd.fd_stdout);
	}
	else
		launch_child_process(cmd, (*env), (*export));
	close(cmd.fd_stdin);
	close(cmd.fd_stdout);
}

void	is_chevrons(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->input = 0;
	cmd->output = 0;
	if (cmd->nb_chevrons > 0)
	{
		while (cmd->path[i])
		{
			if (cmd->path[i][0] == '>')
				cmd->output = 1;
			else if (cmd->path[i][0] == '<')
				cmd->input = 1;
			i++;
		}
	}
	if (cmd->input == 1)
		cmd->fd_stdin = input(cmd->path, 0);
	else
		cmd->fd_stdin = dup(0);
}

void	ft_pipe(t_cmd *cmd)
{
	pipe(cmd->fd_pipe);
	if (cmd->input == 1)
		cmd->fd_pipe[0] = input(cmd->path, cmd->fd_stdin);
	if (cmd->output == 1)
		cmd->fd_pipe[1] = output(cmd->path, cmd->fd_stdout);
	cmd->fd_stdin = cmd->fd_pipe[0];
	cmd->fd_stdout = cmd->fd_pipe[1];
}

void	loop_cmd(int nb_cmd, t_cmd *cmd, t_env **env, t_export **export)
{
	int	i;

	i = 0;
	while (i < nb_cmd)
	{
		is_chevrons(&cmd[i]);
		if (cmd[i].fd_stdin == -1)
			break ;
		dup2(cmd[i].fd_stdin, STDIN_FILENO);
		if (i < nb_cmd - 1)
			ft_pipe(&cmd[i]);
		else
		{
			if (cmd[i].output == 1)
				cmd[i].fd_stdout = output(cmd[i].path, 1);
			else
				cmd[i].fd_stdout = dup(1);
		}
		launch_child_process(cmd[i], (*env), (*export));
		close(cmd[i].fd_stdin);
		close(cmd[i].fd_stdout);
		i++;
	}
}

void	run_commands(t_cmd *cmd, t_env **env, t_export **export)
{
	int	tmp_stdin;
	int	tmp_stdout;
	int	nb_cmd;

	nb_cmd = nb_of_pipe(cmd);
	tmp_stdin = dup(0);
	tmp_stdout = dup(1);
	if (nb_cmd == 1)
	{
		if (cmd[0].nb_chevrons > 0)
			cmd[0].fd_stdin = input(cmd[0].path, tmp_stdin);
		else
			cmd[0].fd_stdin = dup(tmp_stdin);
		if (cmd[0].fd_stdin == -1)
			return ;
		exec_single_cmd(cmd[0], env, export, tmp_stdout);
	}
	else
		loop_cmd(nb_cmd, cmd, env, export);
	dup2(tmp_stdin, STDIN_FILENO);
	dup2(tmp_stdout, STDOUT_FILENO);
	close(tmp_stdin);
	close(tmp_stdout);
}
