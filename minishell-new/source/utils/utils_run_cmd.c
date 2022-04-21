/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_run_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:56:35 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/21 18:59:03 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	nb_of_pipe(t_cmd *cmd)
{
	int	i;
	int	nb_pipe;

	i = 0;
	nb_pipe = 1;
	while (cmd[i].index != -1)
	{
		if (cmd[i].pipe == 1)
			nb_pipe++;
		i++;
	}
	return (nb_pipe);
}
