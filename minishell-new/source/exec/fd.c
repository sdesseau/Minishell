/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 18:50:32 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/27 19:25:51 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	child_heredoc(char *path, int tmp_fd)
{
	char	*str;

	signal(SIGINT, interrupt_here_document);
	while (1)
	{
		str = readline("> ");
		if (!str)
		{
			close(tmp_fd);
			exit(0);
		}
		if (ft_strncmp(&path[2], str, ft_strlen(str)) == 0)
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

int	heredoc(char *path)
{
	int		fd_stdin;
	int		tmp_fd;
	int		save_fd_out;
	pid_t	pid;
	int		status;

	fd_stdin = 0;
	tmp_fd = open("/tmp/.heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd == -1)
		return (-1);
	save_fd_out = dup(0);
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
	tmp_fd = open("/tmp/.heredoc", O_RDONLY);
	dup2(tmp_fd, fd_stdin);
	unlink("/tmp/.heredoc");
	// close(tmp_fd);
	return (fd_stdin);
}

int	get_last_heredoc(char **path, int i)
{
	int	ret2;
	int	ret;
	int	fd_stdin;

	ret2 = i;
	while (path[i])
	{
		if (path[i][0] == '<' && path[i][1] == '<')
			ret = i;
		i++;
	}
	fd_stdin = heredoc(path[ret]);
	i = ret2;
	return (fd_stdin);
}

int	input(char **path, int tmp_stdin)
{
	int	i;
	int	fd_stdin;
	int	ret;

	ret = -1;
	i = 0;
	fd_stdin = -1;
	while (path[i])
	{
		if (path[i][0] == '<' && path[i][1] != '<')
		{
			ret = i;
			if (fd_stdin != -1)
				close(fd_stdin);
			fd_stdin = open(&path[ret][1], O_RDONLY, 0644);
			if (fd_stdin == -1)
			{
				printf("%s :no such file or directoy\n", &path[ret][1]);
				return (-1);
			}
		}
		else if (path[i][0] == '<' && path[i][1] == '<')
			fd_stdin = get_last_heredoc(path, i);
		i++;
	}
	if (ret == -1)
		fd_stdin = dup(tmp_stdin);
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
