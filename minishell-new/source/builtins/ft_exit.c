/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 19:02:02 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/26 21:47:27 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_is_arg_longlong(char *cmd)
{
	int			i;
	int			neg;
	long long	result;

	i = 0;
	neg = 0;
	result = 0;
	if (cmd[i] == '-')
		neg = 1;
	if (cmd[i] == '-' || cmd[i] == '+')
		i++;
	while (cmd[i] >= '0' && cmd[i] <= '9' && cmd[i] != '\0')
	{
		result = (cmd[i] - 48) + (result * 10);
		i++;
	}
	if (neg == 1)
		result = result * (-1);
	if (result < LONGLONG_MIN || result > LONGLONG_MAX)
		return (0);
	return (1);
}

int	ft_is_exit_arg_num(char *cmd)
{
	int		i;

	i = 0;
	if (cmd == NULL)
		return (0);
	if (ft_is_arg_longlong(cmd) == 0)
		return (0);
	if (cmd[0] == '-')
		i++;
	while (cmd[i] != '\0')
	{
		if (cmd[i] < '0' || cmd[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit_numeric(void)
{
	g_exit_code = 2;
	ft_putstr_fd("exit: numeric argument required\n", STDERROR);
	return (g_exit_code);
}

int	ft_exit(char **argv)
{
	g_exit_code = 1;
	ft_putstr_fd("exit\n", STDERROR);
	if (argv[1] != NULL && argv[2] != NULL)
	{
		if (ft_is_exit_arg_num(argv[1]) == 0)
			ft_exit_numeric();
		else
		{
			ft_putstr_fd("exit: too many arguments\n", STDERROR);
			return (1);
		}
	}
	else if (argv[1] != NULL && ft_is_exit_arg_num(argv[1]) == 0)
		ft_exit_numeric();
	else if (argv[1] != NULL)
		g_exit_code = ft_atoi(argv[1]);
	else
		g_exit_code = 0;
	exit(g_exit_code);
	return (g_exit_code);
}
