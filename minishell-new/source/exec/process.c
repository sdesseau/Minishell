/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 19:37:43 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/25 19:39:00 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	child_process(t_cmd cmd, t_env *env, t_export *export)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup2(cmd.fd_stdout, STDOUT_FILENO);
	close(cmd.fd_stdout);
	close(cmd.fd_stdin);
	if ((ft_check_builtins(cmd.user_input[0])) == 1)
		ft_execute_external_cmd(cmd.user_input, env);
	else if (ft_check_builtins(cmd.user_input[0]) == 0)
		ft_execute_builtins(cmd, &env, &export);
	kill(0, SIGQUIT);
}

void	parent_process(t_cmd cmd, pid_t pid)
{
	dup2(cmd.fd_stdin, STDIN_FILENO);
	waitpid(pid, &pid, 0);
	close(cmd.fd_stdin);
	close(cmd.fd_stdout);
	if (WIFEXITED(pid))
		g_exit_code = WEXITSTATUS(pid);
	if (WIFSIGNALED(pid))
	{
		g_exit_code = WTERMSIG(pid);
		if (g_exit_code != 131)
			g_exit_code += 128;
	}	
}

void	launch_child_process(t_cmd cmd, t_env *env, t_export *export)
{
	int		ret;
	pid_t	pid;

	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		printf("minishell: execve: failed to create a new process.\n");
		exit(1);
	}
	if (pid == 0)
		child_process(cmd, env, export);
	else
		parent_process(cmd, pid);
}
