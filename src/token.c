/* This is a managed file. Do not delete this comment. */

#include <corto/x/x.h>
int16_t x_token_construct(
    x_token this)
{

    return 0;

}

int16_t x_token_init(
    x_token this)
{

    corto_set_ref(&this->type, corto_string_o);
    return 0;
}

