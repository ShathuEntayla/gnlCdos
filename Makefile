# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fhidalgo <fhidalgo@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/06 12:02:17 by fhidalgo          #+#    #+#              #
#    Updated: 2026/11/09 17:28:55 by fhidalgo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gnl.a

DIR_S = ./
SOURCES = \
BONUS = \
	
OBJS = $(SOURCES:.c=.o)
OBJS_BONUS = $(BONUS:.c=.o)

INCLUDES = get_next_line.h
INCLUDES_BONUS = 

CC = cc
MEM_CHECK = valgrind --leak-check=full
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDES) -D BUFFER_SIZE=3 -g

all: $(NAME)

bonus: 

#Para entregar los bonus del proyecto se deberá incluir una regla bonus en el Makefile,
#en la que se añadirán todos los headers, librerías o funciones que estén prohibidas
#en la parte principal del proyecto. Los bonus deben estar en archivos distintos ×
#_bonus.{c/h}. La parte obligatoria y los bonus se evalúan por separado.

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS) 
	rm -f $(OBJS_BONUS)
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

#COMANDOS ADICIONALES#

TESTABLE = \
	$(DIR_S)get_next_line_utils.c\
	$(DIR_S)get_next_line.c\

TEST = $(TESTABLE:.c=.o)

TESTABLE_B = \
	$(DIR_S)get_next_line_utils_bonus.c\
	$(DIR_S)get_next_line_bonus.c\

TEST_B = $(TESTABLE_B:.c=.o)

re_s: $(TEST)
	@ar rcs libft.a $(TEST)
	@cc -g get_next_line.c libft.a
	@echo ""
	./a.out	
	@echo ""
	@echo ""
	@make fclean

re_b: $(TEST_B)
	@ar rcs libft.a $(TEST_B)
	@cc -g get_next_line_bonus.c libft.a
	@echo ""
	./a.out	
	@echo ""
	@echo ""
	@make fclean

nc:
	norminette $(TESTABLE)

ncb:
	norminette $(TESTABLE_B)

nl: 
	norminette *.h

g:
	git add .
	git commit -m "Actualizamos README - $$(date +%H:%M:%S)"
	git push
	git status
	git log -1 --oneline

gls:
	git ls-files

test:
	cd gnl_tester
	make all
