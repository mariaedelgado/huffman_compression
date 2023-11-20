#include <stdbool.h>
#include <stdlib.h>

#include "../include/symbols.h"

static int symbols__add_new_symbol(symbols_t* self, char c)
{
    int ret = -1;

    if (self == NULL)
    {
        goto end;
    }

    // Increase the counter of number of symbols and resize the array.
    self->number_of_symbols++;
    void* tmp = realloc(self->symbol, self->number_of_symbols*sizeof(symbol_t));
    if (tmp == NULL)  {
        goto end;
    } else {
        self->symbol = tmp;
    }

    // Add information about new symbol in the correct index of the array.
    self->symbol[self->number_of_symbols - 1].value = c;
    self->symbol[self->number_of_symbols - 1].frequency = 1;
    ret = 0;

end:
    return ret;
}

static int symbols__store_symbol(symbols_t* self, char c)
{
    int ret = -1;

    if (self == NULL)
    {
        goto end;
    }

    // Iterate all the stored symbols. If already present, increase the
    // number of ocurrences of symbol 'c'.
    for (size_t i = 0; i < self->number_of_symbols; i++)
    {
        if (c == self->symbol[i].value) 
        {
            self->symbol[i].frequency++;
            ret = 0;
            goto end;
        }
    }

    // If symbol was not present, add it.
    ret = symbols__add_new_symbol(self, c);

end:
    return ret;
}

static int symbols__swap_symbol_index(symbol_t* symbol_i, symbol_t* symbol_min)
{
    int ret = -1;
    symbol_t tmp = *symbol_i;

    if ((symbol_i == NULL) || (symbol_min) == NULL)
    {
        goto end;
    }

    *symbol_i = *symbol_min;
    *symbol_min = tmp;
    ret = 0;

end:
    return ret;
}

static int symbols__sort(symbols_t* self)
{
    int ret = -1;
    int min_index = 0;

    if (self == NULL)
    {
        goto end;
    }

    for (int i = 0; i < self->number_of_symbols - 1; i++)
    {
        min_index = i;

        for (int j = i + 1; j < self->number_of_symbols; j++)
        {
            if (self->symbol[j].frequency < self->symbol[min_index].frequency)
                min_index = j;
        }

        symbols__swap_symbol_index(&self->symbol[i], &self->symbol[min_index]);
    }

end:
    return ret;
}

int symbols__get_from_input_file(symbols_t* self, FILE* fh_in)
{
    int ret = -1;

    if (fh_in == NULL)
    {
        goto end;
    }

    while (true)
    {
        char c = fgetc(fh_in);

        if (feof(fh_in))
        {
            ret = 0;
            break;
        }

        ret = symbols__store_symbol(self, c);
        if (ret != 0)
        {
            goto end;
        }
    }

    ret = symbols__sort(self);
    
end:
    return ret;
}

void symbols__print(const symbols_t* self)
{
    for (int i = 0; i < self->number_of_symbols; i++)
    {
        printf("Symbol: %c / Frequency: %d\n", self->symbol[i].value, self->symbol[i].frequency);
    }

    return;
}