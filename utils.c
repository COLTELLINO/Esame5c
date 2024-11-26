#include "utils.h"
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
const struct tile let_score[N_LETTERS] = {
	{'A',1}, {'B', 4}, {'C', 1},
	{'D',4}, {'E', 1}, {'F', 4},
	{'G',4}, {'H', 8}, {'I', 1},
	{'L',2}, {'M', 2}, {'N', 2},
	{'O',1}, {'P', 3}, {'Q', 9},
	{'R',1}, {'S', 1}, {'T', 1},
	{'U',4}, {'V', 4}, {'Z', 8}
};

/*USATE NELLA SOLUZIONE*/

char* string_to_lower(char* string) {
	char res[100];
	int i;
	for (i = 0; i < (int)strlen(string); i++)
		res[i] = to_lower_case(string[i]);
	res[i] = '\0';
	return res;
}

int board_charcount(struct board b, char c) {
	int i, j, count = 0;
	char l;
	c = to_upper_case(c);
	for (i = 0; i < b.height; i++)
		for (j = 0; j < b.width; j++) {
			l = b.tiles[i][j].letter;
			if (l == c)
				count++;
		}

	return count;
}

int string_charcount(char* string, char c) {
	int i, count = 0;
	for (i = 0; string[i] != '\0'; i++)
		if (string[i] == c)
			count++;
	return count;
}

int letter_to_score(char l) {
	int i;
	l = to_upper_case(l);
	if (l >= 'A' && l < 'J') i = l - 'A';
	else if (l > 'K' && l < 'W') i = l - 'A' - 2;
	else if (l == 'Z') i = N_LETTERS - 1;
	else return 0;
	return let_score[i].value;

	/*  PIU' CORTO MA MENO EFFICIENTE:
		for (i = 0; i < NUM_LETTERS; i++)
			if (let_score[i].letter == l)
				return let_score[i].value;
		return 0;
	*/
}

char to_upper_case(char l) {
	return (l >= 'a' && l <= 'z') ? l - ('a' - 'A') : l;
}

char to_lower_case(char l) {
	int gap = 'a' - 'A';
	return (l >= 'A' && l <= 'Z') ? l + gap : l;
}



/*USATE SOLO NEI TEST*/
static void print_empty_row(unsigned int length) {
	while (length-- != 0)
		printf(".   .   ");
	puts(".");
}
bool is_italian(char l) {
	return l != 'J' && l != 'K' && l != 'W' && l != 'X' && l != 'Y';
}
static bool check_dim(struct board b) {
	bool ok = b.height == NROWS && b.width == NCOLS;
	if(!ok) printf("Errore: dimensioni board errate\n");
	return ok;
}
static bool check_value(struct tile t) {
	int real_v= letter_to_score(t.letter);
	bool ok;
	ok = (t.value == real_v);
	if (!ok) printf("Errore: valore di %c errato. %d anzichè %d \n", t.letter, t.value, real_v);
	return ok;
}
static bool check_char(struct tile t) {
	char l = to_upper_case(t.letter);
	bool ok = (l >= 'A' && l <= 'Z' && is_italian(l));
	if (!ok) printf("Errore: cattere %c non consentito.\n", t.letter);
	return ok;
}
void print_board(struct board b) {
	int i, j;
	char toprint;
	print_empty_row(b.width);
	for (i = 0; i < b.height; i++) {

		for (j = 0; j < b.width; j++) {
			toprint = b.tiles[i][j].letter;
			printf(".   %c   ", b.tiles[i][j].letter);
		}

		puts(".");

		for (j = 0; j < b.width; j++) {
			toprint = b.tiles[i][j].value;
			printf(".  (%d)  ", b.tiles[i][j].value);
		}


		puts(".");
		print_empty_row(b.width);
	}
}
bool is_a_valid_board(struct board b) {
	int i,j;
	if (!check_dim(b)) return false;
	for (i = 0; i < NROWS; i++) 
		for (j = 0; j < NCOLS; j++) 
			if (!(check_value(b.tiles[i][j]) && check_char(b.tiles[i][j])))
				return false;
	return true;
}
struct board get_empty_board() {
	struct board b;
	b.height = 0;
	b.width = 0;
	return b;
}
struct board costrained_board_gen(enum constraint_type cons, char* letters) {
	int i, j, letter_index;
	struct board b;
	char lett;
	char specular[N_LETTERS+1];
	b.height = NROWS;
	b.width = NCOLS;

	for (i = 0, lett = 'A'; lett <= 'Z'; lett++) {
		if (is_italian(lett)&&strchr(letters, lett) == NULL) {
			specular[i] = lett;
			i++;
		}
	}
	specular[i] = '\0';

	for (i = 0; i < b.height; i++) {
		for (j = 0; j < b.width; j++) {
			switch (cons)
			{
			case MUST_CONTAIN:
				letter_index = (i * b.width + j + 1) - 1;
				if (letter_index < (int)strlen(letters)) {
					lett = to_upper_case(letters[letter_index]);
				}
				else {
					letter_index = rand() % N_LETTERS;
					lett = let_score[letter_index].letter;
				}
				break;
			case MUST_NOT_CONTAIN:
				letter_index = rand() % strlen(specular);
				lett = to_upper_case(specular[letter_index]);
				break;
			case MUST_ONLY_CONTAIN:
				letter_index = rand() % strlen(letters);
				lett = letters[letter_index];
				if(is_italian(lett))
					lett = to_upper_case(lett);
				break;
			case NONE:
				letter_index = rand() % N_LETTERS;
				lett = let_score[letter_index].letter;
				break;
			default:
				break;
			}
			b.tiles[i][j].letter = lett;
			b.tiles[i][j].value = letter_to_score(lett);
		}
	}
	return b;
}
static bool tiles_compare(struct tile t1, struct tile t2) {
	return (t1.letter == t2.letter) && (t1.value == t2.value);
}
bool board_compare(struct board b1, struct board b2) {
	int i, j;
	if (b1.height != b2.height || b1.width != b2.width) 
		return false;
	for (i = 0; i < b1.height; i++) 
		for (j = 0; j < b1.width; j++) 
			if (!tiles_compare(b1.tiles[i][j], b2.tiles[i][j])) 
				return false;
	return true;
}