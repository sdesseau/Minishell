/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_param.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:07:49 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/30 16:27:18 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_nb_param(t_pars *pars, int i)
{
	int	nb;

	nb = 0;
	while (pars[i].i != -1 && (pars[i].val != '|' || (pars[i].val == '|'
				&& pars[i].lock == 1)))
	{
		i = pass_spaces(pars, i);
		if (pars[i].null == 1)
		{
			i++;
			nb++;
			i = pass_spaces(pars, i);
		}
		while (pars[i].i != -1 && ((pars[i].val != ' '
					&& pars[i].val != '|') || ((pars[i].val == ' '
						|| pars[i].val == '|') && pars[i].lock == 1)))
			i++;
		i = pass_spaces(pars, i);
		nb++;
	}
	return (nb);
}

int	len_word(t_pars *pars, int i)
{
	int	len;

	len = 0;
	while (pars[i].i != -1)
	{
		if (pars[i].val == ' ' && pars[i].lock == 0)
			break ;
		else if (pars[i].val == '|' && pars[i].lock == 0)
			break ;
		i++;
		len++;
	}
	return (len);
}

t_cmd	put_pipe(t_cmd data, t_pars *pars, int i, int j)
{
	while (pars[i].i != -1)
	{
		i++;
		i = pass_spaces(pars, i);
		if (pars[i].i != -1 && (pars[i].val != '|'
				|| (pars[i].val == '|' && pars[i].lock == 1)))
		{
			data.user_input[j] = NULL;
			data.pipe = 1;
			data.i = i;
			return (data);
		}
		else if (pars[i].i == -1 || (pars[i].val == '|'
				&& pars[i].s_q == 0 && pars[i].d_q == 0))
		{
			error_parse('|');
			data.i = -1;
			return (data);
		}
	}
	data.i = i;
	return (data);
}

t_cmd	assign_data(t_cmd data, int j, t_pars *pars, int i)
{
	int	k;
	int	l;

	l = len_word(pars, i);
	k = 0;
	while (k < l)
	{
		data.user_input[j][k] = pars[i].val;
		k++;
		i++;
	}
	data.user_input[j][k] = '\0';
	data.i = i;
	return (data);
}

t_cmd	*get_params(t_pars *pars, t_cmd *data, int i, int x)
{
	int	j;

	j = 0;
	while (pars[i].i != -1 && (pars[i].val != '|' || (pars[i].val == '|'
				&& pars[i].lock == 1)))
	{
		if (len_word(pars, i) != 0)
		{
			data[x].user_input[j] = malloc(sizeof(char)
					* (len_word(pars, i) + 1));
			if (data[x].user_input[j] == NULL)
				ft_stop(data, pars);
		}
		else
			break ;
		data[x] = assign_data(data[x], j, pars, i);
		j++;
		i = i_end_param(data, x, pars, i);
	}
	if (pars[i].i != -1 && pars[i].val == '|')
		data[x] = put_pipe(data[x], pars, i, j);
	else
		data[x].i = i;
	data[x].user_input[j] = NULL;
	return (data);
}
