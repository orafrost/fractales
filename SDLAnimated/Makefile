##
## Makefile for gui in/home/orafrost/perso/gui
##
## Made by guillame verrier
## Login   <orafrost@epitech.net>
##
## Started on	Wed Nov  2 16:26:18 2016 guillame verrier
## Last update	Wed Nov  2 16:26:18 2016 guillame verrier
##

NAME =	gui

SRC =	./main.c \

OBJ =	$(SRC:.c=.o)

CFLAGS =-I./include\
	-I/home/${USER}/.froot/include \

LIBFLAG= -lSDL2 -lm

all:	$(NAME)

$(NAME):$(OBJ)
	cc -o $(NAME) $(OBJ) $(CFLAGS) $(LIBFLAG)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
