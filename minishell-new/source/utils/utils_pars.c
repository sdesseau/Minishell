/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:08:12 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/29 16:31:36 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	init_cmd(t_cmd *data)
{
	data->nb_chevrons = 0;
	data->pipe = 0;
	return (*data);
}

t_pars	*put_lock(t_pars *pars)
{
	int	i;

	i = 0;
	while (pars[i].i != -1)
	{
		if (pars[i].s_q == 1 || pars[i].d_q == 1)
			pars[i].lock = 1;
		else
			pars[i].lock = 0;
		i++;
	}
	return (pars);
}

t_cmd	*empty_line(t_cmd *data, t_env **env, t_export **export)
{
	printf("exit\n");
	deallocate_env_export(env, export);
	exit(131);
	return (data);
}

int	pass_spaces(t_pars *pars, int i)
{
	while (pars[i].i != -1 && pars[i].val == ' '
		&& pars[i].lock == 0)
	{
		i++;
		if (pars[i].null == 1)
			break ;
	}
	return (i);
}

int	is_first_pipe(t_pars *pars)
{
	int		i;

	i = 0;
	i = pass_spaces(pars, i);
	if (pars[i].val == '|' && pars[i].lock == 0)
	{
		syntax_error('|');
		return (-1);
	}
	return (0);
}
