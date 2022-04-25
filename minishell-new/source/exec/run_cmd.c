/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:45:28 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/25 14:23:58 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

#define TMP_FILE	"/tmp/minihell_temporary_file"

static int	create_temporary_file(void)
{
	int	fd;

	fd = open(TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	// if (fd == -1)
	// 	error_message("redirect", strerror(errno), 1);
	return (fd);
}

void	interrupt_here_document(int signal)
{
	(void)signal;
	g_exit_code = 130;
	write(1, "\n", 1);
	exit(131);
}

int	heredoc(char *path)
{
	int	tmp_fd;
	int save_fd_out;
	pid_t pid;
	int	status;
	char *str;
	int size;

	tmp_fd = create_temporary_file();
	if (tmp_fd == -1)
		return (-1);
	save_fd_out = dup(0);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, interrupt_here_document);
		while (1)
		{
			str = readline("> ");
			if (!str)
			{
				close(tmp_fd);
				exit(0);
			}
			size = ft_strlen(str);
			if (ft_strncmp(&path[2], str, size) == 0)
			{
				close(tmp_fd);
				free(str);
				break ;
			}
			else
			{
				str = ft_strjoin(str, "\n");
				write(tmp_fd, str, (ft_strlen(str) + 1));
			}
		}
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		tmp_fd = open(TMP_FILE, O_WRONLY | O_TRUNC, 0600);
		close(tmp_fd);
		g_exit_code = 130;
	}
	tmp_fd = open(TMP_FILE, O_RDONLY);
	unlink(TMP_FILE);
	dup2(tmp_fd, 0);
	close(tmp_fd);
	return (tmp_fd);
	// dup2(save_fd_out, STDOUT_FILENO);
	// close(save_fd_out);
	
	// int		fd[2];
	// int		heredoc;
	// int		size;
	// char	*str;
	// int		fd_pipe[2];
	// pid_t	pid;

	// pipe(fd_pipe);
	// fd[0] = fd_pipe[0];
	// fd[1] = fd_pipe[1];
	// pid = fork();
	// if (pid < 0)
	// {
	// 	printf("minishell: heredoc: failed to create a new process.\n");
	// 	exit(1);
	// }
	// if (pid == 0)
	// {
	// 	fd[1] = open("/tmp/.heredoc.txt", O_RDWR | O_CREAT | O_TRUNC, 0600);
	// 	signal(SIGINT, interrupt_here_document);
	// 	while (1)
	// 	{
	// 		str = readline(">");
	// 		if (str == NULL)
	// 			size = 0;
	// 		else
	// 			size = ft_strlen(str);
	// 		if (ft_strncmp(&path[2], str, size) == 0)
	// 		{
	// 			dup2(fd[1], STDOUT_FILENO);
	// 			close(fd[1]);
	// 			close(fd[0]);
	// 			// unlink("/tmp/.heredoc.txt");
	// 			return (fd[1]);
	// 		}
	// 		str = ft_strjoin(str, "\n");
	// 		write(heredoc, str, (ft_strlen(str) + 1));
	// 		close(heredoc);
	// 		heredoc = open("/tmp/.heredoc.txt", O_RDWR | O_APPEND, S_IRWXU);
	// 	}
	// }
	// else if (pid > 0)
	// {
	// 	dup2(fd[0], STDIN_FILENO);
	// 	waitpid(pid, &pid, 0);
	// 	close(fd[1]);
	// 	close(fd[0]);
	// }
	// return (1);
	

	// signal(SIGINT, interrupt_here_document);
	// // name_file = tmpfile(); -> ou tester avec flag O_TMPFILE sur linux ?? #define _GNU_SOURCE 1
	// heredoc = open("/tmp/.heredoc.txt", O_RDWR | O_CREAT | O_TRUNC, 0600);
	// while (1)
	// {
	// 	str = readline(">");
	// 	if (str == NULL)
	// 		size = 0;
	// 	else
	// 		size = ft_strlen(str);
	// 	if (ft_strncmp(&path[2], str, size) == 0)
	// 	{
	// 		fd_stdin = dup(heredoc);
	// 		close(heredoc);
	// 		unlink("/tmp/.heredoc.txt");
	// 		return (fd_stdin);
	// 	}
	// 	str = ft_strjoin(str, "\n");
	// 	write(heredoc, str, (ft_strlen(str) + 1));
	// 	close(heredoc);
	// 	heredoc = open("/tmp/.heredoc.txt", O_RDWR | O_APPEND, S_IRWXU);
	// }
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
			// dup2(fd_stdin, 0);
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
		if (path[i])
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
		close(cmd.fd_stdout);
		close(cmd.fd_stdin);
		if ((ft_check_builtins(cmd.user_input[0])) == 1)
			ft_execute_external_cmd(cmd.user_input, env);
		else if (ft_check_builtins(cmd.user_input[0]) == 0)
			ft_execute_builtins(cmd, &env, &export);
		kill(pid, SIGQUIT);
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
		// printf("output\n");
	while (path[i])
	{
		if (path[i][0] == '>' && path[i][1] == '>')
		{
			// printf("open\n");
			ret = i;
			fd_stdout = open(&path[i][2], O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else if (path[i][0] == '>' && path[i][1] != '>')
		{
			// printf("open\n");
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
		is_chevrons(&cmd[i]);
	// printf("ischevrons\ninput == %i\noutput == %i\n", cmd[i].input, cmd[i].output);
		if (cmd[i].input == 1)
			cmd[i].fd_stdin = input(cmd[i].path, 0);
		else
			cmd[i].fd_stdin = dup(0);
		if (cmd[i].fd_stdin == -1)
			break ;
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
			// printf("cmd.output == %i\n", cmd[i].output);
			if (cmd[i].output == 1)
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
