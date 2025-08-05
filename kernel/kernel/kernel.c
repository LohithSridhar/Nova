#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <kernel/kernel_init.h>

void kernel_main(void) {
	kernel_init();
	printf("Welcome to NovaOS Version 0.1.7!\n\n");
	
	printf("Would you like to play a game? ");

	char temp[100];
	scanf("%s", temp);
	printf("Either way, you're playing!\n");
	bool won = false;

	int secret, guess, attempts = 0;
	const int max_attempts = 7;

	int *guesses = (int *)malloc(max_attempts * sizeof(int));
	if (!guesses) {
		printf("Failed to allocate memory for guesses. Continuing without record.\n");
	}

	// Make a random number
	secret = get_rand() + 1; // get_rand is irq_0 based, and is also conveniently mod 100!
	
	printf("Welcome to the Number Guesser Game! I have a number between 1 and 100. Guess it in %d attempts!\n", max_attempts);

	while (attempts < max_attempts) {
		printf("Attempt #%d: ", attempts + 1);

		if (scanf("%d", &guess) != 1) {
			printf("It's alright; it isn't the first time you forgot how to count!\n");
			continue;
		}

		if (guesses) {
			guesses[attempts] = guess;
		}

		attempts++;

		if (guess == secret) {
			printf("Congratulations! You guessed the correct number, %d, in %d attempts.\n", secret, attempts);
			won = true;
			break;
		}

		if (attempts >= max_attempts) {
			break;
		} else {
			if (guess < secret) {
				printf("Too low! Try again.\n");
			} else {
				printf("Too high! Try again.\n");
			}
		}
	}

	if (!won) printf("Aww, you've failed. The correct answer was %d.\n", secret);

	// If guesses were recorded, print them
	if (guesses) {
		printf("Your guesses were:");
		for (int i = 0; i < attempts - 1; i++) {
			printf(" %d,", guesses[i]);
		}
		printf(" and %d.\n", guesses[attempts - 1]);
		free(guesses);
	}

	abort(won ? "Congratulations, you won!" : "You failed.");

	return;
}