// Write a C program that uses two pipes and two child processes to form a 3-stage pipeline:
// stdin → [Process A] → pipe1 → [Process B] → pipe2 → [Process C] → stdout
// Where:
// Process A (parent) — reads from stdin and writes to pipe1
// Process B (child 1) — reads from pipe1, removes all digits (0–9) from the text, writes to pipe2
// Process C (child 2) — reads from pipe2, reverses each line, writes to stdout

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#define BUFFER_SIZE 256

void reverseLine(char *buffer, int n) {
    int line_start = 0;
    
    for (int i = 0; i <= n; i++) {
        if (i == n || buffer[i] == '\n') {
            int start = line_start;
            int end = i - 1;
            while (start < end) {
                char tmp = buffer[start];
                buffer[start] = buffer[end];
                buffer[end] = tmp;
                start++;
                end--;
            }
            line_start = i + 1;
        }
    }
}

int main() {
	pid_t pid1, pid2;
	int fd1[2], fd2[2];
	char buf [BUFFER_SIZE];
	ssize_t n;

	if (pipe(fd1) < 0 || pipe(fd2) < 0) {
		perror("Pipe creation");
		exit (EXIT_FAILURE);
	}

	pid1 = fork();
	if (pid1 < 0) {
		perror("Forking");
		exit (EXIT_FAILURE);
	}

	if (pid1 != 0) {
		close (fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
		while ((n = read (STDIN_FILENO, buf, BUFFER_SIZE)) > 0) {
			write(fd1[1], buf, n);
		}
		close (fd1[1]);
		wait (NULL);
	} else {
		pid2 = fork();
		if (pid2 != 0 ){
			close (fd1[1]);
			close (fd2[0]);
			while ((n = read (fd1[0], buf, BUFFER_SIZE)) > 0 ) {
				int j = 0;
				for (int i = 0; i < n; i++) {
    					if (!isdigit((unsigned char)buf[i]))
        				buf[j++] = buf[i];
				}
				write(fd2[1], buf, j);
			}
			close (fd1[0]);
			close (fd2[1]);
			wait (NULL);
		} else {
            close(fd1[0]);
            close(fd1[1]);
			close (fd2[1]);
			while ((n = read (fd2[0], buf, BUFFER_SIZE)) > 0) {
				reverseLine(buf, n);
                if(write(STDOUT_FILENO, buf, n) != n){
                    perror("Writing to STDOUT");
                    exit(EXIT_FAILURE);
                }
			}
			close (fd2[0]);
            exit(0);
		}
	}
	return 0;
}
