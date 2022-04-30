/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:07:35 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/29 16:27:06 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_pars	*new_parse_dollar(t_pars *pars, t_pars *tmp, int i, char *val)
{
	int		j;

	j = 0;
	while (j != i)
	{
		tmp[j] = pars[j];
		j++;
	}
	j = assign_new_pars(val, tmp, pars, j);
	i = get_len_name(pars, i + 1);
	while (pars[i].i != -1)
	{
		tmp[j] = pars[i++];
		tmp[j].i = j;
		j++;
	}
	tmp[j].i = -1;
	return (tmp);
}

t_pars	*change_da_dolla(t_pars *pars, int i, char *val, int lenght)
{
	t_pars	*tmp;
	int		len;

	len = ft_strlen(val);
	tmp = malloc(sizeof(t_pars) * ((len + lenght) + 1));
	if (tmp == NULL)
	{
		free(pars);
		return (NULL);
	}
	tmp = new_parse_dollar(pars, tmp, i, val);
	free(pars);
	pars = tmp;
	return (pars);
}

t_pars	*get_exitcode(t_pars *pars, int i, int lenght)
{
	char	*value;
	int		len;
	int		exitcode;

	len = len_exitcode();
	value = NULL;
	value = malloc(sizeof(char) * (len + 1));
	value[len] = '\0';
	exitcode = (int)g_exit_code;
	if (exitcode < 0 || exitcode > 256)
		exitcode %= 256;
	len--;
	while (len >= 0)
	{
		value[len] = (exitcode % 10) + 48;
		exitcode /= 10;
		len--;
	}
	pars = change_da_dolla(pars, i, value, lenght);
	free(value);
	return (pars);
}

t_pars	*get_expansion(t_pars *pars, int i, int lenght, t_env *env)
{
	int		k;
	char	*name;
	char	*value;

	i++;
	k = get_len_name(pars, i);
	name = malloc(sizeof(char) * ((k - i) + 1));
	name = get_name(pars, i, name);
	lenght -= (ft_strlen(name) + 1);
	if (find_env_value(name, env) == NULL)
		pars = no_value(pars, i, k);
	else
	{
		value = find_env_value(name, env);
		pars = change_da_dolla(pars, i - 1, value, lenght);
	}
	free(name);
	return (pars);
}

t_pars	*perform_expansion(t_pars *pars, t_env *env)
{
	int		i;
	int		lenght;

	i = 0;
	while (pars[i].i != -1)
	{
		lenght = get_lenght_pars(pars);
		if (pars[i].val == '$' && pars[i].s_q == 0 && pars[i + 1].val == '?')
			pars = get_exitcode(pars, i, lenght);
		else if (pars[i].val == '$' && pars[i + 1].i == -1)
			i++;
		else if (pars[i].val == '$' && pars[i].s_q == 0)
			pars = get_expansion(pars, i, lenght, env);
		if (pars[i].i != -1 && ((pars[i].val != '$')
				|| (pars[i].val == '$' && pars[i].lock == 1)))
			i++;
	}
	return (pars);
}
