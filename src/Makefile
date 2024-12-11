#================================== COMPILER ==================================#
CC = gcc
CPP = g++
#==============================================================================#

#==================================== FLAGS ===================================#
CFLAGS = -Wall -Wextra -Werror -g --std=gnu11
CPPFLAGS = -Wall -Wextra -Werror -g --std=c++17
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	GFLAGS = -lncurses -lgtest -lstdc++
endif

ifeq ($(UNAME), Darwin)
	GFLAGS =  -I/opt/homebrew/include -L/opt/homebrew/lib -lncurses -lgtest
endif
#==============================================================================#

#================================ TARGET NAMES ================================#
CLANG_FILE = ../materials/linters/.clang-format
CLI_TARGET = cli_launcher
TEST_EXEC = test_launcher
#==============================================================================#

#=============================== DIRECTORY NAMES ==============================#
BUILD_DIR = build
BUILD_DIR_CLI = build/cli
BUILD_DIR_DESKTOP = build/desktop
SNAKE_DIR = brick_game/snake
TETRIS_DIR = brick_game/tetris
CLI_DIR = gui/cli
DESKTOP_DIR = gui/desktop
COMMON_DIR = brick_game/common
TEST_DIR = tests
DVI_DIR = docs
DIST_DIR = dist/brick_game_v2.0
#==============================================================================#

