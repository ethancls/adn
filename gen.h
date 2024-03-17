#include "struct.h"
#include "math.h"
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>
#include <malloc/malloc.h> // Uniquement sous MACOS

// CPU time
double temps_CPU();

// Read the output file and verify that the sequence isn't already written
bool readFile(data_t *, int, FILE *);

// Check if the provided file is of the form dnaX.txt with X integer
bool verify_dna_filename(char *);

// Checks if the output file is in the form toto.txt
bool verify_output_filename(char *);

void printProgress(double, int, data_t*);

void dnasequencer();

// Defines the values used by the program, the default ones or those entered by the user
data_t defineConst();

FILE* modifierFichier(data_t *);

// Read the file and classify the data in a structure adn_t in the sequence 0
data_t lireFichier(data_t);

// Create data.POP random sequences
data_t rand_adn(data_t *);

// Stores the possible solutions in an array of characters S[]
data_t selection(data_t *);

// Evaluates the created solutions by assigning them a survival probability depending on data.IOTA and a score
data_t evaluation(data_t *, FILE *);

// Performs natural selection of solutions by probabilistic survival
data_t hades(data_t *);

// Uses reverse complementation to generate new solutions
data_t mutation(data_t *);

// Display viable solutions and store them in output/out.txt by defaults
data_t display_solutions(data_t *, int, FILE *);