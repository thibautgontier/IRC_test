NAME = server

SERVER_FILES =	main.cpp \
				Server.cpp \
				User.cpp

CLIENT_FILES = client.cpp

CC = clang++ -Wall -Wextra -std=c++98

all: $(NAME)

$(NAME):
	@$(CC) $(SERVER_FILES) -o server
	@$(CC) $(CLIENT_FILES) -lpthread -o client

clean:
	@rm -rf $(NAME)
	@rm -rf client

fclean: clean

re: fclean all