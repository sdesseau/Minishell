/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:07:57 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 16:28:41 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	delete_quotes(t_pars *pars)
{
	int	i;

	i = 0;
	while (pars[i].i != -1)
	{
		if (pars[i].val == '\'' && pars[i].lock == 0 && pars[i + 1].s_q == 1)
			supp_s_quote(pars, i);
		else if (pars[i].val == '\"' && pars[i].lock == 0
			&& pars[i + 1].d_q == 1)
			supp_d_quote(pars, i);
		else if (((pars[i].val == '\"' && pars[i + 1].val == '\"')
				|| (pars[i].val == '\'' && pars[i + 1].val == '\''))
			&& pars[i].lock == 0 && pars[i + 1].lock == 0)
			supp_empty_quote(pars, i);
		if (((pars[i].val != '\'' && pars[i].val != '\"')
				|| ((pars[i].val == '\'' || pars[i].val == '\"')
					&& pars[i].lock == 1)) && pars[i].i != -1)
			i++;
	}
	i = 0;
	while (pars[i].i != -1)
	{
		pars[i].i = i;
		i++;
	}
}

int	check_empty_quote(t_pars *pars, int i)
{
	if (pars[i + 1].val == pars[i].val)
	{
		if (i == 0)
			return (space_empty_quote(pars, i));
		else
			return (zero_empty_quote(pars, i));
	}
	return (1);
}

int	check_quotes_d(t_pars *pars, int i)
{	
	if (check_empty_quote(pars, i) == 1)
	{
		i++;
		while (pars[i].val != '\"' && pars[i].i != -1)
		{
			pars[i].lock = 1;
			pars[i++].d_q = 1;
		}
		if (pars[i].i == -1)
		{
			error_quote('\"');
			return (-1);
		}
	}
	return (i);
}

int	check_quotes_s(t_pars *pars, int i)
{
	if (check_empty_quote(pars, i) == 1)
	{
		i++;
		while (pars[i].val != '\'' && pars[i].i != -1)
		{
			pars[i].lock = 1;
			pars[i++].s_q = 1;
		}
		if (pars[i].i == -1)
		{
			error_quote('\'');
			return (-1);
		}
	}
	return (i);
}

int	secure_quote(t_pars *pars)
{
	int	i;

	i = 0;
	while (pars[i].i != -1)
	{
		if (pars[i].val == '\'')
			i = check_quotes_s(pars, i);
		else if (pars[i].val == '\"')
			i = check_quotes_d(pars, i);
		if (i == -1)
			return (-1);
		i++;
	}
	delete_quotes(pars);
	return (1);
}
