#  |  |  ___ \    \  |         |
#  |  |     ) |  |\/ |   _  |  |  /   _ 
# ___ __|  __/   |   |  (   |    <    __/ 
#    _|  _____| _|  _| \__,_| _|\_\ \___|
#                              by jcluzet
################################################################################
#                                     CONFIG                                   #
################################################################################

NAME        := minishell
CC        := clang
FLAGS    := -fsanitize=address -g3
RM      :=     rm -f

DIRECTORY_EXEC = source/exec/

DIRECTORY_PARSE = source/parse/

DIRECTORY_MAIN = source/main/

DIRECTORY_UTILS = source/utils/

DIRECTORY_BUILTINS = source/builtins/

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

SRC_MAIN    =   main.c\
                shell_loop.c

SRC_EXEC    =   env.c\
                export.c\
                run_cmd.c

SRC_PARSE   =   error.c\
                expansions.c\
                get_cmd.c\
                get_param.c\
                get_redir.c\
                leaks.c\
                parse.c\
                quotes.c\
                signal.c

SRC_UTILS   =   utils_dollar.c \
                utils_dollar2.c \
                utils_pars.c \
                utils_quotes.c \
                utils_redir.c \
                utils.c\

SRC_BUILTINS =  cmd_env.c\
                cmd_unset.c\
                cmd_export.c\
                          
PATH_MAIN = $(addprefix $(DIRECTORY_MAIN), $(SRC_MAIN))
PATH_EXEC = $(addprefix $(DIRECTORY_EXEC), $(SRC_EXEC))
PATH_PARSE = $(addprefix $(DIRECTORY_PARSE), $(SRC_PARSE))
PATH_UTILS = $(addprefix $(DIRECTORY_UTILS), $(SRC_UTILS))
PATH_BUILTINS = $(addprefix $(DIRECTORY_BUILTINS), $(SRC_BUILTINS))

ALL_OBJS =  $(PATH_MAIN:.c=.o) $(PATH_BUILTINS:.c=.o) $(PATH_EXEC:.c=.o) $(PATH_PARSE:.c=.o) $(PATH_UTILS:.c=.o)

${NAME}:		$(ALL_OBJS)
				$(CC) ${FLAGS} -lreadline -o ${NAME} $(ALL_OBJS) 

clean:
				$(RM)	$(ALL_OBJS)

fclean:			clean
				$(RM) $(ALL_OBJS) minishell

re:				fclean $(NAME)

.PHONY:			all clean fclean re
