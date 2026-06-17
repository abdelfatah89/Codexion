NAME = codexion
SRCS = src/cleaner.c \
       src/coder.c \
       src/compile.c \
       src/dongle.c \
       src/init.c \
       src/logger.c \
       src/main.c \
       src/monitor.c \
       src/parser.c \
       src/routine.c \
       src/scheduler.c \
       src/timer.c

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re