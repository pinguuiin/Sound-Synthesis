BBLACK=\033[1;30m
BRED=\033[1;31m
BGREEN=\033[1;32m
BYELLOW=\033[1;33m
BBLUE=\033[1;34m
BPURPLE=\033[1;35m
RESET_COLOR=\033[0m

NAME = minisynth
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0
LDFLAGS = -L.
LDLIBS = -lm -pthread -lasound -lportaudio
HEADER = -I./include

SRC_DIR = src
SRCS = main.c \
		parser.c \
		parser_utils.c \
		processor.c \
		sequencer.c \
		waveforms.c \
		synth.c \
		mixer.c

OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

.SECONDARY: $(OBJS)

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME) $(LDLIBS)
	@echo "$(BBLUE)Compiled and linked.$(RESET_COLOR)"

clean:
	rm -rf $(OBJ_DIR)
	@echo "$(BYELLOW)All object files cleaned.$(RESET_COLOR)"

fclean: clean
	rm -f $(NAME)
	@echo "$(BYELLOW)Executable cleaned.$(RESET_COLOR)"

re: fclean all

.PHONY: all clean fclean re
