# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeguerin <jeguerin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/03 18:59:35 by jeza              #+#    #+#              #
#    Updated: 2024/07/01 18:59:01 by jeguerin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nom de l'executable
NAME = philosophers

# Compilateur
CC := gcc

# Options de compilation
CFLAGS := -Wall -Wextra -Werror -Iheaders/ -pthread -g

#Liste des fichiers source.
SRCS = Sources/main.c Sources/utils.c Sources/manage_args.c \
		Sources/manage_threads.c Sources/count_nb.c \
		Sources/ft_printf.c Sources/ft_putnb.c Sources/manage_time.c \
		# Sources/other_main.c \

OBJS = $(SRCS:.c=.o)

# Regle par defaut pour l'executable
all: author $(NAME)

# Regles pour afficher l'auteur et le projet.
author:
	@echo "Author : jeguerin\n"
	@echo "Project : Philosophers\n"

$(LIBFT):
	make -C Libft

$(PRINTF):
	make -C ft_printf

# Regle pour creer l'executable en liant les objets.
# $(NAME): $(OBJS) $(LIBFT) $(PRINTF) $(GETNEXTLINE_OBJ)
# 	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(PRINTF) $(GETNEXTLINE_OBJ) -o $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Regle pour nettoyer les fichiers objets.
clean:
	$(RM) $(OBJS)

# Regle pour nettoyer les fichiers objets et l'executable.
fclean: clean
	$(RM) $(NAME)

# Regle pour reconstruire le projet (clean, puis all)
re: author fclean all

# Declarer les regles comme des cibles .phony pour eviter des conflits avec
# des fichiers du meme nom.
.PHONY: all clean fclean re
