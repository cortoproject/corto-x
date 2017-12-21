/* This is a managed file. Do not delete this comment. */

#include <corto/x/x.h>
int16_t x_callback_construct(
    x_callback this)
{
    corto_function f = corto_function(this);
    x_visitor visitor = x_visitor(corto_parentof(this));

    /* Find matching rule (by id) in parser */
    corto_method rule = corto_interface_resolveMethod(
        visitor->parser,
        corto_idof(this));

    if (!rule) {
        corto_throw("cannot find rule '%s' in parser '%s'",
            corto_idof(this),
            corto_fullpath(NULL, visitor->parser));
        goto error;
    }

    f->parameters.buffer = corto_calloc(sizeof(corto_parameter) * 2);
    f->parameters.length = 2;

    corto_set_str(&f->parameters.buffer[0].name, "parser");
    corto_set_ref(&f->parameters.buffer[0].type, visitor->parser);

    corto_set_str(&f->parameters.buffer[1].name, "node");
    corto_set_ref(&f->parameters.buffer[1].type, corto_function(rule)->parameters.buffer[0].type);

    return corto_super_construct(this);
error:
    return -1;
}
