/* $CORTO_GENERATED
 *
 * pattern.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/x/x.h>

/* $header(corto/x/pattern/construct) */
char* x_pattern_parseElement(x_pattern this, char *str, corto_buffer *regex, corto_object scope) {
    char *ptr = str, ch;
    corto_bool implicitName = FALSE;
    corto_id id1, id2;
    char *idPtr = id1;
    char *elementName = NULL, *tokenName = NULL;
    corto_type elementType = corto_type(corto_string_o);

    if (ptr[0] == ':') {
        /* Derive name from name of referenced pattern/token */
        implicitName = TRUE;
        ptr ++;
    }

    for (; (ch = *ptr) && (ch != ':') && (ch != '}'); ptr++) {
        *idPtr = ch;
        idPtr++;
    }
    *idPtr = '\0';

    if (ch == ':') {
        if (implicitName) {
            corto_seterr("invalid element expression");
            goto error;
        }

        /* parsed identifier is element name, parse token */
        idPtr = id2;
        ptr ++;
        for (; (ch = *ptr) && (ch != ':') && (ch != '}'); ptr++) {
            *idPtr = ch;
            idPtr++;
        }
        *idPtr = '\0';

        elementName = id1;
        tokenName = id2;

    } else if (ch == '}') {
        if (implicitName) {
            elementName = id1;
        } else {
            elementName = NULL;
        }
        tokenName = id1;

    } else if (!ch) {
        corto_seterr("invalid element expression");
        goto error;
    }

    /* parsed identifier is a token/pattern */
    corto_object token = corto_lookup(scope, tokenName);
    if (!token) {
        corto_seterr("unresolved token/pattern '%s'", tokenName);
        goto error;
    }

    if (corto_instanceof(x_pattern_o, token)) {
        corto_buffer_appendstr(regex, x_pattern(token)->regex);
        elementType = x_pattern(token)->type;
    } else if (corto_instanceof(x_token_o, token)) {
        corto_buffer_append(regex, "(%s)", x_token(token)->regex);
        elementType = x_token(token)->type;
    } else {
        corto_seterr(
            "identifier '%s' does not resolve to pattern or token (type is '%s')",
            tokenName, corto_fullpath(NULL, corto_typeof(token)));
        goto error;
    }

    if (elementName) {
        /* Create member of element type in pattern */
        corto_member m = corto_declareChild(this, elementName, corto_member_o);
        corto_setref(&m->type, elementType);
        if (corto_define(m)) {
            goto error;
        }

        /* The parameter list determines how matched subexpressions should be
         * copied into an instance of this pattern */
        if (elementType->kind == CORTO_PRIMITIVE) {
            x_pattern_parameterAssign(
                x_pattern_parameterListAppendAlloc(this->params),
                elementName,
                elementType
            );
        } else if (elementType->kind == CORTO_COMPOSITE) {
            /* Copy members from nested pattern, prefix with elementName */
            corto_id name;
            corto_iter it = corto_llIter(x_pattern(token)->params);
            while (corto_iterHasNext(&it)) {
                x_pattern_parameter *p = corto_iterNext(&it);
                if (p->name) {
                    sprintf(name, "%s.%s", elementName, p->name);
                }
                x_pattern_parameterAssign(
                    x_pattern_parameterListAppendAlloc(this->params),
                    p->name ? name : NULL,
                    p->type
                );
            }
        }
    } else {
        x_pattern_parameterAssign(
            x_pattern_parameterListAppendAlloc(this->params),
            NULL,
            elementType
        );
    }

    corto_llInsert(this->deps, token);
    corto_release(token);

    return ptr;
error:
    return NULL;
}
/* $end */
corto_int16 _x_pattern_construct(
    x_pattern this)
{
/* $begin(corto/x/pattern/construct) */
    corto_buffer regex = CORTO_BUFFER_INIT;

    if (!this->scope) {
        corto_setref(&this->scope, corto_parentof(this));
    }

    char *ptr, ch;
    for (ptr = this->expr; (ch = *ptr); ptr ++) {
        switch (ch) {
        case '.':
            corto_buffer_appendstr(&regex, "\\.");
            break;
        case '[':
            corto_buffer_appendstr(&regex, "\\[");
            break;
        case '{':
            ptr = x_pattern_parseElement(this, ptr + 1, &regex, this->scope);
            if (!ptr) {
                goto error;
            }
            break;
        case '(':
            corto_buffer_appendstr(&regex, "\\(");
            break;
        case ')':
            corto_buffer_appendstr(&regex, "\\)");
            break;
        case '\\':
            break;
        case '*':
            corto_buffer_appendstr(&regex, "\\*");
            break;
        case '+':
            corto_buffer_appendstr(&regex, "\\+");
            break;
        case '?':
            corto_buffer_appendstr(&regex, "\\?");
            break;
        case '|':
            corto_buffer_appendstr(&regex, "\\|");
            break;
        case '^':
            corto_buffer_appendstr(&regex, "\\^");
            break;
        case '$':
            corto_buffer_appendstr(&regex, "\\$");
            break;
        default:
            corto_buffer_append(&regex, "%c", ch);
            break;
        }
    }

    this->regex = corto_buffer_str(&regex);

    return corto_struct_construct(this);
error:
    return -1;
/* $end */
}

corto_int16 _x_pattern_init(
    x_pattern this)
{
/* $begin(corto/x/pattern/init) */
    corto_setref(&this->type, this);
    return corto_struct_init(this);
/* $end */
}
