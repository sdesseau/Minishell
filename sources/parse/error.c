/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:07:32 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 13:47:29 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	error_parse(char c)
{
	int	i;

	i = ft_strlen("minishell: parse error near ");
	write(1, "minishell: parse error near ", i);
	write(1, "'", 1);
	write(1, &c, 1);
	write(1, "'\n", 2);
}

void	syntax_error(char c)
{
	int	i;

	i = ft_strlen("minishell: syntax error near unexpected token ");
	write(1, "minishell: syntax error near unexpected token ", i);
	write(1, "'", 1);
	write(1, &c, 1);
	write(1, "'\n", 2);
	g_exit_code = 2;
}

void	error_quote(char c)
{
	int	i;

	i = ft_strlen("minishell: unexpected EOF while looking for matching ");
	write(1, "minishell: unexpected EOF while looking for matching ", i);
	write(1, "'", 1);
	write(1, &c, 1);
	write(1, "'\n", 2);
	g_exit_code = 2;
}
