/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 17:14:02 by nachin            #+#    #+#             */
/*   Updated: 2022/04/16 14:54:12 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

unsigned char	g_exit_code;

int	is_internal_cmd(char **argv)
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

int	main(int argc, char **argv, char **envp)
{
	t_env		*env;
	t_export	*export;

	env = NULL;
	export = NULL;
	if (argc != 1)
	{
		printf("Error: Minishell don't take arguments\n");
		return (1);
	}
	(void)argv;
	recup_env(envp, &env);
	recup_export(envp, &export);
	assign_signals_handler();
	shell_loop(env, export);
	return (g_exit_code);
}
