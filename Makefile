base_dir=$(abspath .)

default: generator main

generator: generator.cpp
	g++ -std=c++11 -o generator $<

main: main.cpp
	g++ -Wall -Werror -Wextra -std=c++11 $< -o main -O2

clear:
	rm -rf $(base_dir)/generator $(base_dir)/main 