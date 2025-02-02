# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c99

# Noms des exécutables
CLIENT_EXEC = client
SERVER_EXEC = server

# Règles de compilation
all: $(CLIENT_EXEC) $(SERVER_EXEC)

$(CLIENT_EXEC): client.c
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) client.c

$(SERVER_EXEC): server.c
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) server.c

# Nettoyage des fichiers générés
clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC)

# Règle par défaut
.PHONY: all clean