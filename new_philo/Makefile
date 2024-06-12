# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlaporte <mlaporte@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/30 14:14:24 by mlaporte          #+#    #+#              #
#    Updated: 2024/04/30 14:19:12 by mlaporte         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        := philo

SRC_DIR     := ./
SRCS        := \
			philosophers.c \
			#utils.c \
			#utils2.c \
			#eat.c \
			#init.c \


# Prepend the source directory to each source file
SRCS        := $(SRCS:%=$(SRC_DIR)/%)

BUILD_DIR   := .build
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g

RM          := rm -f
MAKEFLAGS   += --silent --no-print-directory #-j #remove to see relink + flags silence 
DIR_DUP     = mkdir -p $(@D)

all: $(NAME)

$(NAME): $(OBJS) $(LIBS_TARGET)
	$(CC) $(OBJS) -o $(NAME)
	$(info CREATED $(NAME))

$(LIBS_TARGET):
	$(MAKE) -C $(@D)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_DUP)
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEPS)

clean:
	for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f clean; done
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	for f in $(dir $(LIBS_TARGET)); do $(MAKE) -C $$f fclean; done
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

info-%:
	$(MAKE) --dry-run --always-make $* | grep -v "info"

.PHONY: clean fclean re
.SILENT: #remove to see relink + flags silence 

####################################### END ####
