/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprigent <mprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/16 13:23:02 by sdesseau          #+#    #+#             */
/*   Updated: 2022/04/21 23:31:48 by mprigent         ###   ########.fr       */
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
# include <sys/stat.h> 
# include <sys/wait.h>
# define STDIN 0
# define STDOUT 1
# define STDERROR 2
# define LONGLONG_MIN -9223372036854775807
# define LONGLONG_MAX 9223372036854775807

unsigned char	g_exit_code;

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
	int		fd_pipe[2];
}				t_cmd;

//				MAIN				//
/* -------------------------------------------------------------------------- */
/*                         FILE = source/main/main.c                          */
/* -------------------------------------------------------------------------- */
int			main(int argc, char **argv, char **envp);

/* -------------------------------------------------------------------------- */
/*                      FILE = source/main/shell_loop.c                       */
/* -------------------------------------------------------------------------- */
void		shell_loop(t_env *env, t_export *export);

//				BUILTINS				//
/* -------------------------------------------------------------------------- */
/*                   FILE = source/builtins/check_builtins.c                  */
/* -------------------------------------------------------------------------- */
int			ft_check_builtins(char *cmd);
int			ft_execute_builtins(t_cmd cmd, t_env **env, t_export **export);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/builtins/ft_cd.c                           */
/* -------------------------------------------------------------------------- */
char		*ft_home_path(char *path, t_env **env);
int			ft_update_pwd(char *path, int home, t_env **env);
int			ft_set_directory(char *path, int home, t_env **env);
int			ft_path(char **argv, t_env **env);
int			ft_cd(char **argv, t_env **env);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/builtins/ft_echo.c                         */
/* -------------------------------------------------------------------------- */
int			ft_is_flag_n(char *user_input);
void		ft_print_echo(char *cmd);
int			ft_echo(char **argv);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/builtins/ft_env.c                          */
/* -------------------------------------------------------------------------- */
void		print_env(t_env *env);
int			env_command(char **argv, t_env *env);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/builtins/ft_exit.c                         */
/* -------------------------------------------------------------------------- */
int			ft_is_arg_longlong(char *cmd);
int			ft_is_exit_arg_num(char *cmd);
int			ft_exit(char **argv);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/builtins/ft_export.c                       */
/* -------------------------------------------------------------------------- */
char		**sort_export(t_export *export);
char		*get_new_name(char *command, int *index);
char		*find_export_value(char *name, t_export *export);
int			find_equal_value(char *name, t_export *export);
void		print_export(char **tab_export, t_export *export);
int			export_command(char **argv, t_env **env, t_export **export);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/builtins/ft_pwd.c                          */
/* -------------------------------------------------------------------------- */
int			ft_pwd(void);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/builtins/ft_unset.c                        */
/* -------------------------------------------------------------------------- */
void		unset_in_env(char *argv, t_env **env);
void		unset_in_export(char *argv, t_export **export);
int			is_unset_arg_valid(char *arg);
int			unset_command(char **argv, t_env **env, t_export **export);
int			unset_error(int error_id);

//				EXEC				//
/* -------------------------------------------------------------------------- */
/*                           FILE = source/exec/env.c                         */
/* -------------------------------------------------------------------------- */
void		link_new_env_var(t_env **new, char*name, t_env **env);
int			update_env(char *name, char *val, t_env **env);
int			env_var_already_exist(char *name, t_env *env);
void		add_env_var(char *name, char *val, t_env **env);
void		recup_env(char **envp, t_env **env);

/* -------------------------------------------------------------------------- */
/*                         FILE = source/exec/export.c                        */
/* -------------------------------------------------------------------------- */
void		link_new_export_var(t_export **new, char*name, t_export **export);
int			update_export(char *name, char *value, t_export **export);
int			export_var_already_exist(char *name, t_export *export);
int			add_export_var(char *name, char *val, char *line, t_export **export);
void		recup_export(char **envp, t_export **export);

/* -------------------------------------------------------------------------- */
/*                     FILE = source/exec/external_cmd.c                      */
/* -------------------------------------------------------------------------- */
char		**ft_get_path(t_env **env);
int			ft_check_permission(char **cmd, char *ext_cmd,
				struct stat statbuf, t_env *env);
int			ft_execute_external_cmd(char **cmd, t_env *env);

