#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <kernel/kernel_init.h>
#include <sys/osname.h>

extern void wait(void);

int malloc_test(void) {
	printf("Welcome to the heap tester!\n");
	printf("Testing malloc and free...\n");
	printf("Note: go into debug mode to see the heap's state.\n");
	printf("Currently, the heap is in the initial state.\n");
	char temp[2] = ".";
	scanf("%s", temp);

	// Allocate 32 bytes
	char *p = (char *)malloc(32);
	if (!p) {
		printf("Initial malloc(32) failed.\n");
		return 1;
	}
	printf("malloc(32) returned address %p\n", p);
	scanf("%s", temp);

	// Write data into allocated memory
	strcpy(p, "Hello malloc/free tester world!");
	printf("Written string: %s\n", p);
	scanf("%s", temp);

	// Allocate another block
	int *ip = (int *)malloc(sizeof(int) * 10);
	if (!ip) {
		printf("Second malloc(10 * int) failed.\n");
		free(p);
		return 1;
	}
	printf("malloc(10 * int) returned address %p\n", ip);
	scanf("%s", temp);

	// Initialize array
	for (int i = 0; i < 10; i++) {
		ip[i] = i * i;
	}

	printf("integer array contents:\n");
	for (int i = 0; i < 10; i++) {
		printf("%d ", ip[i]);
	}
	printf("\n");
	scanf("%s", temp);

	// Free first block
	free(p);
	printf("Freed the first block (of string)\n");
	scanf("%s", temp);

	// Allocate a new block of 64 bytes
	char *q = (char *)malloc(64);
	if (!q) {
		printf("Third malloc(64) failed.\n");
		free(ip);
		return 1;
	}
	printf("malloc(64) returned address %p\n", q);
	scanf("%s", temp);

	free(ip);
	free(q);
	printf("Freed all remaining blocks!\n");
	scanf("%s", temp);

	printf("malloc/free test completed successfully. Exiting...\n");
	return 0;
}

void kernel_main(void) {
	// Standard initalisation and basic bootsplash here.
	kernel_init();
	printf("\x1B[90;40mWelcome to %s\n", OS_FULL_NAME);
	printf("To enter debug mode, press [CTRL]+[OPT/ALT]+[ESC]\x1B[0m\n\n");

	/* Primary kernel code here. */

	//printf(malloc_test() ? "\x1B[91;40mTest failed.\n" : "\x1B[92;40mTest successful!\n");
	
	printf("\x1B[0mMoving on...\nWould you like to play a game? ");

	char temp[100];
	scanf("%s", temp);
	printf("\x1B[35;40mEither way, you're playing!\x1B[0m\n");
	bool won = false;

	int secret, guess, attempts = 0;
	const int max_attempts = 7;

	int *guesses = (int *)malloc(max_attempts * sizeof(int));
	if (!guesses) {
		printf("\x1B[31;40mFailed to allocate memory for guesses. Continuing without record.\n");
	}

	// Make a random number
	random_init();
	secret = rand() % 100 + 1;
	
	printf("Welcome to the Number Guesser Game! I have a number between 1 and 100. Guess it in %d attempts!\n", max_attempts);
	printf("Hint: the secret is at memory address \x1B[95;40m%p\x1B[0m. Catch it if you can!\n", &secret);

	while (attempts < max_attempts) {
		printf("\x1B[0mAttempt #%d: \x1B[94;40m", attempts + 1);

		if (scanf("%d", &guess) != 1) {
			printf("\x1B[35;40mIt's alright; it isn't the first time you forgot how to count!\n");
			continue;
		}

		if (guesses) {
			guesses[attempts] = guess;
		}

		attempts++;

		if (guess == secret) {
			printf("\x1B[32;40mCongratulations! \x1B[0mYou guessed the correct number, %d, in %d attempts.\n", secret, attempts);
			won = true;
			break;
		}

		if (attempts >= max_attempts) {
			break;
		} else {
			if (guess < secret) {
				printf("\x1B[35;40mToo low! Try again.\n");
			} else {
				printf("\x1B[36;40mToo high! Try again.\n");
			}
		}
	}

	if (!won) printf("\x1B[91;40mAww, you've failed. The correct answer was %d.\n", secret);

	// If guesses were recorded, print them
	if (guesses) {
		printf("\x1B[0mYour guesses were:");
		for (int i = 0; i < attempts - 1; i++) {
			printf(" %d,", guesses[i]);
		}
		if (attempts == 1) printf(" %d. \x1B[92;40mWow, only one guess?! Very good!\x1B[0m\n", guesses[0]);
		else printf(" and %d.\n", guesses[attempts - 1]);
		free(guesses);
	}

	abort(won ? "Congratulations, you won!" : "You failed.", OS_NAME);

	return;
}

// kernel.c - Our main man! This file is what will eventually contain the main loop that works with the user to form the OS.
// As of now, it simply consists of a mallocation test and a simple number guessing game to check if the OS is operational.
