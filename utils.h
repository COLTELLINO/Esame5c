#include <stdbool.h>
#define NROWS 5
#define NCOLS 6
#define N_LETTERS 21
struct tile {
	char letter;
	int value;
};

struct board {
	struct tile tiles[NROWS][NCOLS];
	int width;
	int height;
};

const struct tile let_score[N_LETTERS];

enum constraint_type { MUST_CONTAIN, MUST_NOT_CONTAIN, MUST_ONLY_CONTAIN, NONE };
static void print_empty_row(unsigned int length);
void print_board(struct board b);
bool is_a_valid_board(struct board b);
char to_upper_case(char l);
char to_lower_case(char l);
int board_charcount(struct board b, char c);
int string_charcount(char* string, char c);
struct board get_empty_board();
char* string_to_lower(char* string);
int letter_to_score(char l);
struct board costrained_board_gen(enum constraint_type cons, char* letters);
bool board_compare(struct board b1, struct board b2);