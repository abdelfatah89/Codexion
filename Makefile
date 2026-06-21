NAME = codexion
SRCS = src/cleaner.c \
       src/coder.c \
       src/compile.c \
       src/dongle.c \
       src/heap.c \
       src/heap_tools.c \
       src/init.c \
       src/logger.c \
       src/monitor.c \
       src/parser.c \
       src/routine.c \
       src/timer.c \
       main.c

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread 
RM = rm -f
NORMINETTE = norminette

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

run: all clean

lint:
	$(NORMINETTE) .

re: fclean all

.PHONY: all clean fclean re run