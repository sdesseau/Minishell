/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 13:02:06 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/29 16:28:52 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	int		equal;
	char	*value;

	i = 0;
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

int	run_export_cmd(char **argv, t_env **env, t_export **export)
{
	int		j;
	char	*name;
	char	*value;
	int		i;

	i = 1;
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
		free(name);
		free(value);
		i++;
	}
	return (0);
}

int	export_command(char **argv, t_env **env, t_export **export)
{
	char	**export_sorted;

	if (!argv[1])
	{
		export_sorted = sort_export((*export));
		print_export(export_sorted, (*export));
		free_tab(export_sorted);
		return (0);
	}
	run_export_cmd(argv, env, export);
	return (0);
}
