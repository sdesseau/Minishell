/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 13:02:06 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/26 16:10:16 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_new_name(char *command, int *index)
{
	char	*new_name;
	int		i;

	new_name = NULL;
	i = *index;
	while (command[i] && command[i] != '=')
		i++;
	new_name = ft_substr(command, 0, i);
	if (is_unset_arg_valid(new_name) == 0)
	{
		ft_putstr_fd("export: not a valid identifier\n", 2);
		free(new_name);
		return (NULL);
	}
	if (command[i])
		i++;
	*index = i;
	return (new_name);
}

char	*find_export_value(char *name, t_export *export)
{
	t_export	*tmp;
	int			len;

	tmp = export;
	len = ft_strlen(name);
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, len + 1) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	find_equal_value(char *name, t_export *export)
{
	t_export	*tmp;
	int			len;

	tmp = export;
	len = ft_strlen(name);
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, len + 1) == 0)
			return (tmp->is_equal);
		tmp = tmp->next;
	}
	return (0);
}

void	print_export(char **tab_export, t_export *export)
{
	int		i;
	int		j;
	int		equal;
	char	*value;

	i = 0;
	j = 0;
	equal = 0;
	while (tab_export[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(tab_export[i], 1);
		value = find_export_value(tab_export[i], export);
		equal = find_equal_value(tab_export[i], export);
		if (equal == 1)
		{
			ft_putchar_fd('=', 1);
			ft_putchar_fd('"', 1);
			ft_putstr_fd(value, 1);
			ft_putchar_fd('"', 1);
		}
		else
			ft_putstr_fd(value, 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
}

int	export_command(char **argv, t_env **env, t_export **export)
{
	int		i;
	int		j;
	char	*name;
	char	*value;
	char	**export_sorted;

	i = 1;
	if (!argv[1])
	{
		export_sorted = sort_export((*export));
		print_export(export_sorted, (*export));
		return (0);
	}
	while (argv[i])
	{
		j = 0;
		name = get_new_name(argv[i], &j);
		if (name == NULL)
			return (1);
		value = ft_strdup(argv[i] + j);
		add_export_var(name, value, argv[i], export);
		if (is_equal_sign(argv[i]) == 1)
			add_env_var(name, value, env);
		i++;
	}
	free(name);
	free(value);
	return (0);
}
