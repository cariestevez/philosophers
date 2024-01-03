#gcc -Wall -Wextra -Werror ft_atoi.c main.c
NAME := philo
CFLAGS := -g -Wall -Wextra -Werror
#INC := -Iinc
SRCS := 	main.c ft_atoi.c
OBJS := $(SRCS:.c=.o)

all: ${NAME}

${NAME}: ${OBJS}
	cc -o ${NAME} $^

.c.o:
	cc ${CFLAGS} -c -o $@ $<

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re