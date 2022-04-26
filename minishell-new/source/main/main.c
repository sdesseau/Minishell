/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 17:14:02 by nachin            #+#    #+#             */
/*   Updated: 2022/04/26 15:51:55 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

unsigned char	g_exit_code;

void	deallocate_env_export(t_env **env, t_export **export)
{
	if ((*env))
	{
		while ((*env)->next)
		{
			free((*env)->value);
			free((*env)->name);
			(*env) = (*env)->next;
		}
	}
	if ((*export))
	{
		while ((*export)->next)
		{
			free((*export)->value);
			free((*export)->name);
			(*export) = (*export)->next;
		}
	}
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
	deallocate_env_export(&env, &export);
	return (g_exit_code);
}
