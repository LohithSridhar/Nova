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
	printf("I don't care.\n");
	int secret, guess, attempts = 0;
	const int max_attempts = 7;

	// Make a random number
	secret = get_rand() + 1; // get_rand is irq_0 based, and is also conveniently mod 100!
	
	printf("Welcome to the Number Guesser Game! I have a number between 1 and 100. Guess it in %d attempts!\n", max_attempts);

	while (attempts < max_attempts) {
		printf("Attempt #%d: ", attempts + 1);

		if (scanf("%d", &guess) != 1) {
			printf("It's alright; it isn't the first time you forgot how to count!\n");
			continue;
		}

		attempts++;

		if (guess < secret && attempts < max_attempts) {
			printf("Too low! Try again in ");
		} else if (guess > secret && attempts < max_attempts) {
			printf("Too high! Try again in ");
		} else {
			printf("Congratulations! You guessed the correct number, %d, in %d attempts.\n", secret, attempts);
			abort("Congratulations, You Won!");
		}
	}

	printf("Aww, you've failed. The correct answer was %d.\n", secret);
	abort("You failed.");
}