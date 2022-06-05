
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*        DO NOT MODIFY THIS FILE!                             */
/*        You should only modify the util.c file!              */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "util.h"

enum ARGS {
	X1 = 0,
	Y1,
	X2,
	Y2,
	WORD,
	NR_ARGS
};

int main(int argc, char **argv)
{
	int sum = 0, area = 0;
	int sub = 0;
	int x1, x2, y1, y2;
	char word[1024];
	char line[1024] = {0};
	FILE *input;
	int i;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s input_file\n", argv[0]);
		fprintf(stderr, "Example: %s sample_input/example1\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Read input file */
	input = fopen(argv[1], "r");
	if (input == NULL) {
		perror("ERROR");
		exit(EXIT_FAILURE);
	}
	
	/* Parse arguments: point (x1, y1), (x2, y2) and word */
	while (fgets(line, sizeof(line), input) != NULL) {
		char *temp;
		char opt[1024] = {0};
		temp = strtok(line, ",");
		i = X1;
		for (i = X1; temp != NULL || i < NR_ARGS; i++, temp = strtok(NULL, ",")) {
			if (i == X1) 
				sscanf(temp, "%d", &x1);
			else if (i == Y1)
				sscanf(temp, "%d", &y1);
			else if (i == X2)
				sscanf(temp, "%d", &x2);
			else if (i == Y2)
				sscanf(temp, "%d", &y2);
			else if (i == WORD) {
				strcpy(word, temp);
				word[strlen(word)-1] = '\0'; 
			} else
				break;
		}
	}

	printf("x1: %d, y1: %d, x2: %d, y2: %d, word:%s\n", x1, y1, x2, y2, word);

	struct Point_val P1 = { .x = x1, .y = y1 };
	struct Point_ref P2 = { .x = &x2, .y = &y2 };

	// You have to allocate memory for pointer members of "struct Point_ref"
	struct Point_ref P1_ref = { .x = NULL, .y = NULL };
	struct Point_val P2_val = { .x = 0, .y = 0 };

	// Problem 1. Calculate the sum of x1 and x2 via "Call-by-Value"
	sum = sum_x(x1, x2);
	printf("sum_x: %d\n", sum);

	// Problem 2. Calculate the subtract of y1 and y2 via "Call-by-Reference"
	sub_y(y1, y2, &sub);
	printf("sub_y: %d\n", sub);

	// Problem 3. Convert variable "struct Point_ref" to "struct Point_val"
	Point_ref_to_Point_val(&P2, &P2_val);

	// Problem 4. Calculate Area 
	area = calc_area1(&P1, &P2_val);
	printf("calc_area1: %d\n", area);

	// Problem 5. Convert variable "struct Point_val" to "struct Point_ref"
	Point_val_to_Point_ref(&P1, &P1_ref);

	// Problem 6. Calculate Area 
	calc_area2(&P1_ref, &P2, &area);
	printf("calc_area2: %d\n", area);

	// Problem 7. Reverse a word 
	printf("The reverse is %s\n", reverse(word));

	free(P1_ref.x);
	free(P1_ref.y);

	fclose(input);
	return 0;
}
