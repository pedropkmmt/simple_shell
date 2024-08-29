#include "shell.h"

/**
 * main - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
#include "simple_shell.h"

int main(void)
{
    char *command;

    while (1) {
        display_prompt();
        command = read_command();

        if (command == NULL) { // Handle EOF (Ctrl+D)
            printf("\n");
            break;
        }

        if (strlen(command) > 0) {
            execute_command(command);
        }

        free(command);
    }

    return 0;
}

void display_prompt(void)
{
    printf("$ ");
    fflush(stdout);
}

char *read_command(void)
{
    char *buffer = malloc(BUFFER_SIZE * sizeof(char));
    if (!buffer) {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }

    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        free(buffer);
        return NULL; // Return NULL on EOF
    }

    // Remove trailing newline
    buffer[strcspn(buffer, "\n")] = 0;

    return buffer;
}

void execute_command(char *command)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        char *argv[] = {command, NULL};
        if (execve(command, argv, NULL) == -1) {
            perror("Error");
            exit(EXIT_FAILURE);
        }
    } else { // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}
