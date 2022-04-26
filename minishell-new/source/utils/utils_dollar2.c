/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dollar2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:08:10 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 16:28:41 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	assign_new_pars(char *val, t_pars *tmp, t_pars *pars, int j)
{
	int		l;

	l = 0;
	while (val[l])
	{
		tmp[j].val = val[l];
		if (j != 0)
			tmp[j].lock = tmp[j - 1].lock;
		else
			tmp[j].lock = pars[j].lock;
		tmp[j].i = j;
		j++;
		l++;
	}
	tmp[j].i = -1;
	return (j);
}

int	i_end_param(t_cmd *data, int x, t_pars *pars, int i)
{
	i = data[x].i;
	i = pass_spaces(pars, i);
	return (i);
}

t_pars	*init_last_index(t_pars *pars, int i)
{
	pars[i].i = -1;
	pars[i].val = 'b';
	pars[i].d_q = 0;
	pars[i].s_q = 0;
	pars[i].null = 0;
	pars[i].lock = 0;
	return (pars);
}
