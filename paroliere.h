#ifndef UTILS
#define UTILS
#include "utils.h"
#endif // !UTILS

struct board board_gen_sol();

int check_dictionary_sol(char* word, char* filename);

int check_letters_sol(char* word, struct board b);

int compute_score_sol(char* word, int length);