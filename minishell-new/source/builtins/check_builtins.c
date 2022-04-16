/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 16:11:22 by mprigent          #+#    #+#             */
/*   Updated: 2022/04/16 19:38:09 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int ft_check_builtins(char **argv)
{
   	if (ft_strncmp(argv[0], "echo", 5) == 0
		|| ft_strncmp(argv[0], "cd", 3) == 0
		|| ft_strncmp(argv[0], "pwd", 4) == 0
		|| ft_strncmp(argv[0], "export", 7) == 0
		|| ft_strncmp(argv[0], "unset", 6) == 0
		|| ft_strncmp(argv[0], "env", 4) == 0
		|| ft_strncmp(argv[0], "exit", 5) == 0)
	{
		return (1);
	}
	return (0);
}

int	ft_execute_builtins(char **argv)
{
    if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (ft_echo(argv));
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(argv[0], "exit", 5) == 0)
		ft_exit(argv);
    return (0);
}
