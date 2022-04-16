/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:23:02 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/16 18:31:18 by mprigent         ###   ########.fr       */
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

extern unsigned char	g_exit_code;

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

typedef struct s_cmd
{
	char	**user_input;
	int		nb_chevrons;
	int		pipe;
	char	**path;
	int		index;
	int		i;
	int		fd_stdin;
	int		fd_stdout;
}				t_cmd;

//				MAIN				//
int		main(int argc, char **argv, char **envp);
void	shell_loop(t_env *env, t_export *export);

//				EXEC				//
void	recup_export(char **envp, t_export **export);
void    recup_env(char **envp, t_env **env);
int     get_length_name(char *envp);

//				PARSING				//
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

//				BUILTINS				//

int	ft_is_flag_n(char *user_input);
void ft_print_echo(char *cmd);
int ft_echo(char **argv);
int ft_check_builtins(char **argv);
int	ft_execute_builtins(char **argv);

//				UTILS				//

void	*ft_calloc(size_t count, size_t size);
char	*ft_strrchr(const char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int 	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
char	*find_env_value(char *env_name, t_env *env);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
void	ft_bzero(void *s, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putchar_fd(char c, int fd);

#endif