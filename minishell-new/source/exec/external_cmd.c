/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 16:19:27 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/20 22:34:27 by sdesseau         ###   ########.fr       */
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

int			ft_execute_external_cmd(char **cmd, t_env *env)
{
	struct stat statbuf;
	char		**path;
	char		*ext_cmd;
	char		*temp;
	size_t		i;

	if (!(path = ft_get_path(&env)))
		return (-1);
	i = -1;
	while (path[++i])
	{
		ext_cmd = ft_strjoin(path[i], "/");
		temp = ext_cmd;
		ext_cmd = ft_strjoin(ext_cmd, cmd[0]);
		if (!lstat(ext_cmd, &statbuf))
		{
			return (ft_check_permission(cmd, ext_cmd, statbuf, env));
		}
	}
	return (0);
}