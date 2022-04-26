/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 23:45:16 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/21 22:24:38 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_home_path(char *path, t_env **env)
{
	char		*tmp;
	char		*tmpp;

	if (!ft_strncmp(path, "~/", 2))
	{
		tmp = find_env_value("HOME", (*env));
		if (tmp)
		{
			tmpp = ft_substr(path, 1, ft_strlen(path));
			path = ft_strjoin(tmp, tmpp);
			return (path);
		}
	}
	return (path);
}

int	ft_update_pwd(char *path, int home, t_env **env)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!chdir(path))
	{
		if (pwd)
			update_env("OLDPWD", pwd, env);
		pwd = getcwd(NULL, 0);
		if (pwd)
			update_env("PWD", pwd, env);
		return (1);
	}
	free(pwd);
	return (0);
}

int	ft_set_directory(char *path, int home, t_env **env)
{
	struct stat	st;

	if (ft_update_pwd(path, home, env))
		return (1);
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	g_exit_code = 1;
	if (stat(path, &st) == -1)
	{
		ft_putstr_fd(": No such file or directory", 2);
		g_exit_code = 127;
	}
	else if (!(st.st_mode & S_IXUSR))
		ft_putstr_fd(": Permission denied", 2);
	else
		ft_putstr_fd(": Not a directory", 2);
	ft_putchar_fd('\n', 2);
	return (1);
}

int	ft_path(char **argv, t_env **env)
{
	char	*tmp;

	if (ft_strncmp(argv[1], "-", 1) == 0)
	{
		tmp = find_env_value("OLDPWD", (*env));
		if (tmp)
			ft_set_directory(tmp, 0, env);
		tmp = find_env_value("PWD", (*env));
		if (tmp)
		{
			ft_putstr_fd(tmp, 1);
			ft_putchar_fd('\n', 1);
		}
		return (1);
	}
	return (ft_set_directory(argv[1], 0, env));
}

int	ft_cd(char **argv, t_env **env)
{
	char	*home;

	g_exit_code = 0;
	home = NULL;
	if (argv && argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (!argv[1] || ft_strncmp(argv[1], "~", 1) == 0
		|| ft_strncmp(argv[1], "--", 2) == 0)
	{
		home = find_env_value("HOME", (*env));
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
		return (ft_set_directory(home, 1, env));
	}
	argv[1] = ft_home_path(argv[1], env);
	return (ft_path(argv, env));
}
