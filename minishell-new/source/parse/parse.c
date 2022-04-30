/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 23:13:24 by nachin            #+#    #+#             */
/*   Updated: 2022/04/26 15:53:49 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*start_data(t_cmd *data, int j, t_pars *pars, int i)
{
	data[j] = init_cmd(data[j]);
	data[j].nb_chevrons = find_nb_redir(pars, i);
	if (data[j].nb_chevrons == -1)
	{
		data[j].index = -1;
		free_cmd(data);
		return (NULL);
	}
	else if (data[j].nb_chevrons != 0)
		data = get_redir(pars, data, i, j);
	return (data);
}

t_cmd	*loop_split(t_pars *pars, t_cmd *data)
{
	int	i;
	int	j;
	int	nb;

	i = 0;
	j = 0;
	while (pars[i].i != -1)
	{
		data = start_data(data, j, pars, i);
		if (data == NULL)
			return (NULL);
		nb = get_nb_param(pars, i);
		data[j].user_input = malloc(sizeof(char *) * (nb + 1));
		if (data[j].user_input == NULL)
			ft_stop(data, pars);
		i = pass_spaces(pars, i);
		data = get_params(pars, data, i, j);
		i = data[j].i;
		if (i == -1 || data[j].i == -1)
			return (NULL);
		data[j].index = j;
		j++;
	}
	data[j].index = -1;
	return (data);
}

t_cmd	*split_cmd(t_pars *pars, t_cmd *data)
{
	int	i;
	int	j;
	int	cmd;

	j = 0;
	i = 0;
	cmd = get_nb_cmd(pars);
	data = malloc(sizeof(t_cmd) * (cmd + 1));
	if (data == NULL)
		ft_stop(data, pars);
	return (loop_split(pars, data));
}

t_pars	*assign_pars(char *line, t_pars *pars, t_env *env)
{
	int	i;

	i = 0;
	while (line[i] && line)
	{
		pars[i].val = line[i];
		pars[i].d_q = 0;
		pars[i].s_q = 0;
		pars[i].i = i;
		pars[i].lock = 0;
		pars[i].null = 0;
		i++;
	}
	pars = init_last_index(pars, i);
	if (secure_quote(pars) == -1 || is_first_pipe(pars) == -1)
	{
		pars[0].i = -1;
		return (pars);
	}
	pars = perform_expansion(pars, env);
	if (pars[0].i == -1)
		return (pars);
	pars = put_lock(pars);
	return (pars);
}

t_cmd	*parsing(char *line, t_cmd *data, t_env *env, t_export *export)
{
	int		len;
	t_pars	*pars;

	pars = NULL;
	if (line)
	{
		len = ft_strlen(line);
		if (len == 0)
			return (NULL);
		pars = malloc(sizeof(t_pars) * (len + 1));
		if (pars == NULL)
			return (NULL);
		pars = assign_pars(line, pars, env);
		if (pars[0].i == -1)
		{
			free(pars);
			return (NULL);
		}
		data = split_cmd(pars, data);
		free(pars);
		return (data);
	}
	return (empty_line(data, &env, &export));
}
