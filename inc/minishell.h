/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:23:02 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 15:12:37 by mprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <unistd.h>
# include <limits.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

# include "parse.h"
# include "exec.h"

typedef struct s_cmd_data
{
	char	**user_input;
	int		nb_chevrons;
	int		pipe;
	char	**path;
	int		index;
	int		i;
}				t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_export
{
	char			*name;
	char			*value;
	int				is_equal;
	struct s_export	*next;
}					t_export;
