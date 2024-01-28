NAME := philo
CFLAGS := -g -pthread -Wall -Wextra -Werror
#INC := -Iinc
SRCS := 	main.c init_and_free.c monitoring.c routine.c utils_1.c utils_2.c
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