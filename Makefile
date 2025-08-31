BBLACK=\033[1;30m
BRED=\033[1;31m
BGREEN=\033[1;32m
BYELLOW=\033[1;33m
BBLUE=\033[1;34m
BPURPLE=\033[1;35m
RESET_COLOR=\033[0m

NAME = midione
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0
LDFLAGS = libportaudio.a -lrt -lm -lasound -ljack -pthread
HEADER = -I.
LDFLAGS = -lrt -lm -lasound -ljack -pthread

SRCS = main.c \
		parser.c \
		processor.c \
		sequencer.c \
		waveforms.c \
		synth.c \
		mixer.c \
		utils.c

OBJS = $(SRCS:.c=.o)

.SECONDARY: $(OBJS)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "$(BBLUE)Compiled and linked.$(RESET_COLOR)"

clean:
	rm -f $(OBJS)
	@echo "$(BYELLOW)All object files cleaned.$(RESET_COLOR)"

fclean: clean
	rm -f $(NAME)
	@echo "$(BYELLOW)Executable cleaned.$(RESET_COLOR)"

re: fclean all

.PHONY: all clean fclean re
