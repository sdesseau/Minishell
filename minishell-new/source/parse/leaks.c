/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:07:53 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 16:28:41 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	loop_path(t_cmd data)
{
	int	k;

	k = 0;
	while (data.path[k])
	{
		free(data.path[k++]);
		if (data.path[k] == NULL)
		{
			free(data.path[k]);
			return ;
		}
	}
}

void	stop_loop(t_cmd data)
{
	int	j;

	j = 0;
	while (data.user_input[j])
	{
		free(data.user_input[j++]);
		if (data.user_input[j] == NULL)
		{
			free(data.user_input[j]);
			break ;
		}
	}
	free(data.user_input);
	if (data.nb_chevrons > 0)
	{
		loop_path(data);
		free(data.path);
	}
}

void	ft_stop(t_cmd *data, t_pars *pars)
{
	int	i;

	i = 0;
	if (data)
	{
		while (data[i].index != -1)
		{
			stop_loop(data[i]);
			i++;
		}
		free(data);
	}
	if (pars)
		free(pars);
	printf("Malloc Error\n");
	printf("exit\n");
	exit(131);
}
