#include "paroliere.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
struct board board_gen_sol() {
	int i, j, letter_index;
	struct board b;
	b.height = NROWS;
	b.width = NCOLS;
	//il seed è inizializzato nel main con  l'istruzione srand((unsigned int)time(NULL))
	for (i = 0; i < b.height; i++) {
		for (j = 0; j < b.width; j++) {
			letter_index = rand() % N_LETTERS;
			b.tiles[i][j].letter = let_score[letter_index].letter;
			b.tiles[i][j].value = let_score[letter_index].value;
		}
	}
	return b;
}

int check_dictionary_sol(char* word, char* filename) {
	int i, l;
	char string[100] = "\0";
	FILE* f = fopen(filename, "r");
	if (!f) return;

	while (fscanf(f, "%s ", string) != EOF)
		if (strcmp(string_to_lower(word), string) == 0)
			return 1;
	fclose(f);
	return 0;
}

int check_letters_sol(char* word, struct board b) {
	int i;
	int sc, bc;
	for (i = 0; word[i] != '\0'; i++) {
		sc = string_charcount(word, word[i]);
		bc = board_charcount(b, word[i]);
		if (sc > bc) return 0;
	}
	return 1;
}

int compute_score_sol(char* word, int length) {
	if (length <= 0) return 0;
	return (compute_score_sol(word, length - 1) + letter_to_score(word[length - 1]));
}
