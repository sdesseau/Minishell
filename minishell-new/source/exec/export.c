/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 15:24:19 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/17 12:27:50 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	put_in_export(t_export **new, char *name, char *value)
{
	(*new)->name = ft_strdup(name);
	(*new)->value = ft_strdup(value);
	(*new)->next = NULL;
}

void	*free_export_var(t_export *export)
{
	free(export->name);
	free(export->value);
	free(export);
	return (0);
}

void	link_new_export_var(t_export **new, char*name, t_export **export)
{
	while ((*export) != NULL)
	{
		if ((*export)->next && ft_strncmp((*export)->next->name, name, ft_strlen(name)) == 0)
		{
			(*new)->next = (*export)->next->next;
			free_export_var((*export)->next);
			(*export)->next = (*new);
			break ;
		}
		(*export) = (*export)->next;
	}
}

int	update_export(char *name, char *value, t_export **export)
{
	t_export	*tmp;
	t_export	*new;

	tmp = (*export);
	new = malloc(sizeof(t_export));
	put_in_export(&new, name, value);
	new->is_equal = 1;
	if ((*export) && ft_strncmp((*export)->name, name, ft_strlen(name)) == 0)
	{
		new->next = (*export)->next;
		free_export_var((*export));
		(*export) = new;
		return (0);
	}
	link_new_export_var(&new, name, export);
	if ((*export) == NULL)
		free_export_var(new);
	(*export) = tmp;
	return (0);
}

int	export_var_already_exist(char *name, t_export *export)
{
	t_export	*tmp;

	tmp = export;
	if (!name || !tmp)
		return (0);
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name) + 1) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	add_export_var(char *name, char *val, char *line, t_export **export)
{
	t_export	*tmp;
	t_export	*new;

	tmp = (*export);
	if (export_var_already_exist(name, (*export)) == 1)
	{
		if (ft_strrchr(line, '=') == NULL)
			return (0);
		update_export(name, val, export);
		return (0);
	}
	new = malloc(sizeof(t_export));
	put_in_export(&new, name, val);
	if (ft_strrchr(line, '=') == NULL)
		new->is_equal = 0;
	else
		new->is_equal = 1;
	if (!(*export))
	{
		(*export) = new;
		return (0);
	}
	else
	{
		while ((*export)->next)
			(*export) = (*export)->next;
		(*export)->next = new;
		(*export) = tmp;
	}
	return (0);
}

void	recup_export(char **envp, t_export **export)
{
	int     i;
    int     name_len;
    char    *name;
    char    *val;

	i = 0;
	while (envp[i])
	{
        name_len = get_length_name(envp[i]);
		name = ft_substr(envp[i], 0, name_len);
		val = ft_substr(envp[i], name_len + 1, ft_strlen(&envp[i][name_len]));
		add_export_var(name, val, envp[i], export);
		free(name);
		free(val);
		i++;
	}
}

int	ft_lenlist(t_export *lst)
{
	int		i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

char	**convert_list_to_tab(t_export *export)
{
	t_export	*tmp;
	char		**tab;
	int			i;
	int			len;

	i = 0;
	len = ft_lenlist(export);
	tmp = export;
	tab = (char **)malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (NULL);
	tab[0] = NULL;
	while (tmp)
	{
		tab[i] = ft_strdup(tmp->name);
		tmp = tmp->next;
		i++;
		tab[i] = NULL;
	}
	return (tab);
}

char	**sort_export(t_export *export)
{
	char	**exp;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	exp = convert_list_to_tab(export);
	while (exp[i])
	{
		j = i + 1;
		while (exp[j])
		{
			if (ft_strncmp(exp[i], exp[j], ft_strlen(exp[i])) > 0)
			{
				tmp = exp[i];
				exp[i] = exp[j];
				exp[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (exp);
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

int	is_equal_sign(char *name)
{
	int		j;

	j = 0;
	while (name[j])
	{
		if (ft_strchr(name, '=') == 0)
			return (0);
		j++;
	}
	return (1);
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_new_name_valid(char *name)
{
	size_t	i;
	char	alpha;

	i = 0;
	alpha = 0;
	if (!name|| name[0] == '\0' || name[0] == '=')
		return (0);
	while (name[i] != '\0' && name[i] != '=')
	{
		if (ft_isalpha(name[i]))
			alpha = 1;
		else
		{
			if (ft_isdigit(name[i]) && alpha == 0)
				return (0);
			else if (ft_isdigit(name[i]) == 1 && name[i] != '_')
				return (0);
		}
		i++;
	}
	return (1);
}

char	*get_new_name(char *command, int *index)
{
	char	*new_name;
	int		i;

	new_name = NULL;
	i = *index;
	while (command[i] && command[i] != '=')
		i++;
	new_name = ft_substr(command, 0, i);
	if (is_new_name_valid(new_name) == 0)
	{
		ft_putstr_fd("export: not a valid identifier\n", 2);
		free(new_name);
		return (NULL);
	}
	i++;
	*index = i;
	return (new_name);
}