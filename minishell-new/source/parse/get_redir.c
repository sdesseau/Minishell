/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:07:51 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 16:28:41 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	find_nb_redir(t_pars *pars, int i)
{
	int	nb;

	nb = 0;
	while (pars[i].i != -1 && (pars[i].val != '|'
			|| (pars[i].val == '|' && pars[i].lock == 1)))
	{
		if ((pars[i].val == '<' || pars[i].val == '>')
			&& pars[i].s_q == 0 && pars[i].d_q == 0)
		{
			if (check_false_double_redir(pars, i) == 0)
				return (-1);
			if (check_triple_chevrons(pars, i) == 0)
				return (-1);
			if (check_path(pars, i) == 0)
				return (-1);
			i = pass_redir(pars, i);
			nb++;
		}
		else if (pars[i].i != -1)
			i++;
	}
	return (nb);
}

int	get_len_path(t_pars *pars, int i)
{
	int	len;

	len = 0;
	i = go_redir(pars, i);
	while ((pars[i].val == '<' || pars[i].val == '>') && pars[i].i != -1)
	{
		i++;
		len++;
	}
	i = pass_spaces(pars, i);
	while (((pars[i].val != ' ' && pars[i].val != '|' && pars[i].val != '<'
				&& pars[i].val != '>') || ((pars[i].val == ' '
					|| pars[i].val == '|' || pars[i].val == '<'
					|| pars[i].val == '>') && pars[i].lock == 1))
		&& pars[i].i != -1)
	{
		len++;
		i++;
	}
	return (len);
}

t_pars	*erase_redir(t_pars *pars, int i)
{
	int	ret;

	ret = i;
	i = pass_spaces(pars, i);
	while (pars[i].val == '<' || pars[i].val == '>')
		i++;
	i = pass_spaces(pars, i);
	while (((pars[i].val != ' ' && pars[i].val != '|' && pars[i].val != '<'
				&& pars[i].val != '>') || ((pars[i].val == ' '
					|| pars[i].val == '|' || pars[i].val == '<'
					|| pars[i].val == '>') && pars[i].lock == 1))
		&& pars[i].i != -1)
		i++;
	while (pars[i].i != -1)
	{
		pars[ret] = pars[i++];
		pars[ret].i = ret;
		ret++;
	}
	pars[ret].i = -1;
	return (pars);
}

t_cmd	write_path(t_pars *pars, t_cmd data, int i, int j)
{
	int	k;
	int	ret;

	k = 0;
	ret = i;
	while (pars[i].val == '<' || pars[i].val == '>')
		data.path[j][k++] = pars[i++].val;
	i = pass_spaces(pars, i);
	while (((pars[i].val != ' ' && pars[i].val != '|' && pars[i].val != '<'
				&& pars[i].val != '>') || ((pars[i].val == ' '
					|| pars[i].val == '|' || pars[i].val == '<'
					|| pars[i].val == '>') && pars[i].lock == 1))
		&& pars[i].i != -1)
		data.path[j][k++] = pars[i++].val;
	data.path[j][k] = '\0';
	return (data);
}

t_cmd	*get_redir(t_pars *pars, t_cmd *data, int i, int l)
{
	int	j;

	j = 0;
	data[l].path = malloc(sizeof(char *) * (data[l].nb_chevrons + 1));
	if (data[l].path == NULL)
		ft_stop(data, pars);
	while (pars[i].i != -1 && (j < data[l].nb_chevrons) && (pars[i].val != '|'
			|| (pars[i].val == '|' && pars[i].lock == 1)))
	{
		if (get_len_path(pars, i) != 0)
		{
			data[l].path[j] = malloc(sizeof(char)
					* (get_len_path(pars, i) + 1));
			if (data[l].path[j] == NULL)
				ft_stop(data, pars);
		}
		else
			break ;
		i = go_redir(pars, i);
		data[l] = write_path(pars, data[l], i, j++);
		pars = erase_redir(pars, i);
	}
	data[l].path[j] = NULL;
	return (data);
}
