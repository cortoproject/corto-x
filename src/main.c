/* This is a managed file. Do not delete this comment. */

#include <corto/x/x.h>

int16_t x_parseFile(
    corto_object instance,
    const char *file,
    corto_object visitor)
{
    FILE *f = fopen(file, "r");
    if (!f) {
        corto_throw("x: could not open input file '%s': %s", file);
        goto error;
    }

    fseek(f, 0L, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    corto_routerimpl parser = corto_routerimpl(corto_typeof(instance));
    corto_objectseq *methods = &corto_interface(parser)->methods;
    corto_int32seq routesCalled;
    routesCalled.buffer = corto_calloc(methods->length * sizeof(corto_int32));
    routesCalled.length = methods->length;
    corto_int32 totalLines = 0, totalMatched = 0;
    corto_time start, stop;
    corto_route route = NULL;
    corto_id buffer;
    char *line;
    corto_int32 bytesRead = 0;
    corto_int32 i, lineCount = 0;
    corto_any param = {NULL}, result = {NULL, NULL};
    if (visitor) {
        param.type = corto_typeof(visitor);
        param.value = visitor;
    }

    printf("x/parser: parsing '%s'\n", file);
    corto_time_get(&start);
    while ((line = corto_file_readln(f, buffer, sizeof(buffer)))) {
        if (corto_router_match(instance, line, param, result, &route)) {

        } else if (route) {
            /* Keep track of how many times a route is called */
            for (i = 0; i < methods->length; i++) {
                if (methods->buffer[i] == route) {
                    routesCalled.buffer[i] ++;
                }
            }
            totalMatched ++;
        }

        totalLines ++;
        bytesRead += strlen(line) + 1;
        if (!(lineCount % 5432)) {
            for (i = 0; i < 80; i++) {
                printf("\b");
            }

            printf("x/parser: %.1f%% (total = %d, matched = %d)",
                100.0 * ((float)bytesRead / (float)size), totalLines, totalMatched);
            fflush(stdout);
        }

        lineCount ++;
    }

    corto_time_get(&stop);
    for (i = 0; i < 80; i++) {
        printf("\b");
    }

    printf("x/parser: 100%% (total = %d, matched = %d)  \n",
        totalLines, totalMatched);
    printf("x/parser: done (%.1f seconds)\n",
        corto_time_toDouble(corto_time_sub(stop, start)));
    corto_trace("");
    corto_trace("x: Summary:");

    for (i = 0; i < routesCalled.length; i++) {
        corto_trace("x: %30s: called %d times",
            corto_idof(methods->buffer[i]),
            routesCalled.buffer[i]);
    }

    corto_trace("");
    corto_trace("x: parsed %d lines, matched %d in %fs",
        totalLines,
        totalMatched,
        corto_time_toDouble(corto_time_sub(stop, start)));
    corto_dealloc(routesCalled.buffer);
    fclose(f); /* Close file handler */
    return 0;
error:
    return -1;
}

int16_t x_parseString(
    corto_object instance,
    const char *line,
    corto_object visitor)
{
    corto_int32 matched = 0;
    corto_time start, stop;
    corto_route route = NULL;
    corto_any param = {NULL}, result = {NULL, NULL};
    if (visitor) {
        param.type = corto_typeof(visitor);
        param.value = visitor;
    }

    /* Benchmarking only enabled when Corto running with TRACE verbosity */
    if (corto_log_verbosityGet() <= CORTO_TRACE) {
        corto_trace("x/parser: parsing [%s]", line);
        corto_time_get(&start);
    }

    if (corto_router_match(instance, line, param, result, &route)) {
        /* corto_router_match failed */
    }
    else if (route) {
        /* Route successfully matched. */
        matched = 1;
    }

    if (matched) {
        if (corto_log_verbosityGet() <= CORTO_TRACE) {
            corto_time_get(&stop);
            corto_trace("");
            corto_trace("Successfully parsed [%s] in [%f]s.\nMatched Route [%s]",
                line,
                corto_time_toDouble(corto_time_sub(stop, start)),
                corto_idof(route));
        }
    } else {
        corto_trace("Failed to parse [%s] - no matching routes\n", line)
        return -1;
    }

    return 0;
}

int cortomain(int argc, char *argv[]) {

    /* Insert implementation */

    return 0;
}
