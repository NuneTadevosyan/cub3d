NAME			= cub3d
SRCS			= $(wildcard *.c) 
OBJS			= ${SRCS:.c=.o}
MLX 			= minilibx

CC				= gcc
CFLAGS 			= -Wall -Wextra -Werror
LFLAGS			= -L. -lmlx
METAL_MLX		= libmlx.dylib -framework OpenGL -framework AppKit -o $(NAME)
RM				= rm -f



$(NAME):		$(OBJS)
				@make -s -C $(MLX)
				mv minilibx/libmlx.dylib  libmlx.dylib
				@$(CC) $(CFLAGS) $(LFLAGS)  $(METAL_MLX) $(OBJS) -o $(NAME)

all:
				$(NAME)

%.o: %.c
				@$(CC) $(CFLAGS)  -o $@ -c $<

clean:
				@$(RM) $(OBJS)
				@make clean -C $(MLX)

fclean:			clean
				@$(RM) $(NAME)
				@$(RM) libmlx.dylib
				@$(RM) img.bmp

re:				fclean all

.PHONY: 		all fclean clean re
