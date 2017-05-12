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

#define X_BEAD_GROUP_MIN (2)
#define X_BEAD_PATTERN_MIN (5)

typedef struct x_parser_beadRule x_parser_beadRule;
struct x_parser_beadRule {
    corto_route rule;
    regex_t regex;
    corto_bool empty;
};

typedef struct x_parser_bead x_parser_bead;
struct x_parser_bead {
    corto_id expr;
    int offset;
    corto_ll beads;
    corto_ll rules;
    regex_t regex;
    x_parser_bead *prev;
};

x_parser_bead *x_parser_beadNew(void) {
    x_parser_bead  *result = corto_calloc(sizeof(x_parser_bead));
    result->rules = corto_ll_new();
    return result;
}

x_parser_beadRule *x_parser_beadRuleNew(x_rule rule) {
    x_parser_beadRule *result = corto_calloc(sizeof(x_parser_beadRule));
    result->rule = corto_route(rule);
    result->empty = FALSE;
    return result;
}

void x_parser_printBeads(x_parser_bead *bead, int indent) {
    corto_iter it;
    corto_trace("x: %*s> bead '%s'", indent * 2, "", bead->expr);

    if (bead->rules) {
        it = corto_ll_iter(bead->rules);
        while (corto_iter_hasNext(&it)) {
            x_parser_beadRule *r = corto_iter_next(&it);
            corto_trace("x: %*s  - rule '%s'", indent * 2, "", &r->rule->pattern[bead->offset]);
        }
    }

    if (bead->beads) {
        it = corto_ll_iter(bead->beads);
        while (corto_iter_hasNext(&it)) {
            x_parser_bead *b = corto_iter_next(&it);
            x_parser_printBeads(b, indent + 1);
        }
    }
}

