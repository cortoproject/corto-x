/* $CORTO_GENERATED
 *
 * token.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/x/x.h>

corto_int16 _x_token_construct(
    x_token this)
{
/* $begin(corto/x/token/construct) */

    return 0;

/* $end */
}

corto_int16 _x_token_init(
    x_token this)
{
/* $begin(corto/x/token/init) */

    corto_setref(&this->type, corto_string_o);
    return 0;
/* $end */
}