/* -------------------------------------------------------------------------- */
/*                         FILE = source/exec/run_cmd.c                       */
/* -------------------------------------------------------------------------- */
int			heredoc(char *path);
int			input(char **path, int tmp_stdin);
void		child_process(t_cmd cmd, t_env *env, t_export *export);
int			output(char **path, int tmp_stdout);
void		exec_single_cmd(t_cmd cmd, t_env **env, t_export **export, int tmp);
void		run_commands(t_cmd *cmd, t_env **env, t_export **export);

//				PARSING				//
/* -------------------------------------------------------------------------- */
/*                         FILE = source/parse/error.c                        */
/* -------------------------------------------------------------------------- */
void		error_parse(char c);
void		syntax_error(char c);
void		error_quote(char c);

/* -------------------------------------------------------------------------- */
/*                    FILE = source/parse/expansions.c                        */
/* -------------------------------------------------------------------------- */
t_pars		*new_parse_dollar(t_pars *pars, t_pars *tmp, int i, char *val);
t_pars		*change_da_dolla(t_pars *pars, int i, char *val, int lenght);
t_pars		*get_exitcode(t_pars *pars, int i, int lenght);
t_pars		*get_expansion(t_pars *pars, int i, int lenght, t_env *env);
t_pars		*perform_expansion(t_pars *pars, t_env *env);

/* -------------------------------------------------------------------------- */
/*                       FILE = source/parse/get_cmd.c                        */
/* -------------------------------------------------------------------------- */
int			get_nb_cmd(t_pars *pars);

/* -------------------------------------------------------------------------- */
/*                     FILE = source/parse/get_param.c                        */
/* -------------------------------------------------------------------------- */
int			get_nb_param(t_pars *pars, int i);
int			len_word(t_pars *pars, int i);
t_cmd		put_pipe(t_cmd data, t_pars *pars, int i, int j);
t_cmd		assign_data(t_cmd data, int j, t_pars *pars, int i);
t_cmd		*get_params(t_pars *pars, t_cmd *data, int i, int j);

/* -------------------------------------------------------------------------- */
/*                     FILE = source/parse/get_redir.c                        */
/* -------------------------------------------------------------------------- */
int			find_nb_redir(t_pars *pars, int i);
int			get_len_path(t_pars *pars, int i);
t_pars		*erase_redir(t_pars *pars, int i);
t_cmd		write_path(t_pars *pars, t_cmd data, int i, int j);
t_cmd		*get_redir(t_pars *pars, t_cmd *data, int i, int l);

/* -------------------------------------------------------------------------- */
/*                         FILE = source/parse/leaks.c                        */
/* -------------------------------------------------------------------------- */
void		loop_path(t_cmd data);
void		stop_loop(t_cmd data);
void		ft_stop(t_cmd *data, t_pars *pars);

/* -------------------------------------------------------------------------- */
/*                         FILE = source/parse/parse.c                        */
/* -------------------------------------------------------------------------- */
t_cmd		*start_data(t_cmd *data, int j, t_pars *pars, int i);
t_cmd		*loop_split(t_pars *pars, t_cmd *data);
t_cmd		*split_cmd(t_pars *pars, t_cmd *data);
t_pars		*assign_pars(char *line, t_pars *pars, t_env *env);
t_cmd		*parsing(char *line, t_cmd *data, t_env *env);

/* -------------------------------------------------------------------------- */
/*                        FILE = source/parse/quotes.c                        */
/* -------------------------------------------------------------------------- */
void		delete_quotes(t_pars *pars);
int			check_empty_quote(t_pars *pars, int i);
int			check_quotes_d(t_pars *pars, int i);
int			check_quotes_s(t_pars *pars, int i);
int			secure_quote(t_pars *pars);

/* -------------------------------------------------------------------------- */
/*                        FILE = source/parse/signal.c                        */
/* -------------------------------------------------------------------------- */
void		handler(int signum);
void		child_handler(int signum);
void		assign_signals_handler(void);

//				UTILS				//
/* -------------------------------------------------------------------------- */
/*                   FILE = source/utils/ft_split_utils.c                     */
/* -------------------------------------------------------------------------- */
int			ft_count_word(char *str, char c);
int			ft_len_word(char *str, char c);
char		*ft_dup(char *src, char c);
char		**ft_free(char **tab);

