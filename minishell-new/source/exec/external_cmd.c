/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 16:19:27 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/20 21:22:31 by sdesseau         ###   ########.fr       */
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

int	ft_total_paths(char *path_value)
{
	int		total_paths;
	int		i;

	total_paths = 0;
	i = 0;
	while (path_value[i] != '\0')
	{
		if (path_value[i] == ':')
			total_paths++;
		i++;
	}
	return (total_paths + 1);
}

int	ft_is_cmd_in_folder(char *cmd_full_path)
{
	int		ret;

	if (!cmd_full_path)
		return (0);
	ret = open(cmd_full_path, O_RDONLY);
	if (ret != -1)
	{
		close(ret);
		return (1);
	}
	return (0);
}

char	*ft_concat_path(char *path_1, char *path_2)
{
	char	*concat_path;
	int		size_path_1;
	int		size_path_2;
	int		size_concat_path;

	if (!path_1 || !path_2)
		return (NULL);
	concat_path = NULL;
	size_path_1 = ft_strlen(path_1);
	size_path_2 = ft_strlen(path_2);
	size_concat_path = size_path_1 + size_path_2 + 2;
	concat_path = (char *)malloc(sizeof(char) * size_concat_path + 1);
	if (!concat_path)
		return (NULL);
	ft_strlcpy(concat_path, path_1, size_path_1 + 1);
	ft_strlcat(concat_path, "/", size_concat_path);
	ft_strlcat(concat_path, path_2, size_concat_path + 1);
	return (concat_path);
}

char	*ft_find_exe_path(char *exe_name, char *path_value)
{
	char	**paths_tab;
	char	*path_to_find;
	int		total_path;
	int		i;

	if (!exe_name || !path_value)
		return (NULL);
	paths_tab = NULL;
	path_to_find = NULL;
	if (ft_strchr(exe_name, '/'))
		return (exe_name);
	else
	{
		paths_tab = ft_split(path_value, ':');
		if (!paths_tab)
			return (NULL);
		total_path = ft_total_paths(path_value);
		i = 0;
		while (i < total_path)
		{
			path_to_find = ft_concat_path(paths_tab[i], exe_name);
			if (ft_is_cmd_in_folder(path_to_find) == 1)
				return (path_to_find);
			if (path_to_find != NULL)
				free(path_to_find);
			i++;
		}
	}
	return (NULL);
}

int	execute_external_cmd(t_cmd *cmd, t_env *env, pid_t pid)
{
	int		result;
	char	*path_value;
	char	*cmd_path;
	char	**envp;

	envp = ft_conv_env_to_tab(env);
	if (cmd->user_input[0] == NULL)
		return (0);
	path_value = getenv("PATH");
	cmd_path = ft_find_exe_path(cmd->user_input[0], path_value);
	if (cmd_path == NULL || (cmd->user_input[0][0] == '.'
		&& cmd->user_input[0][1] == 0) || (cmd->user_input[0][0] == '.'
		&& cmd->user_input[0][1] == '.' && cmd->user_input[0][2] == 0))
	{
		printf("%s: command not found\n", cmd->user_input[0]);
		exit(127);
	}
	result = execve(cmd_path, cmd->user_input, envp);
	if (result == -1)
	{
		printf("%s: permission denied\n", cmd->user_input[0]);
		exit(126);
	}
	// wait(&pid);
	return (0);
}