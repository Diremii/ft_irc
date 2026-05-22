NAME=ircserv

OBJ_DIR=./.obj
MY_SOURCES= sources/main.cpp \
			sources/Server.cpp \

MY_OBJECTS=$(MY_SOURCES:%.cpp=$(OBJ_DIR)/%.o)

# ====================
#      COMMANDS
# ====================
RM=rm -f
CC=c++
STANDARD_FLAGS=-Wall -Werror -Wextra -g -std=c++98 -I./includes

# ====================
#        HEADER
# ====================
define HEADER
	$(CYAN) _____     __     ______     ______     __    __
	$(CYAN)/\  __-.  /\ \   /\  == \   /\  ___\   /\ "-./  \\
	$(CYAN)\ \ \/\ \ \ \ \  \ \  __<   \ \  __\   \ \ \-./\ \\
	$(CYAN) \ \____-  \ \_\  \ \_\ \_\  \ \_____\  \ \_\ \ \_\\
	$(CYAN)  \/____/   \/_/   \/_/ /_/   \/_____/   \/_/  \/_/\
	$(CYAN)
endef
export HEADER

# ====================
#     COLORS
# ====================
RESET=\033[0m
GREEN=\033[0;32m
CYAN=\033[0;36m
BLUE=\033[0;34m
YELLOW=\033[0;33m
RED=\033[0;31m

# ====================
#     COMPILATION
# ====================
all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@clear
	@echo "$(CYAN)Compiling $<...$(RESET)"
	@mkdir -p $(dir $@)
	@$(CC) $(STANDARD_FLAGS) -c $< -o $@
	@$(eval COMPILED_FILES += "$< $(GREEN)🗹\n$(RESET)")
	@clear
	@echo "\n$$HEADER"

$(NAME): $(MY_OBJECTS)
	@$(CC) $(STANDARD_FLAGS) $(MY_OBJECTS) -o $(NAME)
	@echo "\n$(GREEN)All files have been successfully compiled!$(RESET)"
	@echo "$(CYAN)Compiled files:$(RESET)"
	@echo " "$(COMPILED_FILES)

clean:
	@$(RM) $(MY_OBJECTS)
	@clear
	@echo "\n$(CYAN)   ╔════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)     All objects have been deleted! $(RESET)"
	@echo "$(CYAN)   ╚════════════════════════════════╝$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@clear
	@echo "\n$(CYAN)   ╔════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)      All cleaned up! Ready to build fresh! $(RESET)"
	@echo "$(CYAN)   ╚════════════════════════════════════════╝$(RESET)"

re: fclean all

.PHONY: all clean fclean re