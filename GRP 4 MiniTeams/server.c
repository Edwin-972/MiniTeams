#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

char received_message[1024];
int message_index = 0;
int client_pid = 0;

void sigusr1_handler(int sig, siginfo_t *info, void *context) {
    (void)sig;       
    (void)context;   
    received_message[message_index] = '0';
    message_index++;
    client_pid = info->si_pid; // Récupérer le PID du client
}

void sigusr2_handler(int sig, siginfo_t *info, void *context) {
    (void)sig;       
    (void)context;   
    received_message[message_index] = '1';
    message_index++;
    client_pid = info->si_pid; // Récupérer le PID du client
}

void end_of_transmission_handler(int sig) {
    (void)sig;      
    received_message[message_index] = '\0';

    // Reconstruire le message à partir des bits reçus
    char message[128] = {0};
    for (int i = 0; i < message_index; i += 8) {
        char byte = 0;
        for (int j = 0; j < 8; j++) {
            byte |= (received_message[i + j] - '0') << j;
        }
        message[i / 8] = byte;
    }

    // Afficher le message avec le PID du client
    printf("[From client %d] %s\n", client_pid, message);

    // Enregistrer le message dans le fichier conversations.log 
    FILE *log_file = fopen("conversations.log", "a");
    if (log_file) {
        time_t now = time(NULL);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "[%H:%M:%S]", localtime(&now));
        fprintf(log_file, "%s [From client %d] %s\n", timestamp, client_pid, message);
        fclose(log_file);
    }

    message_index = 0; // Réinitialiser pour le prochain message
}

int main() {
    printf("Miniteams starting...\n");
    printf("My PID is %d\n", getpid());
    printf("Waiting for new messages\n");


    struct sigaction sa1, sa2;
    sa1.sa_sigaction = sigusr1_handler;
    sa1.sa_flags = SA_SIGINFO;
    sa2.sa_sigaction = sigusr2_handler;
    sa2.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &sa1, NULL);
    sigaction(SIGUSR2, &sa2, NULL);
    signal(SIGINT, end_of_transmission_handler);

    while (1) {
        pause(); // Attente des signaux
    }

    return 0;
}