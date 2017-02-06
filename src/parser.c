/* $CORTO_GENERATED
 *
 * parser.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/x/x.h>

/* $header() */
#include <regex.h>   
/* $end */

corto_int32 _x_parser_matchRoute(
    x_parser this,
    corto_route route,
    corto_stringseq pattern,
    corto_any param,
    corto_any *routerData)
{
/* $begin(corto/x/parser/matchRoute) */
    x_rule rule = x_rule(route);
    regex_t *regex = (regex_t*)rule->compiledRegex;
    int ret = -1;

    if (regex) {
        regmatch_t *match = alloca(sizeof(regmatch_t) * (regex->re_nsub + 1));
        ret = regexec(regex, pattern.buffer[0], (size_t)regex->re_nsub + 1, match, 0);

        if (!ret) {
            corto_object result = corto_declare(rule->pattern);

            int i;
            for (i = 1; i <= regex->re_nsub; i++) {
                x_pattern_parameter *p = corto_llGet(rule->pattern->params, i - 1);
                if (!p->name) {
                    continue;
                }
                
                corto_id substr;
                char *substrPtr = substr;
                memcpy(substr, &pattern.buffer[0][match[i].rm_so], match[i].rm_eo - match[i].rm_so);
                substr[match[i].rm_eo - match[i].rm_so] = '\0';

                /* Assign member of result object */
                corto_value o, m, v, r;
                o = corto_value_object(result, NULL);
                corto_int16 ret = corto_value_getMember(&o, p->name, &m);
                if (ret) {
                    corto_seterr("invalid parameter '%s': %s", p->name, corto_lasterr());
                    corto_delete(result);
                    goto error;
                }

                r = corto_value_init();
                v = corto_value_value(corto_string_o, &substrPtr);
                corto_value_binaryOperator(CORTO_ASSIGN, &m, &v, &r);
                corto_value_free(&r);
            }

            if (corto_define(result)) {
                corto_delete(result);
                goto error;
            }

            routerData->type = corto_type(rule->pattern);
            routerData->value = result;
            
        } else if (ret != REG_NOMATCH) {
            corto_seterr("x: error matching regex");
        }
    }

    return !ret ? 0 : -1;
error:
    return -1;
/* $end */
}
