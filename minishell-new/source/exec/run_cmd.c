/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:45:28 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/20 21:41:47 by sdesseau         ###   ########.fr       */
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

int     heredoc(char *path)
{
    int fd_stdin;
    int heredoc;
    int	size;
	char *str;

	heredoc = open(".heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	while (1)
	{
		str = readline(">");
		if (str == NULL)
			size = 0;
		else
			size = ft_strlen(str);
		if (ft_strncmp(&path[2], str, size) == 0)
            break ;
        str = ft_strjoin(str, "\n");
		write(heredoc, str, (ft_strlen(str) + 1));
		close(heredoc);
		heredoc = open(".heredoc.txt", O_RDWR | O_APPEND, S_IRWXU);
	}
    fd_stdin = dup(heredoc);
    close(heredoc);
    unlink(".heredoc.txt");
    return (fd_stdin);
}

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
        {
            close(fd_stdin);
            ret = i;
            fd_stdin = open(&path[ret][1], O_RDONLY, 0644);
            if (fd_stdin == -1)
            {
                printf("%s :no such file or directoy\n", &path[ret][1]);
                return (-1);
            }
        }
        else if (path[i][0] == '<' && path[i][1] == '<')
        {
            ret = i;
            while (path[i])
            {
                if (path[i][0] == '<' && path[i][1] == '<')
                    ret = i;
                i++;
            }
			fd_stdin = heredoc(path[ret]);
            return (fd_stdin);
        }
        i++;
    }
    if (ret == -1)
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
		printf("error fork\n");
		exit(1);
	}
	if (pid == 0)
	{
		signal(SIGQUIT, child_handler);
	    dup2(cmd.fd_stdout, STDOUT_FILENO);
	    execute_external_cmd(&cmd, env, pid);
	    close(cmd.fd_stdout);
		close(cmd.fd_stdin);
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
        // printf("g_exit_code > %i\n", g_exit_code);
	    if (WIFSIGNALED(pid))
	    {
	    	g_exit_code = WTERMSIG(pid);
	    	if (g_exit_code != 131)
	    		g_exit_code += 128;
	    }
	}
}

int     output(char **path, int tmp_stdout)
{
    int fd_stdout;
    int i;
    int ret;

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

void    exec_single_cmd(t_cmd cmd, t_env **env, t_export **export, int tmp_stdout)
{
    dup2(cmd.fd_stdin, STDIN_FILENO);
    if (cmd.nb_chevrons > 0)
		cmd.fd_stdout = output(cmd.path, tmp_stdout);
	else
		cmd.fd_stdout = dup(tmp_stdout);
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
    if (cmd[i].fd_stdin == -1)
        return ;
    if (nb_cmd == 1)
    {
        exec_single_cmd(cmd[i], env, export, tmp_stdout);
        i++;
    }
    while (i < nb_cmd)
    { 
    if (cmd[i].nb_chevrons > 0)
        cmd[i].fd_stdin = input(cmd[i].path, tmp_stdin);
    else
        cmd[i].fd_stdin = dup(tmp_stdin);
        if (i < nb_cmd - 1)
		{
			pipe(cmd[i].fd_pipe);
			cmd[i].fd_stdin = cmd[i].fd_pipe[0];
            cmd[i].fd_stdout = cmd[i].fd_pipe[1];
		}
		else
		{
			if (cmd[i].nb_chevrons > 0)
				cmd[i].fd_stdout = output(cmd[i].path, tmp_stdout);
			else
				cmd[i].fd_stdout = dup(tmp_stdout);
		}
        if ((ft_check_builtins(cmd[i].user_input[0])) == 0)
        {
	        dup2(cmd[i].fd_stdout, 1);
            g_exit_code = ft_execute_builtins(cmd[i], env, export);
	        close(cmd[i].fd_stdin);
	        close(cmd[i].fd_stdout);
        }
        else
            child_process(cmd[i], (*env), (*export));
		i++;
    }
	dup2(tmp_stdin, STDIN_FILENO);
	dup2(tmp_stdout, STDOUT_FILENO);
    close(tmp_stdin);
    close(tmp_stdout);
    // dup2(fd_stdin, STDIN_FILENO);
}
