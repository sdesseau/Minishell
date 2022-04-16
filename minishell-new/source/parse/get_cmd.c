/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:07:47 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 16:28:41 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_nb_cmd(t_pars *pars)
{
	int	cmd;
	int	i;

	cmd = 1;
	i = 0;
	while (pars[i].i != -1)
	{
		if (pars[i].val == '|' && pars[i].s_q == 0 && pars[i].d_q == 0)
			cmd++;
		i++;
	}
	return (cmd);
}
