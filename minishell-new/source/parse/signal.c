/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:08:03 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/30 16:28:50 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handler(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	interrupt_here_document(int signal)
{
	(void)signal;
	g_exit_code = 130;
	write(1, "\n", 1);
	exit(131);
}

void	assign_signals_handler(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handler);
}
