#include <stdint.h>
#include <stdio.h>

typedef struct {
    char value;
    uint8_t frequency;
} symbol_t;

typedef struct {
    size_t number_of_symbols;
    symbol_t* symbol;
} symbols_t;

/* ------------------------------------------------------------------------------- */

/** \brief Get the array of symbols sorted by their frequency of appearance in
ascending order.
* \param self as symbols_t*. Pointer to the symbols_t struct containing the data.
* \param fh_in as FILE*. Input file to be parsed to obtain the symbols.
* \return int. Error code (1 if OK).
*/
int symbols__get_from_input_file(symbols_t* self, FILE* fh_in);

/** \brief Print the array of symbols obtained from the file, containing the value
of the character and the frequency of its appearance in the text.
* \param self as const symbols_t*. Pointer to the symbols_t struct containing the data.
*/
void symbols__print(const symbols_t* self);
