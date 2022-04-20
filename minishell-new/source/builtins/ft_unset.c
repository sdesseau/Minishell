/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 12:52:10 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/20 23:35:07 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	unset_in_env(char *argv, t_env **env)
{
	t_env	*tmp;
	t_env	*i_env;

	tmp = (*env);
	if (ft_strncmp(argv, (*env)->name, ft_strlen(argv)) == 0)
	{
		if (tmp->next)
			(*env) = tmp->next;
		free_env_var(tmp);
		return ;
	}
	i_env = (*env);
	while (i_env && i_env->next)
	{
		if (ft_strncmp(argv, i_env->next->name, ft_strlen(argv)) == 0)
		{
			tmp = i_env->next->next;
			i_env ->next = tmp;
			break ;
		}
		i_env = i_env->next;
	}
}

void	unset_in_export(char *argv, t_export **export)
{
	t_export	*tmp;
	t_export	*i_export;

	tmp = (*export);
	if (ft_strncmp(argv, (*export)->name, ft_strlen(argv)) == 0)
	{
		if (tmp->next != NULL)
			(*export) = tmp->next;
		else
			(*export) = (*export);
		free_export_var(tmp);
		return ;
	}
	i_export = (*export);
	while (i_export && i_export->next)
	{
		if (ft_strncmp(argv, i_export->next->name, ft_strlen(argv)) == 0)
		{
			tmp = i_export->next->next;
			i_export->next = tmp;
			break ;
		}
		i_export = i_export->next;
	}
}

int	is_unset_arg_valid(char *arg)
{
	int		i;

	i = 0;
	while (arg[i] != '\0')
	{
		if ((arg[i] >= 65 && arg[i] <= 90) || (arg[i] >= 97 && arg[i] <= 122)
			|| arg[i] == 95)
			i++;
		else
			return (0);
	}
	return (1);
}

int	unset_command(char **argv, t_env **env, t_export **export)
{
	int		i;

	i = 1;
	if (!argv[1])
		return (unset_error(1));
	while (argv[i])
	{
		if ((is_unset_arg_valid(argv[i])) == 1)
		{
			unset_in_env(argv[i], env);
			unset_in_export(argv[i], export);
		}
		else
			return (unset_error(2));
		i++;
	}
	return (0);
}

int	unset_error(int error_id)
{
	g_exit_code = 1;
	if (error_id == 1)
		ft_putstr_fd("unset: not enough arguments\n", 2);
	if (error_id == 2)
		ft_putstr_fd("unset: not a valid identifier\n", 2);
	return (1);
}
