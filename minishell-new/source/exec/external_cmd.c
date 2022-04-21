/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:07:13 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/21 21:01:22 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_size_env(t_env *lst)
{
	int		i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

char	*ft_convert_env(char *name, char *value)
{
	char	*final_str;
	int		name_size;
	int		value_size;
	int		size_final_str;

	if (!name || !value)
		return (NULL);
	final_str = NULL;
	name_size = ft_strlen(name);
	value_size = ft_strlen(value);
	size_final_str = name_size + value_size + 1;
	final_str = (char *)malloc(sizeof(char) * size_final_str + 1);
	if (!final_str)
		return (NULL);
	ft_strlcpy(final_str, name, name_size + 1);
	ft_strlcat(final_str, "=", size_final_str + 1);
	ft_strlcat(final_str, value, size_final_str + 1);
	return (final_str);
}

char	**ft_conv_env_to_tab(t_env *env)
{
	t_env	*temp;
	char	**tab;
	int		len;
	int		i;

	i = 0;
	len = ft_size_env(env);
	temp = env;
	tab = (char **)calloc(sizeof(char *), (len + 1));
	if (tab == NULL)
		return (NULL);
	while (temp != NULL)
	{
		tab[i] = ft_convert_env(temp->name, temp->value);
		temp = temp->next;
		i++;
	}
	return (tab);
}

char	**ft_get_path(t_env **env)
{
	char	**path;
	char	*temp;

	if (!(temp = find_env_value("PATH", (*env))))
		return (NULL);
	if (!(path = ft_split(temp, ':')))
	{
		return (NULL);
	}
	return (path);
}

int			ft_check_permission(char **cmd, char *ext_cmd, struct stat statbuf, t_env *env)
{
	char	**envp;
	
	envp = ft_conv_env_to_tab(env);
	if (statbuf.st_mode & S_IFREG)
	{
		if (statbuf.st_mode & S_IXUSR)
			execve(ext_cmd, cmd, envp);
		else
		{
			ft_putstr_fd("minishell: execve: permission denied: ", 2);
			ft_putstr_fd(ext_cmd, 2);
			ft_putchar_fd('\n', 2);
			g_exit_code = 1;
		}
		return (1);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[1], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		g_exit_code = 1;
	}
	return (1);
}

// int	check(char **cmd, t_env *env)
// {
// 	struct stat	f;
// 	int			flag;

// 	if (lstat(cmd[0], &f) != -1)
// 	{
// 		flag = f.st_mode & S_IFMT;
// 		if (flag == S_IFDIR && (cmd[0][ft_strlen(cmd[0]) - 1] == '/' || ft_strncmp(cmd[0], "./", 2)))
// 			return (ft_error(cmd[0], ": Is a directory\n", 0, 126));
// 		else if (flag != S_IXUSR && flag != S_IRUSR && flag != S_IFDIR && flag != S_IFLNK)
// 			return (ft_error(cmd[0], ": Permission denied\n", 0, 126));
// 		else if ((ft_strncmp(cmd[0], "./", 2) || ft_strncmp(cmd[0], "/", 1)) && (f.st_mode & S_IXUSR) && (f.st_mode & S_IRUSR))
// 			return (ft_check_permission(cmd, ft_strdup(cmd[0]), f, env));
// 	}
// 	else if (ft_strncmp(cmd[0], "./", 2) || cmd[0][0] == '/')
// 	{
// 		return (ft_error(cmd[0], ": No such file or directory\n", 0, 127));
// 	}
// 	return (ft_error(cmd[0], ": command not found\n", 0, 127));
// }

int			ft_execute_external_cmd(char **cmd, t_env *env)
{
	struct stat statbuf;
	char		**path;
	char		*ext_cmd;
	size_t		i;
	
	path = ft_get_path(&env);
	if (!path)
		return (-1);
	if (lstat(cmd[0], &statbuf) != -1)
	{
		if ((ft_strncmp(cmd[0], "./", 2) || ft_strncmp(cmd[0], "/", 1)) && (statbuf.st_mode & S_IXUSR) && (statbuf.st_mode & S_IRUSR))
		{
			return(ft_check_permission(cmd, ft_strdup(cmd[0]), statbuf, env));
		}
	}
	i = 0;
	while (path[i] && cmd[0])
	{
		ext_cmd = ft_strjoin(path[i], "/");
		ext_cmd = ft_strjoin(ext_cmd, cmd[0]);
		if (!lstat(ext_cmd, &statbuf))
		{
			return (ft_check_permission(cmd, ext_cmd, statbuf, env));
		}
		i++;
		if ((cmd[0][0] == '.' && cmd[0][1] == 0) || (cmd[0][0] == '.' && cmd[0][1] == '.' && cmd[0][2] == 0) || !path[i])
		{
			printf("minishell: %s: command not found\n", cmd[0]);
			exit(127);
		}
	}
	return (0);
}
