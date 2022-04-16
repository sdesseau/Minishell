/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdesseau <sdesseau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 13:54:00 by nachin            #+#    #+#             */
/*   Updated: 2022/04/16 13:50:11 by sdesseau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

typedef struct s_pars
{
	char	val;
	char	t;
	int		d_q;
	int		s_q;
	int		i;
	int		lock;
	int		null;
}				t_pars;

t_cmd	*parsing(char *line, t_cmd *data, t_env *env);
t_pars		*assign_pars(char *line, t_pars *pars, t_env *env);
void		assign_type(char *line, t_pars *pars, int i);
t_cmd	*loop_split(t_pars *pars, t_cmd *data);
t_cmd	*empty_line(t_cmd *data);

t_pars		*perform_expansion(t_pars *pars, t_env *env);
t_pars		*change_da_dolla(t_pars *pars, int i, char *val, int lenght);
t_pars		*get_exitcode(t_pars *pars, int i, int lenght);
int			len_exitcode(void);
t_pars		*no_value(t_pars *pars, int i, int k);
int			assign_new_pars(char *val, t_pars *tmp, t_pars *pars, int j);
int			get_len_name(t_pars *pars, int i);
t_pars		*get_expansion(t_pars *pars, int i, int lenght, t_env *env);
t_pars		*new_parse_dollar(t_pars *pars, t_pars *tmp, int i, char *val);

t_pars		*put_lock(t_pars *pars);
void		delete_quotes(t_pars *pars);
int			secure_quote(t_pars *pars);
int			check_quotes_s(t_pars *pars, int i);
int			check_quotes_d(t_pars *pars, int i);
int			space_empty_quote(t_pars *pars, int i);
int			zero_empty_quote(t_pars *pars, int i);
int			check_empty_quote(t_pars *pars, int i);
void		supp_empty_quote(t_pars *pars, int i);
void		supp_s_quote(t_pars *pars, int i);
void		supp_d_quote(t_pars *pars, int i);

t_cmd	*start_data(t_cmd *data, int j, t_pars *pars, int i);
t_cmd	*split_cmd(t_pars *pars, t_cmd *data);
int			get_nb_cmd(t_pars *pars);

int			get_nb_param(t_pars *pars, int i);
t_cmd	*get_params(t_pars *pars, t_cmd *data, int i, int j);
int			len_word(t_pars *pars, int i);
t_cmd	put_pipe(t_cmd data, t_pars *pars, int i, int j);
t_cmd	assign_data(t_cmd data, int j, t_pars *pars, int i);
int			i_end_param(t_cmd *data, int x, t_pars *pars, int i);

int			check_false_double_redir(t_pars *pars, int i);
int			check_triple_chevrons(t_pars *pars, int i);
int			check_path(t_pars *pars, int i);
int			pass_redir(t_pars *pars, int i);
int			get_len_path(t_pars *pars, int i);
t_pars		*erase_redir(t_pars *pars, int i);
t_cmd	*get_redir(t_pars *pars, t_cmd *data, int i, int l);
int			find_nb_redir(t_pars *pars, int i);
int			go_redir(t_pars *pars, int i);
t_cmd	write_path(t_pars *pars, t_cmd data, int i, int j);

t_cmd	init_cmd(t_cmd data);
void		print_data(t_cmd *data);

void		error_parse(char c);
void		syntax_error(char c);
void		error_quote(char c);

void		ft_stop(t_cmd *data, t_pars *pars);
void		stop_loop(t_cmd data);
void		loop_path(t_cmd data);

int			pass_spaces(t_pars *pars, int i);
int			get_lenght_pars(t_pars *pars);
char		*get_name(t_pars *pars, int i, char *name);
int			is_first_pipe(t_pars *pars);
t_pars		*init_last_index(t_pars *pars, int i);

void		assign_signals_handler(void);
void		child_handler(int signum);
void		handler(int signum);

#endif
