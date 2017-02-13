/* $CORTO_GENERATED
 *
 * callback.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/x/x.h>

corto_int16 _x_callback_construct(
    x_callback this)
{
/* $begin(corto/x/callback/construct) */
    corto_function f = corto_function(this);
    x_visitor visitor = x_visitor(corto_parentof(this));

    /* Find matching rule (by id) in parser */
    corto_method rule = corto_interface_resolveMethod(
        visitor->parser, 
        corto_idof(this));

    if (!rule) {
        corto_seterr("cannot find rule '%s' in parser '%s'",
            corto_idof(this),
            corto_fullpath(NULL, visitor->parser));
        goto error;
    }

    f->parameters.buffer = corto_calloc(sizeof(corto_parameter) * 2);
    f->parameters.length = 2;

    corto_setstr(&f->parameters.buffer[0].name, "parser");
    corto_setref(&f->parameters.buffer[0].type, visitor->parser);

    corto_setstr(&f->parameters.buffer[1].name, "node");
    corto_setref(&f->parameters.buffer[1].type, corto_function(rule)->parameters.buffer[0].type);

    return corto_method_construct(this);
error:
    return -1;
/* $end */
}
