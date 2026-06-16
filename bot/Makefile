NAME=bot

OBJ_DIR=./.obj
MY_SOURCES= sources/main.cpp \
			sources/Bot.cpp \
			sources/TicTacToe.cpp \
			sources/Minimax.cpp \
			sources/BotCommands.cpp \

MY_SOURCES+=utils/utils.cpp


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
	$(CYAN) ______     ______     ______  
	$(CYAN)/\\  == \\   /\\  __ \\   /\\__  _\\ 
	$(CYAN)\\ \\  __<   \\ \\ \\/\\ \\  \\/_/\\ \\/ 
	$(CYAN) \\ \\_____\\  \\ \\_____\\    \\ \\_\\ 
	$(CYAN)  \\/_____/   \\/_____/     \\/_/
	$(CYAN)
endef
export HEADER

# ====================
#       COLORS
# ====================

RESET=\033[0m
GREEN=\033[0;32m
CYAN=\033[0;36m

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
	@$(RM) -r $(OBJ_DIR)
	@$(RM) $(NAME)
	@$(RM) $(NAME)
	@clear
	@echo "\n$(CYAN)   ╔════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)      All cleaned up! Ready to build fresh! $(RESET)"
	@echo "$(CYAN)   ╚════════════════════════════════════════╝$(RESET)"

re: fclean all

.PHONY: all clean fclean re