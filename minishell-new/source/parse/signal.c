/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:08:03 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/19 23:26:32 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handler(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

void	child_handler(int signum)
{
	(void)signum;
	//printf("Quit (core dumped)\n");
	exit(131);
}

void	assign_signals_handler(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handler);
}
