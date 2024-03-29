/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 18:50:32 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/29 16:24:11 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	child_heredoc(char *path, int tmp_fd)
{
	char	*str;
	char	*tmp;

	signal(SIGINT, interrupt_here_document);
	while (1)
	{
		str = readline("> ");
		if (!str && close(tmp_fd))
			exit(0);
		if (ft_strncmp(&path[2], str, ft_strlen(str)) == 0)
		{
			close(tmp_fd);
			free(str);
			break ;
		}
		else
		{
			tmp = str;
			str = ft_strjoin(tmp, "\n");
			free(tmp);
			write(tmp_fd, str, (ft_strlen(str) + 1));
			free(str);
		}
	}
	exit(0);
}

int	heredoc(char *path)
{
	int		fd_stdin;
	int		tmp_fd;
	pid_t	pid;
	int		status;

	tmp_fd = open("/tmp/.heredoc", O_WRONLY | O_CREAT | O_APPEND, 0600);
	if (tmp_fd == -1)
		return (-1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		child_heredoc(path, tmp_fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		tmp_fd = open("/tmp/.heredoc", O_WRONLY | O_TRUNC, 0600);
		close(tmp_fd);
		g_exit_code = 130;
	}
	tmp_fd = open("/tmp/.heredoc", O_RDONLY, 0644);
	fd_stdin = dup(tmp_fd);
	close(tmp_fd);
	return (fd_stdin);
}

int	input_suite(int ret, int ret2, int fd_stdin, int tmp_stdin)
{
	if (ret2 != -1)
		unlink("/tmp/.heredoc");
	if (ret == -1 && ret2 == -1)
		fd_stdin = dup(tmp_stdin);
	return (fd_stdin);
}

int	input(char **path, int tmp_stdin, int ret, int ret2)
{
	int		i;
	int		fd_stdin;

	i = -1;
	fd_stdin = -1;
	while (path[++i])
	{
		if (path[i][0] == '<' && path[i][1] != '<')
		{
			ret = i;
			if (fd_stdin != -1)
				close(fd_stdin);
			fd_stdin = open(&path[ret][1], O_RDONLY, 0644);
			if (fd_stdin == -1 && printf(ERROR1, &path[ret][1]))
				return (-1);
		}
		else if (path[i][0] == '<' && path[i][1] == '<')
		{
			ret2 = i;
			fd_stdin = heredoc(path[ret2]);
		}
	}
	fd_stdin = input_suite(ret, ret2, fd_stdin, tmp_stdin);
	return (fd_stdin);
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
			fd_stdout = open(&path[ret][2], O_WRONLY
					| O_CREAT | O_APPEND, 0644);
		}
		else if (path[i][0] == '>' && path[i][1] != '>')
		{
			ret = i;
			fd_stdout = open(&path[ret][1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		}
		i++;
	}
	if (ret == -1)
		fd_stdout = dup(tmp_stdout);
	return (fd_stdout);
}
