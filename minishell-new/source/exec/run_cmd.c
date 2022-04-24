/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:45:28 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/24 14:18:53 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	heredoc(char *path)
{
	int		fd_stdin;
	int		heredoc;
	int		size;
	char	*str;

	heredoc = open("/tmp/.heredoc.txt", O_RDWR | O_CREAT | O_TRUNC, 0600);
	while (1)
	{
		str = readline(">");
		if (str == NULL)
			size = 0;
		else
			size = ft_strlen(str);
		if (ft_strncmp(&path[2], str, size) == 0)
		{
			fd_stdin = dup(heredoc);
			close(heredoc);
			unlink("/tmp/.heredoc.txt");
			return (fd_stdin);
		}
		str = ft_strjoin(str, "\n");
		write(heredoc, str, (ft_strlen(str) + 1));
		close(heredoc);
		heredoc = open("/tmp/.heredoc.txt", O_RDWR | O_APPEND, S_IRWXU);
	}
}

int	input(char **path, int tmp_stdin)
{
	int	i;
	int	fd_stdin;
	int	ret;
	int ret2;

	ret = -1;
	i = 0;
	while (path[i])
	{
		if (path[i][0] == '<' && path[i][1] != '<')
		{
			close(fd_stdin);
			ret = i;
			fd_stdin = open(&path[ret][1], O_RDONLY, 0644);
			if (fd_stdin == -1)
			{
				printf("%s :no such file or directoy\n", &path[ret][1]);
				return (-1);
			}
			dup2(fd_stdin, 0);
		}
		else if (path[i][0] == '<' && path[i][1] == '<')
		{
			ret2 = i;
			while (path[i])
			{
				if (path[i][0] == '<' && path[i][1] == '<')
					ret = i;
				i++;
			}
			fd_stdin = heredoc(path[ret]);
		}
		i++;
	}
	if (ret == -1)
		fd_stdin = dup(tmp_stdin);
	return (fd_stdin);
}

void	child_process(t_cmd cmd, t_env *env, t_export *export)
{
	int		ret;
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		printf("minishell: execve: failed to create a new process.\n");
		exit(1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		// signal(SIGQUIT, child_handler);
		dup2(cmd.fd_stdout, STDOUT_FILENO);
		if ((ft_check_builtins(cmd.user_input[0])) == 1)
			ft_execute_external_cmd(cmd.user_input, env);
		else if (ft_check_builtins(cmd.user_input[0]) == 0)
			ft_execute_builtins(cmd, &env, &export);
		close(cmd.fd_stdout);
		close(cmd.fd_stdin);
		// kill(pid, SIGQUIT);
	}
	else
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
}

int	output(char **path, int tmp_stdout)
{
	int	fd_stdout;
	int	i;
	int	ret;

	ret = -1;
	i = 0;
	while (path[i])
	{
		if (path[i][0] == '>' && path[i][1] == '>')
		{
			ret = i;
			fd_stdout = open(&path[i][2], O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else if (path[i][0] == '>' && path[i][1] != '>')
		{
			ret = i;
			fd_stdout = open(&path[i][1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		}
		i++;
	}
	if (ret == -1)
		fd_stdout = dup(tmp_stdout);
	return (fd_stdout);
}

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
		child_process(cmd, (*env), (*export));
}

void	is_chevrons(t_cmd cmd)
{
	int	i;
	
	i = 0;
	cmd.input = 0;
	cmd.output = 0;
	if (cmd.nb_chevrons > 0)
	{
		while (cmd.path[i])
		{
			if (cmd.path[i][0] == '>')
				cmd.output = 1;
			else if (cmd.path[i][0] == '<')
				cmd.input = 1;
			i++;
		}
	}
}

void	run_commands(t_cmd *cmd, t_env **env, t_export **export)
{
	int	tmp_stdin;
	int	tmp_stdout;
	int	nb_cmd;
	int	i;

	i = 0;
	nb_cmd = nb_of_pipe(cmd);
	tmp_stdin = dup(0);
	tmp_stdout = dup(1);
	if (nb_cmd == 1)
	{
		if (cmd[i].nb_chevrons > 0)
			cmd[i].fd_stdin = input(cmd[i].path, tmp_stdin);
		else
			cmd[i].fd_stdin = dup(tmp_stdin);
		if (cmd[i].fd_stdin == -1)
			return ;
		exec_single_cmd(cmd[i], env, export, tmp_stdout);
		i++;
	}
	while (i < nb_cmd)
	{
		is_chevrons(cmd[i]);
		if (cmd[i].nb_chevrons > 0)
			cmd[i].fd_stdin = input(cmd[i].path, 0);
		else
			cmd[i].fd_stdin = dup(0);
		if (i < nb_cmd - 1)
		{
			pipe(cmd[i].fd_pipe);
			if (cmd[i].input == 1)
				cmd[i].fd_pipe[0] = input(cmd[i].path, cmd[i].fd_stdin);
			if (cmd[i].output == 1)
				cmd[i].fd_pipe[1] = output(cmd[i].path, cmd[i].fd_stdout);
			cmd[i].fd_stdin = cmd[i].fd_pipe[0];
			cmd[i].fd_stdout = cmd[i].fd_pipe[1];
		}
		else
		{
			if (cmd[i].nb_chevrons > 0)
				cmd[i].fd_stdout = output(cmd[i].path, 1);
			else
				cmd[i].fd_stdout = dup(1);
		}
		child_process(cmd[i], (*env), (*export));
		i++;
	}
	dup2(tmp_stdin, STDIN_FILENO);
	dup2(tmp_stdout, STDOUT_FILENO);
	close(tmp_stdin);
	close(tmp_stdout);
}
