# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kpineda- <kpineda-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/02 21:53:56 by angnavar          #+#    #+#              #
#    Updated: 2025/08/12 09:27:43 by kpineda-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

# Directorios principales
SRC_DIR = src
INCLUDE_DIR = includes
LIBFT_DIR = includes/libft
MINILIBX_DIR = includes/minilibx-linux

LIBFT = $(LIBFT_DIR)/libft.a
MINILIBX = $(MINILIBX_DIR)/libmlx.a

SRC = src/gnl_utils.c src/gnl.c src/utils.c src/main.c src/render.c src/parse_file.c src/exit.c src/parse_map.c
OBJ_DIR = obj
OBJ = $(SRC:.c=.o)
OBJ := $(addprefix $(OBJ_DIR)/, $(OBJ))

# Compilador y flags
CC = cc
RM = rm -f
RMDIR = rm -rf
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I$(MINILIBX_DIR)
MLX_FLAGS = -lX11 -lXext -lm

# ==================================== BANNER =================================== #

define	banner
		@echo "\033[0;34m"
		@echo "											 "
		@echo " ██████╗██╗   ██╗██████╗ ██████╗ ██████╗  "
		@echo "██╔════╝██║   ██║██╔══██╗╚════██╗██╔══██╗ "
		@echo "██║     ██║   ██║██████╔╝ █████╔╝██║  ██║ "
		@echo "██║     ██║   ██║██╔══██╗ ╚═══██╗██║  ██║ "
		@echo "╚██████╗╚██████╔╝██████╔╝██████╔╝██████╔╝ "
 		@echo " ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝  "
		@echo "\033[0m"
endef

# =================================== MAIN TARGET =============================== #
all: $(NAME)

$(NAME): $(LIBFT) $(MINILIBX) $(OBJ)
	$(banner)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MINILIBX) -o $(NAME) $(MLX_FLAGS)
	@echo "\033[0;32m▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄ Compilación completada. ▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄\033[0m"

# ============================== COMPILACIÓN LIBRERÍAS ========================== #

$(LIBFT):
	@echo "📦 Compilando Libft..."
	@$(MAKE) -s -C $(LIBFT_DIR) --no-print-directory

$(MINILIBX):
	@echo "🚑  Compilando libmlx..."
	@$(MAKE) -s -C $(MINILIBX_DIR) --no-print-directory

# =========================== COMPILACIÓN DE ARCHIVOS ========================== #

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "\033[0;34m⚙️  Compiling $<...\033[0m"
	@$(CC) $(CFLAGS)  -c $< -o $@

# ================================ REGLAS DE LIMPIEZA =========================== #

clean:
	@echo "🧹 Limpiando archivos objeto..."
	@$(MAKE) -s -C $(LIBFT_DIR) clean --no-print-directory
	@$(MAKE) -s -C $(MINILIBX_DIR) clean --no-print-directory
	@$(RMDIR) $(OBJ_DIR)

fclean: clean
	@echo "🗑️  Eliminando ejecutable..."
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)
	@$(RM) $(NAME)_asan

re: fclean all

# ============================== EXECUTION RULE ================================= #

run: all
	@echo "\033[0;36m🚀 Running program...\033[0m"
	@./$(NAME)

# ====================== ADDRESSSANITIZER TARGETS ============================== #

$(NAME)_asan: $(LIBFT) $(MINILIBX) $(OBJ)
	$(banner)
	@echo "\033[0;33m🧪 Linking with AddressSanitizer...\033[0m"
	@$(CC) $(CFLAGS) -g -fsanitize=address $(CFLAGS) $(OBJ) $(LIBFT) $(MINILIBX) -o $(NAME)_asan $(MLX_FLAGS)
	@echo "\033[0;32m▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄ ASan build ready ▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄\033[0m"

asan: $(NAME)_asan

runasan: $(NAME)_asan
	@echo "\033[0;36m🚀 Running with ASan...\033[0m"
	@./$(NAME)_asan

# ============================== REGLAS ESPECIALES ============================== #

.PHONY: all clean fclean re