corto_bool x_parser_beadMajorityInBeads(x_parser_bead *bead, char ch) {
    if (bead->beads) {
        corto_iter it = corto_ll_iter(bead->beads);
        while (corto_iter_hasNext(&it)) {
            x_parser_bead *b = corto_iter_next(&it);
            if (b->expr && (b->expr[0] == ch)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

corto_string x_parser_regexFromExpr(x_parser this, corto_string expr) {
    x_pattern p = corto_declare(x_pattern_o);
    corto_string result = NULL;
    if (!p) {
        goto error;
    }
    corto_ptr_setstr(&p->expr, expr);
    corto_ptr_setref(&p->scope, corto_parentof(this));
    if (corto_define(p)) {
        goto error;
    }
    if (p->regex) {
        corto_asprintf(&result, "^%s", p->regex);
    }
    corto_delete(p);
    return result;
error:
    return NULL;
}

corto_int16 x_parser_compileBeads(x_parser this, x_parser_bead *bead) {
    if (strlen(bead->expr)) {
        corto_string regex = x_parser_regexFromExpr(this, bead->expr);
        if (!regex) {
            goto error;
        }

        if (regcomp(&bead->regex, regex, REG_EXTENDED)) {
            goto error;
        }

        corto_dealloc(regex);
    }

    if (bead->beads) {
        corto_iter it = corto_ll_iter(bead->beads);
        while (corto_iter_hasNext(&it)) {
            x_parser_bead *b = corto_iter_next(&it);
            if (x_parser_compileBeads(this, b)) {
                goto error;
            }
        }
    }

    if (bead->rules) {
        corto_iter it = corto_ll_iter(bead->rules);
        while (corto_iter_hasNext(&it)) {
            x_parser_beadRule *r = corto_iter_next(&it);
            corto_string regex = x_parser_regexFromExpr(this, &r->rule->pattern[bead->offset]);
            if (!regex) {
                r->empty = TRUE;
            } else {
                if (regcomp(&r->regex, regex, REG_EXTENDED)) {
                    goto error;
                }
            }
            corto_dealloc(regex);
        }
    }

    return 0;
error:
    return -1;
}

corto_bool x_parser_cleanEmptyBeads(x_parser_bead *bead) {
    if (bead->beads) {
        corto_iter it = corto_ll_iter(bead->beads);
        while (corto_iter_hasNext(&it)) {
            x_parser_bead *b = corto_iter_next(&it);
            if (x_parser_cleanEmptyBeads(b)) {
                corto_ll_remove(bead->beads, b);
            }
        }
        if (!corto_ll_size(bead->beads)) {
            corto_ll_free(bead->beads);
            bead->beads = NULL;
        }
    }
    if (bead->rules) {
        if (!corto_ll_size(bead->rules)) {
            corto_ll_free(bead->rules);
            bead->rules = NULL;
        }
    }

    return bead->rules == NULL && bead->beads == NULL;
}

corto_route x_parser_findRouteInBeads(x_parser_bead *b, corto_string str) {
    char *ptr = str;
    corto_iter it;

    /* If bead has expression, test if string matches */
    if (b->expr[0]) {
        regmatch_t match;
        if (regexec(&b->regex, str, 1, &match, 0)) {
            return NULL;
        }
        ptr = &str[match.rm_eo];
    }

    /* If string matches (or if bead is root) first look in other beads */
    if (b->beads) {
        it = corto_ll_iter(b->beads);
        while (corto_iter_hasNext(&it)) {
            x_parser_bead *bead = corto_iter_next(&it);
            corto_route result = x_parser_findRouteInBeads(bead, ptr);
            if (result) return result;
        }
    }

    /* If no match is found in other beads, look in own rules */
    if (b->rules) {
        it = corto_ll_iter(b->rules);
        while (corto_iter_hasNext(&it)) {
            x_parser_beadRule *rule = corto_iter_next(&it);
            if (rule->empty) {
                if (!ptr[0]) {
                    return rule->rule;
                }
            } else {
                if (!regexec(&rule->regex, ptr, 0, NULL, 0)) {
                    return rule->rule;
                }
            }
        }
    }

    return NULL;
}

x_parser_bead* x_parser_optimize(x_parser this) {
    corto_objectseq *methods = &corto_interface(this)->methods;
    x_parser_bead *b_root = NULL, *b_cur = NULL;
    corto_iter it;
    corto_int32 i;
    corto_bool changed = FALSE;

    b_root = x_parser_beadNew();

    /* Initialize root bead */
    for (i = 0; i < methods->length; i++) {
        if (corto_instanceof(x_rule_o, methods->buffer[i])) {
            corto_route r = methods->buffer[i];
            if (r->pattern) {
                x_parser_beadRule *br = x_parser_beadRuleNew(x_rule(r));
                corto_ll_append(b_root->rules, br);
            }
        }
    }

    b_cur = b_root;

    int prev = 0;
    do {
        corto_int32 n, n_prev = 0, count = 0, max = 0;
        char majority = 0, ch = 0;

        changed = FALSE;
        n_prev = 0;
        n = b_cur->offset;

        corto_debug("\n==== ITER (%s, offset = %d)\n", b_cur->expr, n);

        do {
            max = 0;
            majority = '\0';
            it = corto_ll_iter(b_cur->rules);

            while (corto_iter_hasNext(&it)) {
                x_parser_beadRule *r = corto_iter_next(&it);
                if (r->rule->pattern) {
                    if (!(ch = r->rule->pattern[n])) {
                        break;
                    }
                    if (!majority || (ch != majority)) {
                        count = 0;
                        corto_iter it2 = corto_ll_iter(b_cur->rules);
                        for (i = 0; corto_iter_hasNext(&it2); i++) {
                            x_parser_beadRule *r = corto_iter_next(&it2);
                            if (r->rule->pattern) {
                                if (r->rule->pattern[n] == ch) {
                                    count++;
                                }
                            }
                        }
                        if (count >= max && !x_parser_beadMajorityInBeads(b_cur, ch)) {
                            majority = ch;
                            max = count;
                        }
                    }
                }
            }

            /* If current majority is smaller than X_BEAD_GROUP_MIN, don't create new bead */
            if (!ch || (max < X_BEAD_GROUP_MIN)) {
                break;
            }

            corto_debug(">> majority = '%c', total = %d, max = %d, prev = %d, n = %d, n_prev = %d\n",
                majority, corto_ll_size(b_cur->rules), max, prev, n, n_prev);

            if ((prev != max) && (!prev || ((prev - max) >= X_BEAD_GROUP_MIN))) {
                corto_debug(">> new bead '%s' -> '%c'\n", b_cur->expr, majority);
                x_parser_bead *b = x_parser_beadNew();
                b->prev = b_cur;
                b->expr[0] = majority;
                b->expr[1] = '\0';
                b->offset = b_cur->offset + 1;
                if (!b_cur->beads) {
                    b_cur->beads = corto_ll_new();
                }
                corto_ll_append(b_cur->beads, b);

                /* Move matching rules to new bead */
                it = corto_ll_iter(b_cur->rules);
                while (corto_iter_hasNext(&it)) {
                    x_parser_beadRule *r = corto_iter_next(&it);
                    if (r->rule->pattern && (r->rule->pattern[n] == majority)) {
                        corto_ll_remove(b_cur->rules, r);
                        corto_ll_append(b->rules, r);
                        corto_debug("moved from '%s'=>'%s' (%s)\n", b_cur->expr, b->expr, r->rule->pattern);
                        changed = TRUE;
                    }
                }

                n_prev = n;
                b_cur = b;
            } else {
                corto_int32 len = strlen(b_cur->expr);
                b_cur->expr[len] = majority;
                b_cur->expr[len + 1] = '\0';
                b_cur->offset ++;

                if (prev != max) {
                    it = corto_ll_iter(b_cur->rules);
                    while (corto_iter_hasNext(&it)) {
                        x_parser_beadRule *r = corto_iter_next(&it);
                        if (r->rule->pattern && (r->rule->pattern[n] != majority)) {
                            corto_ll_remove(b_cur->rules, r);
                            corto_ll_append(b_cur->prev->rules, r);
                            corto_debug("moved from '%s'=>'%s' (%s)\n", b_cur->expr, b_cur->prev->expr, r->rule->pattern);
                            changed = TRUE;
                        }
                    }
                }
            }

            prev = max;
            n++;
        } while (max >= X_BEAD_GROUP_MIN);

        if (!changed) {
            b_cur = b_cur->prev;
            prev = 0;
        }
    } while (b_cur);

    x_parser_cleanEmptyBeads(b_root);

    x_parser_printBeads(b_root, 0);

    if (x_parser_compileBeads(this, b_root)) {
        goto error;
    }

    return b_root;
error:
    return NULL;
}
/* $end */

int16_t _x_parser_construct(
    x_parser this)
{
/* $begin(corto/x/parser/construct) */
    corto_int16 ret = corto_routerimpl_construct(this);
    if (ret) {
        goto error;
    }

    /* Build regex chain from rules to avoid evaluating substrings twice */
    if (!(this->ruleChain = (corto_word)x_parser_optimize(this))) {
        goto error;
    }

    return 0;
error:
    return -1;
/* $end */
}

corto_route _x_parser_findRoute_v(
    x_parser this,
    corto_object instance,
    corto_stringseq pattern,
    corto_any param,
    corto_any *routerData)
{
/* $begin(corto/x/parser/findRoute) */
    // Uncomment this line to switch to legacy lookup of routes (slow)
    // return corto_routerimpl_findRoute_v(this, pattern, param, routerData);

    // Find route in optimized parser administration
    x_parser_bead *b = (x_parser_bead*)this->ruleChain;

    corto_route result = x_parser_findRouteInBeads(b, pattern.buffer[0]);
    if (result) {
        /* matchRoute extracts data from the string and stores it in routerData */
        if (x_parser_matchRoute(this, result, pattern, param, routerData)) {
            printf("matchRoute error\n");
            goto error;
        }

        /* If parameter is set to a visitor, call visitor and return NULL, so
         * the router won't invoke the rule of the parser class */
        if (param.value && corto_typeof(param.type) == corto_type(x_visitor_o)) {
            corto_object visitor = param.value;
            corto_method callback = corto_interface_resolveMethod(
                corto_typeof(visitor), corto_idof(result));

            /* Visitor doesn't necessarily implement callbacks for all rules */
            if (callback) {
                void *args[3];
                args[0] = &visitor;
                args[1] = instance,
                args[2] = &routerData->value;
                corto_callb(corto_function(callback), NULL, args);
            }

            /* If visitor implements matched, call it for every rule */
            callback = corto_interface_resolveMethod(
                corto_typeof(visitor), "_matched");
            if (callback) {
                void *args[3];
                args[0] = &visitor;
                args[1] = instance,
                args[2] = &routerData->value;
                corto_callb(corto_function(callback), NULL, args);
            }

            result = NULL;
        }
    }

    return result;
error:
    return NULL;
/* $end */
}

int32_t _x_parser_matchRoute_v(
    x_parser this,
    corto_route route,
    corto_stringseq pattern,
    corto_any param,
    corto_any *routerData)
{
/* $begin(corto/x/parser/matchRoute) */
    x_rule rule = x_rule(route);
    regex_t *regex = (regex_t*)rule->compiledRegex;
    int ret = regex ? -1 : 0;

    if (regex) {
        regmatch_t *match = alloca(sizeof(regmatch_t) * (regex->re_nsub + 1));

        ret = regexec(regex, pattern.buffer[0], (size_t)regex->re_nsub + 1, match, 0);

        if (!ret) {
            corto_object result = corto_declare(rule->pattern);

            int i;
            for (i = 1; i <= regex->re_nsub; i++) {
                x_pattern_parameter *p = corto_ll_get(rule->pattern->params, i - 1);
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
                corto_int16 ret = corto_value_memberExpr(&o, p->name, &m);
                if (ret) {
                    corto_seterr("invalid parameter '%s': %s", p->name, corto_lasterr());
                    corto_delete(result);
                    goto error;
                }

                r = corto_value_empty();
                v = corto_value_value(&substrPtr, corto_string_o);
                corto_value_binaryOp(CORTO_ASSIGN, &m, &v, &r);
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
