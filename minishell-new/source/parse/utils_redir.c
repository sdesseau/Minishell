/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:08:24 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 13:47:47 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_false_double_redir(t_pars *pars, int i)
{
	if (pars[i].val == '<' && pars[i + 1].val == '>' && pars[i + 1].s_q == 0
		&& pars[i + 1].d_q == 0)
	{
		syntax_error(pars[i].val);
		return (0);
	}
	else if (pars[i].val == '>' && pars[i + 1].val == '<'
		&& pars[i + 1].s_q == 0 && pars[i + 1].d_q == 0)
	{
		syntax_error(pars[i].val);
		return (0);
	}
	return (1);
}

int	check_triple_chevrons(t_pars *pars, int i)
{
	if (pars[i + 1].i == -1)
	{
		syntax_error(pars[i].val);
		return (0);
	}
	if ((pars[i + 1].val == '<' || pars[i + 1].val == '>'))
	{
		i++;
		if ((pars[i + 1].val == '<' || pars[i + 1].val == '>')
			&& pars[i].lock == 0)
		{
			syntax_error(pars[i + 1].val);
			return (0);
		}
	}
	return (1);
}

int	check_path(t_pars *pars, int i)
{
	int	ret;

	ret = i;
	while ((pars[i].val == '<' || pars[i].val == '>')
		&& pars[i].s_q == 0 && pars[i].d_q == 0)
		i++;
	i = pass_spaces(pars, i);
	if (pars[i].i == -1 || (pars[i].val == '|'
			&& pars[i].s_q == 0 && pars[i].d_q == 0))
	{
		if (pars[i].i == -1)
			syntax_error(pars[ret].val);
		else
			syntax_error('|');
		return (0);
	}
	else if ((pars[i].val == '<' || pars[i].val == '>')
		&& pars[i].s_q == 0 && pars[i].d_q == 0)
	{
		syntax_error(pars[i].val);
		return (0);
	}
	return (1);
}

int	pass_redir(t_pars *pars, int i)
{
	while (pars[i].val == '<' || pars[i].val == '>')
		i++;
	i = pass_spaces(pars, i);
	while (pars[i].i != -1)
	{
		if (pars[i].val == ' ' || pars[i].val == '|'
			|| pars[i].val == '<' || pars[i].val == '>')
		{
			if (pars[i].s_q == 1 || pars[i].d_q == 1)
				i++;
			else
				break ;
		}
		else
			i++;
	}
	return (i);
}

int	go_redir(t_pars *pars, int i)
{
	while (pars[i].val != '<' && pars[i].val != '>' && pars[i].i != -1)
	{
		i++;
		while ((pars[i].val == '<' || pars[i].val == '>')
			&& (pars[i].s_q == 1 || pars[i].d_q == 1)
			&& pars[i].i != -1)
			i++;
	}
	return (i);
}
