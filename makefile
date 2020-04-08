all:
	clang++ -Wall -Wextra -Werror -Wpedantic -pedantic-errors -std=c++17 -O3 -s -lsfml-system -lsfml-window -lsfml-graphics main.cpp spin.cpp helpers.cpp -o spin
