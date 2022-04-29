/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:08:08 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/29 16:27:40 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_lenght_pars(t_pars *pars)
{
	int	len;

	len = 0;
	while (pars[len].i != -1)
		len++;
	return (len);
}

int	len_exitcode(void)
{
	int	len;
	int	value;

	value = (int)g_exit_code;
	if (value == 0 || value == 256)
		return (1);
	if (value > 256 || value < 0)
		value = value % 256;
	len = 0;
	while (value > 0)
	{
		value = value / 10;
		len++;
	}
	return (len);
}

int	get_len_name(t_pars *pars, int i)
{
	int	j;

	j = i;
	while (pars[j].i != -1 && ((pars[j].val != ' ' && pars[j].val != '=' && pars[j].val != '/'
				&& pars[j].val != '$' && pars[j].val != '\\')
			|| ((pars[j].val == ' ' || pars[j].val == '=' || pars[j].val == '/'
					|| pars[j].val == '$' || pars[j].val == '\\')
				&& pars[j].lock == 1))
				&& pars[j - 1].lock == pars[j].lock)
		j++;
	return (j);
}

char	*get_name(t_pars *pars, int i, char *name)
{
	int	j;

	j = get_len_name(pars, i);
	if (name == NULL)
	{
		free(pars);
		printf("Malloc Error\nexit\n");
		exit(131);
	}
	name[j - i] = '\0';
	while (j-- > i)
		name[j - i] = pars[j].val;
	return (name);
}

t_pars	*no_value(t_pars *pars, int i, int k)
{
	k = i - 1;
	while (pars[i].i != -1 && ((pars[i].val != ' ' && pars[i].val != '/'
				&& pars[i].val != '$' && pars[i].val != '\\')
			|| ((pars[i].val == ' ' || pars[i].val == '/'
					|| pars[i].val == '$' || pars[i].val == '\\')
				&& pars[i].lock == 1)))
		i++;
	while (pars[i].i != -1)
	{
		pars[k] = pars[i];
		pars[k].i = k;
		k++;
		i++;
	}
	pars[k].i = -1;
	return (pars);
}
