/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 18:30:11 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/17 15:21:40 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp != NULL)
	{
		ft_putstr_fd(tmp->name, 1);
		ft_putchar_fd('=', 1);
		ft_putstr_fd(tmp->value, 1);
		ft_putchar_fd('\n', 1);
		tmp = tmp->next;
	}
}

int	env_command(char **argv, t_env *env)
{
	if (argv[1] != NULL)
	{
		ft_putstr_fd("env: No such file or directory\n", 2);
		return (127);
	}
	print_env(env);
	return (0);
}
