#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// Envoie un message au serveur bit par bit
void send_signal(int pid, char *message) {
    size_t len = strlen(message);
    
    // Parcours de chaque caractère du message
    for (size_t i = 0; i <= len; i++) {
        char c = message[i];
        // Envoi des 8 bits de chaque caractère
        for (int j = 0; j < 8; j++) {
            if ((c >> j) & 1) {
                kill(pid, SIGUSR2);  // Envoi d'un 1
            } else {
                kill(pid, SIGUSR1);  // Envoi d'un 0
            }
            usleep(100); 
        }
    }
    
    // Signal de fin de transmission
    kill(pid, SIGINT);
}

int main(int argc, char *argv[]) {
    // Vérification des arguments
    if (argc != 3) {
        printf("Usage: %s <server_pid> <message>\n", argv[0]);
        return 1;
    }

    // Conversion et validation du PID
    int server_pid = atoi(argv[1]);
    if (server_pid <= 0) {
        printf("Invalid server PID\n");
        return 1;
    }

    // Envoi du message
    send_signal(server_pid, argv[2]);
    printf("Sending to %d.\nDone.\n", server_pid);
    
    return 0;
}