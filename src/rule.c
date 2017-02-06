/* $CORTO_GENERATED
 *
 * rule.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/x/x.h>

/* $header() */
#include <regex.h>   
/* $end */

corto_int16 _x_rule_construct(
    x_rule this)
{
/* $begin(corto/x/rule/construct) */
    if (corto_route(this)->pattern) {
        this->pattern = corto_declareChild(corto_parentof(corto_parentof(this)), corto_idof(this), x_pattern_o);
        corto_setstr(&this->pattern->expr, corto_route(this)->pattern);
        corto_setref(&this->pattern->scope, corto_parentof(corto_parentof(this)));
        if (corto_define(this->pattern)) {
            goto error;
        }

        corto_asprintf(&this->regex, "^%s$", this->pattern->regex);

        corto_trace("%s: regex = %s", corto_idof(this), this->regex);

        /* Compile regular expression */
        regex_t *regex = corto_alloc(sizeof(regex_t));
        int ret = regcomp(regex, this->pattern->regex, REG_EXTENDED);
        if (ret) {
            corto_seterr("x: failed to compile regex '%s'\n", this->pattern->regex);
            goto error;
        }

        this->compiledRegex = (corto_word)regex;
    }

    corto_int16 ret = corto_route_construct(this);
    if (ret) {
        goto error;
    }

    /* Overwrite type of 'node' to match the type of the pattern */
    if (this->pattern) {
        corto_setref(&corto_function(this)->parameters.buffer[0].type, this->pattern);
    }

    return 0;
error:
    return -1;
/* $end */
}

corto_void _x_rule_destruct(
    x_rule this)
{
/* $begin(corto/x/rule/destruct) */

    regfree((regex_t*)this->compiledRegex);
    corto_dealloc((regex_t*)this->compiledRegex);

/* $end */
}
