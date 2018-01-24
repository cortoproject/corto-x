/* This is a managed file. Do not delete this comment. */

#include <corto/x/x.h>

char* x_pattern_parseElement(x_pattern this, char *str, corto_buffer *regex, corto_object scope) {
    char *ptr = str, ch;
    corto_bool implicitName = FALSE;
    corto_id id1, id2;
    char *idPtr = id1;
    char *elementName = NULL, *tokenName = NULL, *regexLiteral;
    int regexLiteralCount = 0;
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
            /* ':something:' */
            corto_throw("invalid element expression");
            goto error;
        }

        /* parsed identifier is element name, parse token */
        idPtr = id2;
        ptr ++;

        if (ptr[0] == '(') {
            ptr ++;
            regexLiteral = ptr;
            for (; (ch = *ptr) && (ch != ')'); ptr ++) {
                if (ch == '(') {
                    corto_throw("unexpected '(' in regex literal");
                    goto error;
                }
                regexLiteralCount++;
            }
            if (ch != ')') {
                corto_throw("expected ')' after '('");
                goto error;
            }
            ptr ++;
            if (ptr[0] != '}') {
                corto_throw("expected ')}' after '{('");
                goto error;
            }
        }

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
        corto_throw("invalid element expression");
        goto error;
    }

    corto_object token = NULL;
    if (!regexLiteralCount) {
        /* parsed identifier is a token/pattern */
        token = corto_lookup(scope, tokenName);
        if (!token) {
            corto_throw("unresolved token/pattern '%s'", tokenName);
            goto error;
        }

        if (corto_instanceof(x_pattern_o, token)) {
            corto_buffer_appendstr(regex, x_pattern(token)->regex);
            elementType = x_pattern(token)->type;
        } else if (corto_instanceof(x_token_o, token)) {
            corto_buffer_append(regex, "(%s)", x_token(token)->regex);
            elementType = x_token(token)->type;
        } else {
            corto_throw(
                "identifier '%s' does not resolve to pattern or token (type is '%s')",
                tokenName, corto_fullpath(NULL, corto_typeof(token)));
            goto error;
        }
    } else {
        if (elementName) corto_buffer_appendstr(regex, "(");
        corto_buffer_appendstrn(regex, regexLiteral, regexLiteralCount);
        if (elementName) corto_buffer_appendstr(regex, ")");
        elementType = corto_type(corto_string_o);
    }

    if (elementName) {
        /* Create member of element type in pattern */
        corto_member m = corto_declare(this, elementName, corto_member_o);
        corto_set_ref(&m->type, elementType);
        if (corto_define(m)) {
            goto error;
        }

        /* The parameter list determines how matched subexpressions should be
         * copied into an instance of this pattern */
        if (elementType->kind == CORTO_PRIMITIVE) {
            x_pattern_parameter *p = corto_ptr_new(x_pattern_parameter_o);
            corto_set_str(&p->name, elementName);
            corto_set_ref(&p->type, elementType);
            corto_ll_append(this->params, p);
        } else if (elementType->kind == CORTO_COMPOSITE) {
            /* Copy members from nested pattern, prefix with elementName */
            corto_id name;
            corto_iter it = corto_ll_iter(x_pattern(token)->params);
            while (corto_iter_hasNext(&it)) {
                x_pattern_parameter *p = corto_iter_next(&it);
                if (p->name) {
                    sprintf(name, "%s.%s", elementName, p->name);
                }
                x_pattern_parameter *newParam = corto_ptr_new(x_pattern_parameter_o);
                corto_set_str(&newParam->name, p->name ? name : NULL);
                corto_set_ref(&newParam->type, p->type);
                corto_ll_append(this->params, newParam);
            }
        }
    } else {
        x_pattern_parameter *p = corto_ptr_new(x_pattern_parameter_o);
        corto_set_str(&p->name, NULL);
        corto_set_ref(&p->type, elementType);
        corto_ll_append(this->params, p);
    }

    if (token) {
        corto_ll_insert(this->deps, token);
        corto_release(token);
    }

    return ptr;
error:
    return NULL;
}

int16_t x_pattern_construct(
    x_pattern this)
{
    corto_buffer regex = CORTO_BUFFER_INIT;
    char *expr = corto_strdup(this->expr);
    if (!this->scope) {
        corto_set_ref(&this->scope, corto_parentof(this));
    }

    char *ptr, ch;
    for (ptr = expr; (ch = *ptr); ptr ++) {
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
                corto_throw("error parsing '%s'", this->expr);
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
            if (ptr[1] == '{') {
                corto_buffer_appendstr(&regex, "\\{");
                ptr ++;
            } else if (ptr[1] == '}') {
                corto_buffer_appendstr(&regex, "\\}");
                ptr ++;
            } else if (ptr[1] == '\\') {
                corto_buffer_appendstr(&regex, "\\\\");
                ptr ++;
            } else {
                corto_throw("invalid escape sequence '\\%c'", ptr[1]);
                goto error;
            }
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
        case '/':
            corto_buffer_appendstr(&regex, "\\/");
            break;
        default:
            corto_buffer_append(&regex, "%c", ch);
            break;
        }
    }

    this->regex = corto_buffer_str(&regex);

    free (expr);

    return corto_struct_construct(this);
error:
    return -1;
}

int16_t x_pattern_init(
    x_pattern this)
{
    corto_set_ref(&this->type, this);
    return corto_struct_init(this);
}