#====================== LIST OF FILE AND DIRS IN MODULES ======================#
TETRIS_SRC = $(wildcard $(TETRIS_DIR)/*.c)
SNAKE_SRC = $(wildcard $(SNAKE_DIR)/*.cpp)
CLI_SRC_C = $(wildcard $(CLI_DIR)/*.c)
CLI_SRC_CPP = $(wildcard $(CLI_DIR)/*.cpp)
SOURCE_TEST = $(wildcard $(TEST_DIR)/*.cpp)

TETRIS_OBJS = $(patsubst $(TETRIS_DIR)/%.c, $(BUILD_DIR_CLI)/%.o, $(TETRIS_SRC))
SNAKE_OBJS = $(patsubst $(SNAKE_DIR)/%.cpp, $(BUILD_DIR_CLI)/%.o, $(SNAKE_SRC))
CLI_SRC_C_OBJ = $(patsubst $(CLI_DIR)/%.c, $(BUILD_DIR_CLI)/%.o, $(CLI_SRC_C))
CLI_SRC_CPP_OBJ = $(patsubst $(CLI_DIR)/%.cpp, $(BUILD_DIR_CLI)/%.o, $(CLI_SRC_CPP))

CLI_C_OBJ = $(TETRIS_OBJS) $(CLI_SRC_C_OBJ)
CLI_CPP_OBJ = $(SNAKE_OBJS) $(CLI_SRC_CPP_OBJ)
#==============================================================================#

#================================= MAIN TARGETS ===============================#
.PHONY: all install uninstall cli desktop launch_cli test gcov_report clang_i clang_n cppcheck valgrind_cli valgrind_test rebuild clean dvi dist

all: gcov_report install

install: uninstall cli desktop

uninstall:
	@rm -rf build

cli: $(BUILD_DIR) $(BUILD_DIR_CLI) $(CLI_C_OBJ) $(CLI_CPP_OBJ)
	$(CPP) $(CPPFLAGS) $(CLI_CPP_OBJ) $(CLI_C_OBJ) $(GFLAGS) -o $(BUILD_DIR_CLI)/$(CLI_TARGET)
	@rm $(CLI_C_OBJ) $(CLI_CPP_OBJ)
	@echo "cli mode has been built in build/cli"
$(BUILD_DIR_CLI)/%.o: $(TETRIS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR_CLI)/%.o: $(CLI_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR_CLI)/%.o: $(SNAKE_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@
$(BUILD_DIR_CLI)/%.o: $(CLI_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

desktop: $(BUILD_DIR) $(BUILD_DIR_DESKTOP)
	@qmake $(DESKTOP_DIR)/brickgame.pro -o $(BUILD_DIR_DESKTOP)
	@make -C $(BUILD_DIR_DESKTOP)

launch_cli:
	@if [ -f $(BUILD_DIR_CLI)/$(CLI_TARGET) ]; then \
		$(BUILD_DIR_CLI)/$(CLI_TARGET); \
	else \
		$(MAKE) cli; \
		$(BUILD_DIR_CLI)/$(CLI_TARGET); \
	fi

test:
	$(CPP) $(CPPFLAGS) $(SOURCE_TEST) $(SNAKE_DIR)/fsm.cpp -o $(TEST_EXEC) $(GFLAGS)
	./$(TEST_EXEC)
	@rm $(TEST_EXEC)

gcov_report: clean
	@echo "generating gcov report..." 
	@$(CPP) $(CPPFLAGS) $(SOURCE_TEST) $(SNAKE_DIR)/fsm.cpp -o $(TEST_EXEC) $(GFLAGS) --coverage 
	@./$(TEST_EXEC)
	@lcov --no-external -t "$(TEST_EXEC)" -o $(TEST_EXEC).info -c -d . \
			--exclude "$(PWD)/tests/*" \
			--exclude "$(PWD)/brick_game/tetris/*" \
			--exclude "$(PWD)/brick_game/snake/controller.cpp" \
			--exclude "$(PWD)/gui/*"
	@genhtml -o report $(TEST_EXEC).info
	@rm -rf *.gcno *.gcda *.gcov 

clang_i:
	@cp $(CLANG_FILE) .
	clang-format -i $(SNAKE_DIR)/*.cpp \
									$(SNAKE_DIR)/*.hpp \
									$(TETRIS_DIR)/*.c \
									$(TETRIS_DIR)/*.h \
									$(CLI_DIR)/*.c \
									$(CLI_DIR)/*.h \
									$(CLI_DIR)/*.cpp \
									$(DESKTOP_DIR)/*.cpp \
									$(COMMON_DIR)/*h \
									$(TEST_DIR)/*.cpp \
									$(TEST_DIR)/*.hpp
	@rm .clang-format

clang_n:
	@cp $(CLANG_FILE) .
	clang-format -n $(SNAKE_DIR)/*.cpp \
									$(SNAKE_DIR)/*.hpp \
									$(TETRIS_DIR)/*.c \
									$(TETRIS_DIR)/*.h \
									$(CLI_DIR)/*.c \
									$(CLI_DIR)/*.h \
									$(CLI_DIR)/*.cpp \
									$(DESKTOP_DIR)/*.cpp \
									$(COMMON_DIR)/*h \
									$(TEST_DIR)/*.cpp \
									$(TEST_DIR)/*.hpp
	@rm .clang-format

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --check-level=exhaustive $(SNAKE_DIR)/* $(TETRIS_DIR)/* $(CLI_DIR)/* $(DESKTOP_DIR)/* $(COMMON_DIR)/*

valgrind_cli:
	@if [ -f $(BUILD_DIR_CLI)/$(CLI_TARGET) ]; then \
		valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt $(BUILD_DIR_CLI)/$(CLI_TARGET); \
	else \
		$(MAKE) cli; \
		valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt $(BUILD_DIR_CLI)/$(CLI_TARGET); \
	fi
	@cat valgrind_out.txt
	@rm test valgrind_out.txt

valgrind_test:
	$(CPP) $(CPPFLAGS) $(SOURCE_TEST) $(SNAKE_DIR)/fsm.cpp -o $(TEST_EXEC) $(GFLAGS)
	valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt ./$(TEST_EXEC)
	@cat valgrind_out.txt
	@rm test valgrind_out.txt

rebuild: uninstall install

clean:
	@rm -rf build snake_max_score.txt tetris_max_score.txt *.gcno *.gcda *.info report $(TEST_EXEC) test_launcher.dSYM dist $(TEST_EXEC)
	@find ./docs -mindepth 1 -maxdepth 1 | grep -v './docs/snake_fsm_scheme.jpg' | xargs rm -rf
	@echo "cleaning..."

dvi:
	@find ./docs -mindepth 1 -maxdepth 1 | grep -v './docs/snake_fsm_scheme.jpg' | xargs rm -rf
	doxygen Doxyfile
ifeq ($(UNAME), Linux)
	xdg-open $(DVI_DIR)/html/index.html
endif
ifeq ($(UNAME), Darwin)
	open $(DVI_DIR)/html/index.html
endif

dist: clean
	@rm -rf $(DIST_DIR)
	@echo "Creating a distribution..."
	@mkdir -p $(DIST_DIR)
	@echo "Distribution packaging..."
	@tar --exclude='./tests' --exclude='./dist' -czf $(DIST_DIR)/brick_game_v2.0.tar.gz -C ./ .
	@echo "Distribution created"
	
#==============================================================================#

#============================== CREATE DIRECTORIES ============================#
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR_CLI):
	mkdir -p $(BUILD_DIR_CLI)
$(BUILD_DIR_DESKTOP):
	mkdir -p $(BUILD_DIR_DESKTOP)
#==============================================================================#