OBJDIR := objs
OBJLIST := \
	main.o \
	keyboard.o \
	map.o \
	parse_lvl.o
LIBFTDIR := libft
#MLXDIR := minilibx_macos
MLXDIR := minilibx
MLX_GRAPHICSDIR := mlx_graphics
MLX_IMGUIDIR := mlx_imgui
OBJS := $(addprefix $(OBJDIR)/,$(OBJLIST))
SRCDIR := srcs
INCDIR := -Iincludes -I$(LIBFTDIR)/includes -I$(MLXDIR) -I$(MLX_GRAPHICSDIR)/includes -I$(MLX_IMGUIDIR)/includes -I/opt/X11/include/
LIBDIR := -L$(LIBFTDIR) -L$(MLXDIR) -L$(MLX_GRAPHICSDIR) -L$(MLX_IMGUIDIR)
LIBOPT := -lXext -lX11 -lm -lmlx_imgui -lmlx_graphics -lmlx -lft
#LIBOPT := -lft -lm -lmlx -framework OpenGL -framework AppKit
LIB    := $(LIBDIR) $(LIBOPT)
CFLAGS := -Wall -Wextra -g $(INCDIR)
NAME := fdf

CC := clang

DEPDIR := .d
DEPFLAGS = -MMD -MP -MF $(DEPDIR)/$*.Td
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS)
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

.PHONY : all clean clean_lft fclean fclean_lft binclean binclean_lft re re_lft clean_mlx re_mlx

all : | $(LIBFTDIR)/libft.a $(MLXDIR)/libmlx.a $(MLX_GRAPHICSDIR)/libmlx_graphics.a $(NAME)
	
$(NAME) : $(OBJS)
	@echo Binary
	$(CC) $^ $(CFLAGS) $(LIB) -o $(NAME)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo Objects
	$(CC) $(DEPFLAGS) $(CFLAGS) -o $@ -c $<
	$(POSTCOMPILE)

$(OBJS): | $(OBJDIR) $(DEPDIR)

$(DEPDIR):
	mkdir -p $(DEPDIR) >/dev/null

$(OBJDIR):
	mkdir $(OBJDIR)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

$(LIBFTDIR)/libft.a:
	make -C $(dir $@)

$(MLXDIR)/libmlx.a:
	make -C $(dir $@)

$(MLX_GRAPHICSDIR)/libmlx_graphics.a:
	make -C $(dir $@)

clean :
	rm -rf $(OBJDIR)
	rm -rf $(DEPDIR)

clean_lft :
	make -C $(LIBFTDIR) clean

clean_mlx :
	make -C $(MLXDIR) clean

binclean:
	rm -f $(NAME)

binclean_lft:
	make -C $(LIBFTDIR) binclean

fclean : clean binclean
fclean_lft : clean_lft binclean_lft

re : | fclean all

re_lft : | fclean_lft re

re_mlx : | clean_mlx re

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(OBJLIST)))
