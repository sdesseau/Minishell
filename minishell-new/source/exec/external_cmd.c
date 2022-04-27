/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 16:19:27 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/27 18:25:21 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_tab(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	if (tab)
		free(tab);
}

char	**ft_get_path(t_env **env)
{
	char	**path;
	char	*temp;

	temp = find_env_value("PATH", (*env));
	if (!temp)
		return (NULL);
	path = ft_split(temp, ':');
	if (!path)
		return (NULL);
	return (path);
}

int	ft_check_permission(char **cmd, char *ext_cmd,
							struct stat statbuf, t_env *env)
{
	char	**envp;

	envp = ft_conv_env_to_tab(env);
	if (statbuf.st_mode & S_IFREG)
	{
		if (statbuf.st_mode & S_IXUSR)
			execve(ext_cmd, cmd, envp);
		else
		{
			printf("minishell: execve: permission denied: %s\n", ext_cmd);
			exit(126);
		}
		return (1);
	}
	else
	{
		printf("minishell: %s: Is a directory\n", cmd[0]);
		exit(126);
	}
	return (1);
}

int	ft_check_errors(char **cmd, t_env *env, struct stat statbuf)
{
	int		flag;

	if (lstat(cmd[0], &statbuf) != -1)
	{
		flag = statbuf.st_mode & S_IFMT;
		if ((ft_strncmp(cmd[0], "./", 2) || ft_strncmp(cmd[0], "/", 1))
			&& (statbuf.st_mode & S_IXUSR) && (statbuf.st_mode & S_IRUSR))
		{
			return (ft_check_permission(cmd, ft_strdup(cmd[0]), statbuf, env));
		}
		else if ((flag == S_IFDIR && (cmd[0][ft_strlen(cmd[0]) - 1] == '/'
				|| ft_strncmp(cmd[0], "./", 2))))
		{
			printf("minishell: %s: Is a directory\n", cmd[0]);
			exit(126);
		}
		else if (flag != S_IXUSR && flag != S_IRUSR
			&& flag != S_IFDIR && flag != S_IFLNK)
		{
			printf("minishell: %s: Permission denied\n", cmd[0]);
			exit(126);
		}
	}
	return (0);
}

int	ft_run_ext_cmd(char **cmd, t_env *env, char **path, struct stat statbuf)
{
	char		*ext_cmd;
	size_t		i;

	i = 0;
	if (!path)
	{
		printf("minishell: %s: No such file or directory\n", cmd[0]);
		exit(127);
	}
	while (path[i] && cmd[0])
	{
		ext_cmd = ft_strjoin(path[i], "/");
		ext_cmd = ft_strjoin(ext_cmd, cmd[0]);
		if (!lstat(ext_cmd, &statbuf))
		{
			int	x = (ft_check_permission(cmd, ext_cmd, statbuf, env));
			free(ext_cmd);
			return (x);
		}
		i++;
		if ((cmd[0][0] == '.' && cmd[0][1] == 0)
			|| (cmd[0][0] == '.' && cmd[0][1] == '.'
			&& cmd[0][2] == 0) || !path[i])
		{
			printf("minishell: %s: Command not found\n", cmd[0]);
			exit(127);
		}
	}
	return (0);
}

int	ft_execute_external_cmd(char **cmd, t_env *env)
{
	struct stat	statbuf;
	char		**path;

	lstat(cmd[0], &statbuf);
	path = ft_get_path(&env);
	ft_check_errors(cmd, env, statbuf);
	ft_run_ext_cmd(cmd, env, path, statbuf);
	return (0);
}
