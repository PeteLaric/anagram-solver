/*
	ana-solv.c
	A simple anagram solver in C.
	(cc) 2021-01-12 BY Pete Laric
	http://www.PeteLaric.com
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE	1000

int min_partial_word_length = 3; // Bully English class only accepts words of 3+ letters



void convert_to_lowercase(char *word, int word_length)
{
	for (int i = 0; i < word_length; i++)
	{
		word[i] = tolower(word[i]);
	}
}



void sort_word(char *inword, char *outword, int word_length)
{
	strncpy(outword, inword, BUFFER_SIZE);

	// bubble sort

	// single pass
	int swaps_made = 0;
	do
	{
		// diagnostic:
		//printf("%s\n", outword);
		swaps_made = 0;
		for (int i = 0; i < word_length-1; i++)
		{
			if (outword[i] > outword[i+1])
			{
				// swap 'em
				char buf = outword[i];
				outword[i] = outword[i+1];
				outword[i+1] = buf;
				swaps_made++;
			}
		}
	} while (swaps_made > 0);
}



// returns 1 if small word can be made from big word letters; 0 otherwise
int can_be_made_from(char *big_word, int big_word_length, char *small_word, int small_word_length)
{
	char bwc[BUFFER_SIZE];
	char *big_word_copy = bwc;
	strncpy(big_word_copy, big_word, big_word_length);

	// iterate through each letter in small word
	for (int i = 0; i < small_word_length; i++)
	{
		// iterate through each letter in large word
		// try to find an instance of the small word letter
		int letter_found = 0;
		for (int j = 0; j < big_word_length && !letter_found; j++)
		{
			if (small_word[i] == big_word_copy[j])
			{
				// we found the letter
				big_word_copy[j] = '*';
				letter_found = 1;
				break;
			}
		}
		if (!letter_found) return 0;
	}
	return 1;
}



int dict_search(char *word_sorted, int word_length)
{
	int matches_found = 0;
	int partial_matches_found = 0;
	FILE *fp = fopen("dictionary.txt", "r");
	while (!feof(fp))
	{
		// read word from dictionary
		char dw[BUFFER_SIZE];
		char *dict_word = dw;
		fscanf(fp, "%s", dict_word);

		// check word length and convert to lower case
		int dict_word_length = strlen(dict_word);
		convert_to_lowercase(dict_word, word_length);
		// diagnostic:
		//printf("%s (%i)\n", dict_word, dict_word_length);

		// if dictionary word is same size, and sorts to identical string,
		// then we have a match

		// sort dictionary word
		char dws[BUFFER_SIZE];
		char *dict_word_sorted = dws;
		sort_word(dict_word, dict_word_sorted, dict_word_length);

		if (!strcmp(word_sorted, dict_word_sorted))
		{
			// we have a match!
			printf("%s", dict_word);
			//printf("\t*** MATCH ***\n");
			printf("\n");
			matches_found++;
		}
		else if (can_be_made_from(word_sorted, word_length, dict_word, dict_word_length) && dict_word_length >= 3)
		{
			// we have a partial match!
			printf("%s (partial match)", dict_word);
			//printf("\t*** MATCH ***\n");
			printf("\n");
			partial_matches_found++;
		}
	}
	fclose(fp);
	return matches_found;
}



int main()
{

	printf("\nINPUT WORD: ");
	char iw[BUFFER_SIZE];
	char *input_word = iw;
	scanf("%s", input_word);

	// check word length and convert to lower case
	int word_length = strlen(input_word);
	convert_to_lowercase(input_word, word_length);
	printf("\nYOU ENTERED: %s", input_word);
	printf("\nlength: %i", word_length);

	printf("\n\n");

	printf("sorting...\n");

	char ws[BUFFER_SIZE];
	char *word_sorted = ws;
	sort_word(input_word, word_sorted, word_length);
	printf("\nRESULT: %s", word_sorted);

	printf("\n\n");

	printf("ANAGRAMS:\n\n");
	int matches_found = dict_search(word_sorted, word_length);

	printf("\n%i matches found.\n", matches_found);

	printf("\n\n");

	return 0;
}