/* -------------------------------------------------------------------------- */
/*                      FILE = source/utils/ft_split.c                        */
/* -------------------------------------------------------------------------- */
char		**ft_split_and_fill_array(char *str, char c, char **tab);
char		**ft_split(char const *s, char c);

/* -------------------------------------------------------------------------- */
/*                  FILE = source/utils/utils_dollar.c                        */
/* -------------------------------------------------------------------------- */
int			get_lenght_pars(t_pars *pars);
int			len_exitcode(void);
int			get_len_name(t_pars *pars, int i);
char		*get_name(t_pars *pars, int i, char *name);
t_pars		*no_value(t_pars *pars, int i, int k);

/* -------------------------------------------------------------------------- */
/*                 FILE = source/utils/utils_dollar2.c                        */
/* -------------------------------------------------------------------------- */
int			assign_new_pars(char *val, t_pars *tmp, t_pars *pars, int j);
int			i_end_param(t_cmd *data, int x, t_pars *pars, int i);
t_pars		*init_last_index(t_pars *pars, int i);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/utils/utils_env.c                          */
/* -------------------------------------------------------------------------- */
void		*free_env_var(t_env *env);
int			get_length_name(char *envp);
void		put_in_env(t_env **new, char *name, char *value);
char		*find_env_value(char *env_name, t_env *env);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/utils/utils_export.c                       */
/* -------------------------------------------------------------------------- */
void		put_in_export(t_export **new, char *name, char *value);
void		*free_export_var(t_export *export);
int			is_equal_sign(char *name);
int			ft_lenlist(t_export *lst);
char		**convert_list_to_tab(t_export *export);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/utils/utils_ext_cmd.c                      */
/* -------------------------------------------------------------------------- */
int			ft_size_env(t_env *lst);
char		*ft_convert_env(char *name, char *value);
char		**ft_conv_env_to_tab(t_env *env);

/* -------------------------------------------------------------------------- */
/*                    FILE = source/utils/utils_pars.c                        */
/* -------------------------------------------------------------------------- */
t_cmd		init_cmd(t_cmd data);
t_pars		*put_lock(t_pars *pars);
t_cmd		*empty_line(t_cmd *data);
int			pass_spaces(t_pars *pars, int i);
int			is_first_pipe(t_pars *pars);

/* -------------------------------------------------------------------------- */
/*                    FILE = source/utils/utils_putfd.c.c                     */
/* -------------------------------------------------------------------------- */
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *str, int fd);
void		ft_putnbr_fd(int n, int fd);

/* -------------------------------------------------------------------------- */
/*                  FILE = source/utils/utils_quotes.c                        */
/* -------------------------------------------------------------------------- */
void		supp_s_quote(t_pars *pars, int i);
void		supp_d_quote(t_pars *pars, int i);
void		supp_empty_quote(t_pars *pars, int i);
int			space_empty_quote(t_pars *pars, int i);
int			zero_empty_quote(t_pars *pars, int i);

/* -------------------------------------------------------------------------- */
/*                  FILE = source/utils/utils_redir.c                         */
/* -------------------------------------------------------------------------- */
int			check_false_double_redir(t_pars *pars, int i);
int			check_triple_chevrons(t_pars *pars, int i);
int			check_path(t_pars *pars, int i);
int			pass_redir(t_pars *pars, int i);
int			go_redir(t_pars *pars, int i);

/* -------------------------------------------------------------------------- */
/*                   FILE = source/utils/utils_run_cmd.c                      */
/* -------------------------------------------------------------------------- */
int			nb_of_pipe(t_cmd *cmd);

/* -------------------------------------------------------------------------- */
/*                    FILE = source/utils/utils_str.c                         */
/* -------------------------------------------------------------------------- */
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
char		*ft_strdup(const char *s1);

/* -------------------------------------------------------------------------- */
/*                    FILE = source/utils/utils_str_2.c                       */
/* -------------------------------------------------------------------------- */
int			ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
char		*ft_substr(char const *s, unsigned int start, size_t len);

/* -------------------------------------------------------------------------- */
/*                         FILE = source/utils/utils.c                        */
/* -------------------------------------------------------------------------- */
void		*ft_calloc(size_t count, size_t size);
void		ft_bzero(void *s, size_t n);
long long	ft_atoi(const char *str);

#endif
