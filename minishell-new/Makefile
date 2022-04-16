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
FLAGS    := 
RM      :=     rm -f

DIRECTORY_EXEC = source/exec/

DIRECTORY_PARSE = source/parse/

DIRECTORY_MAIN = source/main/

DIRECTORY_UTILS = source/utils/

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
                          
PATH_MAIN = $(addprefix $(DIRECTORY_MAIN), $(SRC_MAIN))
PATH_EXEC = $(addprefix $(DIRECTORY_EXEC), $(SRC_EXEC))
PATH_PARSING = $(addprefix $(DIRECTORY_PARSE), $(SRC_PARSE))
PATH_UTILS = $(addprefix $(DIRECTORY_UTILS), $(SRC_UTILS))

ALL_OBJS =  $(PATH_MAIN:.c=.o)  $(PATH_EXEC:.c=.o) $(PATH_PARSING:.c=.o) $(PATH_UTILS:.c=.o)

.c.o:
	${CC} ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}:		$(ALL_OBJS)
				$(CC)   -o ${NAME} $(ALL_OBJS)  ${CFLAGS} -lreadline 

clean:
				$(RM)	$(ALL_OBJS)

fclean:			clean
				$(RM) $(ALL_OBJS) minishell

re:				fclean $(NAME)

.PHONY:			all clean fclean re
