/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:45:28 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/20 21:52:52 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void    handle_exit_command(t_cmd cmd)
{
    if (ft_strncmp(cmd.user_input[0], "exit", 4) == 0)
    {
    	kill(0, SIGPIPE);
    	ft_exit(cmd.user_input);
    }
}

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
    {
       if ((fd_stdin = open(&path[ret][1], O_RDONLY, 0644)) == -1)
        {
            printf("no such file or directoy\n");
            exit(1);
        }
    }
    else
        fd_stdin = dup(tmp_stdin);
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

void	child_process(t_cmd cmd, t_env *env, t_export *export)
{
	int		ret;
	pid_t		pid;

	pid = fork();
	if (pid < 0)
	{
		printf("execve: failed to create a new process.\n");
		exit(1);
	}
	if (pid == 0)
	{
		signal(SIGQUIT, child_handler);
		close(cmd.fd_stdin);
	    dup2(cmd.fd_stdout, 1);
	    close(cmd.fd_stdout);
        // if ((ft_check_builtins(cmd.user_input[0])) == 0)
        //     ft_execute_builtins(cmd, &env, &export);
        // else
		    ft_execute_external_cmd(cmd.user_input, env);
        kill(pid, SIGQUIT);
	}
	else
	{
		close(cmd.fd_stdout);
		dup2(cmd.fd_stdin, 0);
		close(cmd.fd_stdin);
        // printf("before wait\n");
		waitpid(pid, &pid, 0); // pid ou -1 !!!!!!!!!!!!!!
        // printf("after wait\n");
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
    int nb_cmd;
    int i;

    i = 0;
    nb_cmd = nb_of_pipe(cmd);
    tmp_stdin = dup(0);
    tmp_stdout = dup(1);
    if (cmd[i].nb_chevrons > 0)
        cmd[i].fd_stdin = input(cmd[i].path, tmp_stdin);
    else
        cmd[i].fd_stdin = dup(tmp_stdin);
    if (nb_cmd == 1)
    {
        dup2(cmd[i].fd_stdin, STDIN_FILENO);
        if (cmd[i].nb_chevrons > 0 && cmd[i].path[0][0] == '>')
			cmd[i].fd_stdout = output(cmd[i].path);
		else
			cmd[i].fd_stdout = dup(tmp_stdout);
		if ((ft_check_builtins(cmd[i].user_input[0])) == 0)
		{
			dup2(cmd[i].fd_stdout, 1);
			ft_execute_builtins(cmd[i], env, export);
			close(cmd[i].fd_stdout);
			close(cmd[i].fd_stdin);
		}
		else
			child_process(cmd[i], (*env), (*export));
        i++;
    }
    while (i < nb_cmd)
    {
        if (i < nb_cmd - 1)
		{
			pipe(cmd[i].fd_pipe);
			cmd[i].fd_stdin = cmd[i].fd_pipe[0];
            cmd[i].fd_stdout = cmd[i].fd_pipe[1];
            // cmd[i + 1].fd_stdin = cmd[i].fd_pipe[1];
		}
		else
		{
			if (cmd[i].nb_chevrons > 0 && cmd[i].path[0][0] == '>')
				cmd[i].fd_stdout = output(cmd[i].path);
			else
				cmd[i].fd_stdout = dup(tmp_stdout);
		}
        // dup2(cmd[i].fd_stdin, STDIN_FILENO);
        child_process(cmd[i], (*env), (*export));
		i++;
    }
	dup2(tmp_stdin, STDIN_FILENO);
	dup2(tmp_stdout, STDOUT_FILENO);
    close(tmp_stdin);
    close(tmp_stdout);
    // dup2(fd_stdin, STDIN_FILENO);
}
