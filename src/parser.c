/* This is a managed file. Do not delete this comment. */

#include <corto/x/x.h>

#include <regex.h>

#define X_BEAD_GROUP_MIN (2)
#define X_BEAD_PATTERN_MIN (5)
#define X_SUBEXPR_BUFF_SIZE (256)

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
            corto_trace("x: %*s  - rule '%s' ('%s')", indent * 2, "", &r->rule->pattern[bead->offset], corto_idof(r->rule));
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
            if (b->expr[0] == ch) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

corto_string x_parser_regexFromExpr(x_parser this, corto_string expr, bool rule) {
    x_pattern p = corto_declare(NULL, NULL, x_pattern_o);
    corto_string result = NULL;
    if (!p) {
        goto error;
    }
    corto_set_str(&p->expr, expr);
    corto_set_ref(&p->scope, corto_parentof(this));
    if (corto_define(p)) {
        goto error;
    }
    if (p->regex) {
        if (rule) {
            result = corto_asprintf("^%s$", p->regex);
        } else {
            result = corto_asprintf("^%s", p->regex);
        }
    }
    corto_delete(p);
    return result;
error:
    return NULL;
}

corto_int16 x_parser_compileBeads(x_parser this, x_parser_bead *bead) {
    if (strlen(bead->expr)) {
        corto_string regex = x_parser_regexFromExpr(this, bead->expr, false);
        if (!regex) {
            goto error;
        }

        corto_debug("x: compiling regex '%s' for bead '%s'", regex, bead->expr);
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
            corto_string regex = x_parser_regexFromExpr(this, &r->rule->pattern[bead->offset], true);
            if (!regex) {
                r->empty = TRUE;
                corto_debug("x: mark rule '%s' as empty", corto_idof(r->rule));
            } else {
                corto_debug("x: compiling regex '%s' for rule '%s'", regex, corto_idof(r->rule));
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
        if (!corto_ll_count(bead->beads)) {
            corto_ll_free(bead->beads);
            bead->beads = NULL;
        }
    }
    if (bead->rules) {
        if (!corto_ll_count(bead->rules)) {
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
        corto_debug("evaluate bead '%s' for '%s'", b->expr, str);
        regmatch_t match;
        if (regexec(&b->regex, str, 1, &match, 0)) {
            corto_debug("bead '%s' does not match '%s'", b->expr, str);
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
                corto_debug("evaluate empty rule '%s' for '%s'", corto_idof(rule->rule), ptr);
                if (!ptr[0]) {
                    corto_debug("matched empty rule '%s' with '%s'", corto_idof(rule->rule), ptr);
                    return rule->rule;
                }
            } else {
                corto_debug("evaluate rule '%s' (regex='%s') for '%s'", corto_idof(rule->rule), x_rule(rule->rule)->regex, ptr);
                if (!regexec(&rule->regex, ptr, 0, NULL, 0)) {
                    corto_debug("matched rule '%s' with '%s'", corto_idof(rule->rule), ptr);
                    return rule->rule;
                }
            }
        }
    }

    return NULL;
}

int x_parser_scanForMajority(char *expr, char *majorityBuffer) {
    char *ptr = expr, ch;
    bool inlineRegex = false;

    if (expr[0] == '{') {
        for (ptr = expr; (ch = *ptr); ptr ++) {
            majorityBuffer[ptr - expr] = ch;
            if (ch == '(') {
                inlineRegex = true;
            } else if (ch == ')') {
                inlineRegex = false;
            } else if (ch == '\\') {
                ptr ++;
                majorityBuffer[ptr - expr] = ch;
            }
            if (!inlineRegex && ch == '}') {
                ptr ++;
                majorityBuffer[ptr - expr] = '}';
                break;
            }
        }
        majorityBuffer[ptr - expr] = '\0';
    } else {
        majorityBuffer[0] = expr[0];
        majorityBuffer[1] = '\0';
        ptr = expr + 1;
    }

    return ptr - expr;
}

x_parser_bead* x_parser_optimize(x_parser this) {
    corto_objectseq *methods = &corto_interface(this)->methods;
    x_parser_bead *b_root = NULL, *b_cur = NULL;
    corto_iter it;
    corto_int32 i;
    corto_bool changed = FALSE;

    b_root = x_parser_beadNew();

    /* Initially add all rules as beads to the root */
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
        corto_int32 n, count = 0, max = 0;
        char ch = 0;
        int majorityLen = 0;

        /* The majority buffer holds a string that is shared by the majority
         * of rules currently evaluated. The reason that this is a string and is
         * not evaluated on a per-character basis is because subexpressions ({})
         * must be evaluated atomically, and should not be split up. */
        char buff[X_SUBEXPR_BUFF_SIZE];
        char majority[X_SUBEXPR_BUFF_SIZE];

        changed = FALSE;
        n = b_cur->offset;

        corto_trace("x: iter ('%s', offset = %d)", b_cur->expr, n);

        do {
            max = 0;
            majority[0] = '\0';

            it = corto_ll_iter(b_cur->rules);
            while (corto_iter_hasNext(&it)) {
                x_parser_beadRule *r = corto_iter_next(&it);
                if (r->rule->pattern) {
                    if (!(majorityLen = x_parser_scanForMajority(&r->rule->pattern[n], buff))) {
                        break;
                    }
                    if (!majority[0] || (strcmp(buff, majority))) {
                        count = 0;
                        corto_iter it2 = corto_ll_iter(b_cur->rules);
                        for (i = 0; corto_iter_hasNext(&it2); i++) {
                            x_parser_beadRule *r = corto_iter_next(&it2);
                            if (r->rule->pattern) {
                                char patternBuff[X_SUBEXPR_BUFF_SIZE];
                                x_parser_scanForMajority(&r->rule->pattern[n], patternBuff);
                                if (!strncmp(patternBuff, buff, majorityLen)) {
                                    count++;
                                }
                            }
                        }
                        if (count >= max && !x_parser_beadMajorityInBeads(b_cur, ch)) {
                            strcpy(majority, buff);
                            max = count;
                        }
                    }
                }
            }

            /* If current majority is smaller than X_BEAD_GROUP_MIN, don't create new bead */
            if (!strlen(majority) || (max < X_BEAD_GROUP_MIN)) {
                break;
            }

            majorityLen = strlen(majority);

            corto_trace("x: majority = '%s', total = %d, max = %d, prev = %d, n = %d",
                majority, corto_ll_count(b_cur->rules), max, prev, n);

            if ((prev != max) && (!prev || ((prev - max) >= X_BEAD_GROUP_MIN))) {
                corto_trace("x: new bead '%s' -> '%s'", b_cur->expr, majority);

                x_parser_bead *b = x_parser_beadNew();
                b->prev = b_cur;
                strcpy(b->expr, majority);
                b->offset = b_cur->offset + majorityLen;
                if (!b_cur->beads) {
                    b_cur->beads = corto_ll_new();
                }
                corto_ll_append(b_cur->beads, b);

                /* Move matching rules to new bead */
                it = corto_ll_iter(b_cur->rules);
                while (corto_iter_hasNext(&it)) {
                    x_parser_beadRule *r = corto_iter_next(&it);
                    if (r->rule->pattern && (!strncmp(&r->rule->pattern[n], majority, majorityLen))) {
                        corto_ll_remove(b_cur->rules, r);
                        corto_ll_append(b->rules, r);
                        corto_trace("x: moved '%s' from '%s' to '%s'", corto_idof(r->rule), b_cur->expr, b->expr);
                        changed = TRUE;
                    }
                }

                b_cur = b;

            } else {
                corto_int32 len = strlen(b_cur->expr);
                strcpy(&b_cur->expr[len], majority);
                b_cur->offset += strlen(majority);

                if (prev != max) {
                    it = corto_ll_iter(b_cur->rules);
                    while (corto_iter_hasNext(&it)) {
                        x_parser_beadRule *r = corto_iter_next(&it);

                        /* Move rules that no longer match to previous bead */
                        if (r->rule->pattern && (strncmp(&r->rule->pattern[n], majority, majorityLen))) {
                            corto_ll_remove(b_cur->rules, r);
                            corto_ll_append(b_cur->prev->rules, r);
                            corto_trace("x: moved '%s' from '%s' to '%s'", corto_idof(r->rule), b_cur->expr, b_cur->prev->expr);
                            changed = TRUE;
                        }
                    }
                }
            }

            prev = max;
            n += majorityLen;
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

int16_t x_parser_construct(
    x_parser this)
{
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
}

corto_route x_parser_findRoute_v(
    x_parser this,
    corto_object instance,
    corto_stringseq pattern,
    corto_any param,
    corto_any *routerData)
{
    // Uncomment this line to switch to legacy lookup of routes (slow)
    // return corto_routerimpl_findRoute_v(this, pattern, param, routerData);

    corto_log_push("x");

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
                corto_invokeb(corto_function(callback), NULL, args);
            }

            /* If visitor implements matched, call it for every rule */
            callback = corto_interface_resolveMethod(
                corto_typeof(visitor), "_matched");
            if (callback) {
                void *args[3];
                args[0] = &visitor;
                args[1] = instance,
                args[2] = &routerData->value;
                corto_invokeb(corto_function(callback), NULL, args);
            }

            result = NULL;
        }
    }

    corto_log_pop();
    return result;
error:
    corto_log_pop();
    return NULL;
}

int32_t x_parser_matchRoute_v(
    x_parser this,
    corto_route route,
    corto_stringseq pattern,
    corto_any param,
    corto_any *routerData)
{
    x_rule rule = x_rule(route);
    regex_t *regex = (regex_t*)rule->compiledRegex;
    int ret = regex ? -1 : 0;

    if (regex) {
        regmatch_t *match = alloca(sizeof(regmatch_t) * (regex->re_nsub + 1));

        ret = regexec(regex, pattern.buffer[0], (size_t)regex->re_nsub + 1, match, 0);

        if (!ret) {
            corto_object result = corto_declare(NULL, NULL, rule->pattern);

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
                    corto_throw("invalid parameter '%s': %s", p->name, corto_lasterr());
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
            corto_throw("x: error matching regex");
        }
    }

    return !ret ? 0 : -1;
error:
    return -1;
}
