/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:08:21 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 13:47:47 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	supp_s_quote(t_pars *pars, int i)
{
	int		j;

	j = i;
	while (pars[j].i != -1)
	{
		pars[j] = pars[j + 1];
		j++;
	}
	pars[j - 1].i = -1;
	while (pars[i].s_q == 1)
		i++;
	j = i;
	while (pars[j].i != -1)
	{
		pars[j] = pars[j + 1];
		j++;
	}
	pars[j - 1].i = -1;
}

void	supp_d_quote(t_pars *pars, int i)
{
	int		j;

	j = i;
	while (pars[j].i != -1)
	{
		pars[j] = pars[j + 1];
		j++;
	}
	pars[j].i = -1;
	while (pars[i].d_q == 1)
		i++;
	j = i;
	while (pars[j].i != -1)
	{
		pars[j] = pars[j + 1];
		j++;
	}
	pars[j].i = -1;
}

void	supp_empty_quote(t_pars *pars, int i)
{
	int		j;

	j = i;
	while (pars[j].i != -1)
	{
		if (pars[j + 2].i != -1)
		{
			pars[j] = pars[j + 2];
			j++;
		}
		else
			pars[j].i = -1;
	}
}

int	space_empty_quote(t_pars *pars, int i)
{
	int	ret;

	if (pars[i + 2].i == -1 || ((pars[i + 2].val == ' '
				|| pars[i + 2].val == '|') && pars[i + 2].lock == 0))
	{
		ret = i;
		pars[i].val = ' ';
		pars[i].d_q = 1;
		i++;
		while (pars[i].i != -1)
		{
			pars[i] = pars[i + 1];
			i++;
		}
		pars[i - 1].i = -1;
		i = ret;
		return (0);
	}
	return (1);
}

int	zero_empty_quote(t_pars *pars, int i)
{
	int	ret;

	if ((pars[i - 1].val == ' ' || pars[i - 1].val == '|')
		&& (pars[i + 2].i == -1 || (pars[i + 2].val == ' '
				|| pars[i + 2].val == '|')))
	{
		ret = i;
		pars[i].val = 0;
		pars[i].d_q = 1;
		pars[i].null = 1;
		i++;
		while (pars[i].i != -1)
		{
			pars[i] = pars[i + 1];
			i++;
		}
		pars[i - 1].i = -1;
		i = ret;
		return (0);
	}
	return (1);
}
