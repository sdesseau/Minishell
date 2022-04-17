/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:45:28 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/17 19:38:55 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// int     heredoc(char *path)
// {
//     int fd_stdin;

//     return (fd_stdin);
// }

int     input(char **path, int tmp_stdin)
{
    int i;
    int fd_stdin;
    int ret;

    ret = -1;
    i = 0;
    while (path[i])
    {
        if (path[i][0] == '<' && path[i][1] != '<')
            ret = i;
        // else if (path[i][0] == '<' && path[i][1] == '<')
        // {
		// 	fd_stdin = heredoc(path[i]);
        //     return (fd_stdin);
        // }
        i++;
    }
    if (ret != -1)
        fd_stdin = open(&path[ret][1], O_RDONLY, 0644);
    else
        fd_stdin = dup(tmp_stdin);
    // printf("ret >> %i\n", ret);
    return (fd_stdin);
}

int     nb_of_pipe(t_cmd *cmd)
{
    int i;
    int nb_pipe;

    i = 0;
    nb_pipe = 1;
    while (cmd[i].index != -1)
    {
        if (cmd[i].pipe == 1)
            nb_pipe++;
        i++;
    }
    // printf("nb of cmd pipe >> %i\n", nb_pipe);
    return (nb_pipe);
}

void	child_process(t_cmd cmd, t_env *env, int *pipe, t_export *export)
{
	int		ret;
	pid_t		pid;

	pid = fork();
	ret = 0;
    // printf("child process\n");
	if (pid < 0)
	{
		close(pipe[1]);
		printf("error fork\n");
		exit(1);
	}
	if (pid == 0)
	{
		signal(SIGQUIT, child_handler);
		// handle_exit_cmd(cmd);
		close(pipe[0]);
		dup2(pipe[1], 1);
        if ((ft_check_builtins(cmd.user_input[0])) == 0)
            ret = ft_execute_builtins(&cmd, &env, &export);
        return ;
        // else
		//     execute_external_cmd(cmd, env);
	}
	else
	{
		close(pipe[1]);
		dup2(pipe[0], 0);
        // printf("before wait\n");
		wait(&ret);
        // printf("after wait\n");
	}
}

int     output(char **path)
{
    int fd_stdout;

    if (path[0][0] == '>' && path[0][1] == '>')
		fd_stdout = open(&path[0][2], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd_stdout = open(&path[0][1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    return (fd_stdout);
}

void    run_commands(t_cmd *cmd, t_env **env, t_export **export)
{
    int tmp_stdin;
    int tmp_stdout;
    int fd_stdin;
    int fd_stdout;
    int fd_pipe[2];
    int nb_cmd;
    int i;

    i = 0;
    nb_cmd = nb_of_pipe(cmd);
    tmp_stdin = dup(0);
    tmp_stdout = dup(1);
    if (cmd[i].nb_chevrons > 0)
        fd_stdin = input(cmd[i].path, tmp_stdin);
    else
        fd_stdin = dup(tmp_stdin);
    while (i < nb_cmd)
    {
        // printf("loop runcmd\n");
        dup2(fd_stdin, STDIN_FILENO);
        close(fd_stdin);
        if (i < nb_cmd - 1)
		{
			pipe(fd_pipe);
			fd_pipe[0] = fd_stdin;
            fd_pipe[1] = fd_stdout;
		}
		else
		{
			if (cmd[i].nb_chevrons > 0 && cmd[i].path[0][0] == '>')
				fd_stdout = output(cmd[i].path);
			else
				fd_stdout = dup(tmp_stdout);
		}
        child_process(cmd[i], (*env), fd_pipe, (*export));
		i++;
    }
    dup2(fd_stdin, STDIN_FILENO);
    dup2(fd_stdout, STDOUT_FILENO);
    close(tmp_stdin);
    close(tmp_stdout);
}
