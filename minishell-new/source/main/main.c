/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 17:14:02 by nachin            #+#    #+#             */
/*   Updated: 2022/04/24 17:57:23 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

unsigned char	g_exit_code;

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
