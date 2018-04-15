/* This is a managed file. Do not delete this comment. */

#include <corto/x/x.h>
int16_t x_visitor_construct(
    x_visitor this)
{
    return corto_class_construct(this);
}


x_callback x_visitor_findMethod(
    x_visitor this,
    x_rule rule)
{
    corto_interface interface = corto_interface(this);
    int32_t i;
    for (i = 0; i < interface->methods.length; i ++) {
        corto_method m = interface->methods.buffer[i];
        if (corto_typeof(m) == (corto_type)x_callback_o) {
            x_callback callback = x_callback(m);
            if (callback->rule == rule) {
                return callback;
            }
        }
    }

    return NULL;
}
