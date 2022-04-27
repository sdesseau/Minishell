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

void	deallocate_export_lst_elem(t_export *elem)
{
	if (!elem)
		return ;
	free(elem->name);
	free(elem->value);
	free(elem);
}

void	deallocate_env_lst_elem(t_env *elem)
{
	if (!elem)
		return ;
	free(elem->name);
	free(elem->value);
	free(elem);
}

void	deallocate_env_export(t_env **env, t_export **export)
{
	t_env 		*tmp_env;
	t_export 	*tmp_export;

	if (!(*env))
		return ;
	tmp_env = (*env);
	while ((*env)->next)
	{
		(*env) = (*env)->next;
		deallocate_env_lst_elem(tmp_env);
		tmp_env = (*env);
	}
	deallocate_env_lst_elem(tmp_env);

	tmp_export = (*export);
	while ((*export)->next)
	{
		(*export) = (*export)->next;
		deallocate_export_lst_elem(tmp_export);
		tmp_export = (*export);
	}
	deallocate_export_lst_elem(tmp_export);
	// t_env *tmp_env;
	// t_export *tmp_export;

	// if ((*env))
	// {
	// 	while ((*env)->next)
	// 	{
	// 		free((*env)->value);
	// 		free((*env)->name);
	// 		tmp_env = (*env)->next;
	// 		free((*env));
	// 		env = &tmp_env;
	// 	}
	// 	free((*env)->value);
	// 	free((*env)->name);
	// 	free((*env));
	// }
	// if ((*export))
	// {
	// 	while ((*export)->next)
	// 	{
	// 		free((*export)->value);
	// 		free((*export)->name);
	// 		tmp_export = (*export)->next;
	// 		free((*export));
	// 		export = &tmp_export;
	// 	}
	// 	free((*export)->value);
	// 	free((*export)->name);
	// }
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
