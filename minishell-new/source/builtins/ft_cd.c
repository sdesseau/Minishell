/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 16:28:06 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/17 18:49:13 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_cd_nb_args(char **argv)
{
	int		i;

	i = 1;
	while (argv[i] != NULL)
	{
		if (i >= 2)
			return (1);
		i++;
	}
	return (0);
}

int ft_cd_error(int error_id)
{
    g_exit_code = 1;
    if (error_id == 1)
       ft_putstr_fd("cd: too many arguments\n", STDERROR);
    else if (error_id == 2)
        ft_putstr_fd("cd: HOME not set\n", STDERROR);
    else if (error_id == 3)
        ft_putstr_fd("cd: OLDPWD not set\n", STDERROR);
    else if (error_id == 4)
		ft_putstr_fd("cd: error retrieving current directory: getcwd: \
				cannot access parent directories: No such file or directory\n",
			STDERROR);
    else if (error_id == 5)
		ft_putstr_fd("cd: No such file or directory\n", STDERROR);
    return (1);
}

int	ft_update_old_pwd(char *old_pwd, t_env **env)
{
	char	new_pwd[4096];
	char	*ret;

	if (find_env_value("OLDPWD", (*env)) != NULL)
		update_env("OLDPWD", old_pwd, env);
	else
		add_env_var("OLDPWD", old_pwd, env);
	ret = getcwd(new_pwd, 4096);
	if (ret == NULL)
		return (1);
	update_env("PWD", new_pwd, env);
	return (0);
}

int	ft_cd(char **argv, t_env **env)
{
    char	old_pwd[4096];
	char	*home_value;
	char	*old_pwd_value;
    int		ret;

    ret = 0;
	getcwd(old_pwd, 4096);
    if (ft_cd_nb_args(argv) == 1)
		return (ft_cd_error(1));
    if (argv[1] == 0 && ft_strncmp(argv[0], "cd", 3) == 0)
	{
        home_value = find_env_value("HOME", (*env));
		ret = chdir(home_value);
		if (home_value == NULL || ret == -1)
			return (ft_cd_error(2));
    }
    else if (argv[1][0] == '-')
	{
		old_pwd_value = find_env_value("OLDPWD", (*env));
		printf("%s\n", old_pwd_value);
		ret = chdir(old_pwd_value);
		if (old_pwd_value == NULL || ret == -1)
			return (ft_cd_error(3));
	}
    else
	{
		if (chdir(argv[1]) == 0)
		{
			if (ft_update_old_pwd(old_pwd, env) == 1)
				return (ft_cd_error(4));
		}
		else
			return (ft_cd_error(5));
	}
	return (0);
}
