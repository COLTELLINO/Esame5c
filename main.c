#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#include "paroliere.h"
#include "paroliere_sol.h"
#include <string.h>
#define FILENAME "dizionario.txt"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define END   "\x1b[0m"
#define NUM_TESTS 5

//#define TEST_GEN
//#define TEST_DICT
//#define TEST_SCORE
#define TEST_LETT


static bool test_valid();
static bool test_random();
static bool test_same_letter();
int main() {
	int i;
	char* words[] = { "abate","banana", "RISMA", "cane","tErRAcOttA", "ewowsg", "DFgsgS" };
	char* words2[] = {"CIAONTTOTRALL", "ciao", "CaNoTtO", "taRallO", "AAAAAAAAAA", "aaaaaaaaa", "KTEST"};
	struct board b = get_empty_board();
	int stu_ret, sol_ret;
	srand((unsigned int)time(NULL));
#ifdef TEST_GEN
	/*TEST BOARD GEN*/
	printf(YELLOW"=== TEST BOARD GEN ===\n"END);
	bool error=false; 
	if (!test_valid()) error = true;
	else printf("Test valid: ok\n");
	
	if(!test_random) error = true;
	else printf("Test random: ok\n");
	
	if (!test_same_letter()) error = true;
	else printf("Test same letter: ok\n");
	if (!error) printf("Test gen: "GREEN"ok\n"END);
	puts("");
#endif // TEST_GEN

#ifdef TEST_DICT
	/*TEST CHECK DICTIONARY*/
	printf(YELLOW"=== TEST CHECK DICTIONARY ===\n"END);

	for (i = 0; i < (sizeof(words) / sizeof(words[0])); i++)
		if ((stu_ret = check_dictionary(words[i], FILENAME)) != (sol_ret = check_dictionary_sol(words[i], FILENAME)))
			printf(RED"Errore: "END"check dictionary. Parola: %s, res: %d anziche' %d\n", words[i], stu_ret, sol_ret);
		else printf("test dictionary con %s: "GREEN"ok"END"\n", words[i]);
	puts("");
#endif // TEST_DICT

#ifdef TEST_SCORE
	/*TEST COMPUTE SCORE*/
	printf(YELLOW"=== TEST COMPUTE SCORE ===\n"END);
	for (i = 0; i < 3; i++) {
		stu_ret = compute_score(words[i], strlen(words[i]));
		sol_ret = compute_score_sol(words[i], strlen(words[i]));
		if (stu_ret != sol_ret)
			printf(RED"Errore: "END"compute score con %s restituisce: %d anziche' %d\n", words[i], stu_ret, sol_ret);
		else printf("test score con %s: "GREEN"ok"END"\n", words[i]);
	}

	puts("");
#endif // TEST_SCORE

#ifdef TEST_LETT

	/*TEST CHECK LETTERS*/
	printf(YELLOW"=== TEST CHECK LETTERS ===\n"END);
	b = costrained_board_gen(MUST_CONTAIN, words2[0]);
	bool stampa = false;
	for (i = 1; i < (sizeof(words2) / sizeof(words2[0])); i++) {
		stu_ret = check_letters(words2[i], b);
		sol_ret = check_letters_sol(words2[i], b);
		if (stu_ret != sol_ret) {
			stampa = true;
			printf(RED"Errore: "END" check letters. Parola: %s, res: %d anziche' %d\n", words2[i], stu_ret, sol_ret);
		}
		else printf("test check letters con %s: "GREEN"ok"END"\n", words2[i]);
	}

	if (stampa) {
		printf("ref board (h=%d, w=%d): \n", b.height, b.width);
		print_board(b);
	}
	puts("");
#endif // TEST_LETT
	return 0;
}

static bool test_valid() {
	int i;
	struct board b;
	for (i = 0; i < NUM_TESTS; i++) {
		b = board_gen();
		if (!is_a_valid_board(b)) {
			printf(RED"Errore: "END"board non valida\n");
			printf("stampa board (h=%d, w=%d): \n", b.height, b.width);
			print_board(b);
			puts("");
			return false;
		}
	}
	printf("stampa ultima board (h=%d, w=%d): \n", b.height, b.width);
	print_board(b);
	puts("");
	return true;
}
static bool test_random() {
	struct board b1, b2;
	b1 = board_gen();
	b2 = board_gen();
	if (board_compare(b1, b2)) {
		printf(RED"Errore: "END"genera stessa board\n");
		return false;
	}
	return true;
}
static bool test_same_letter() {
	struct board b, b2;
	b = board_gen();
	char l[2];
	l[0] = to_upper_case(b.tiles[1][1].letter);
	l[1] = '\0';
	b2 = costrained_board_gen(MUST_ONLY_CONTAIN, l);
	if (board_compare(b, b2)) {
		printf(RED"Errore: "END"genera sempre la stessa lettera\n");
		printf("stampa board (h=%d, w=%d): \n", b.height, b.width);
		print_board(b);
		puts("");
		return false;
	}
	return true;
}
