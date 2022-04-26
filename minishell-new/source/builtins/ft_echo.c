/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 15:08:28 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/26 20:33:58 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_is_flag_n(char *user_input)
{
	int		i;

	i = 0;
	if (user_input)
	{
		if (user_input[i] != '-')
			return (1);
		i++;
		if (user_input[i] != 'n')
			return (1);
		while (user_input[i] != '\0')
		{
			if (user_input[i] != 'n')
				return (1);
			i++;
		}
	}
	return (0);
}

void	ft_print_echo(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && cmd[i] != '\0')
	{
		ft_putchar_fd(cmd[i], 1);
		i++;
	}
}

int	ft_no_arg(char **argv)
{
	if (!argv[1])
	{
		ft_putchar_fd('\n', 1);
		g_exit_code = 0;
		return (0);
	}
	return (0);
}

int	ft_echo(char **argv)
{
	int	is_n;
	int	i;

	i = 1;
	is_n = ft_is_flag_n(argv[i]);
	ft_no_arg(argv);
	while (argv[i] != NULL)
	{
		if (ft_is_flag_n(argv[i]) == 0)
			i++;
		else
			break ;
	}
	while (argv[i] != NULL)
	{
		ft_print_echo(argv[i]);
		i++;
		if (argv[i] != NULL)
			ft_putchar_fd(' ', 1);
	}
	if (is_n == 1)
		ft_putchar_fd('\n', 1);
	return (0);
}
