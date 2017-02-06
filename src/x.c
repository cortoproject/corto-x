/* $CORTO_GENERATED
 *
 * x.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/x/x.h>

corto_int16 _x_parseFile(
    corto_object instance,
    corto_string file)
{
/* $begin(corto/x/parseFile) */
    FILE *f = fopen(file, "r");
    if (!f) {
        corto_seterr("x: could not open input file '%s': %s", file, corto_lasterr());
        goto error;
    }

    corto_routerimpl parser = corto_routerimpl(corto_typeof(instance));
    corto_objectseq *methods = &corto_interface(parser)->methods;

    corto_int32seq routesCalled;
    routesCalled.buffer = corto_calloc(methods->length * sizeof(corto_int32));
    routesCalled.length = methods->length;

    corto_int32 totalLines = 0, totalMatched = 0;

    corto_route route = NULL;
    corto_id buffer;
    char *line;
    corto_object *obj;
    corto_int32 i;
    corto_any param = {NULL}, result = {corto_object_o, &obj};
    while ((line = corto_fileReadLine((corto_file)f, buffer, sizeof(buffer)))) {
        if (corto_router_match(instance, line, param, result, &route)) {
            corto_lasterr(); /* Suppress uncatched error warning */
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
    }

    corto_trace("");
    corto_trace("x: Summary:");
    for (i = 0; i < routesCalled.length; i++) {
        corto_trace("x: %30s: called %d times", 
            corto_idof(methods->buffer[i]), 
            routesCalled.buffer[i]);
    }
    corto_trace("");
    corto_trace("x: parsed %d lines, matched %d", totalLines, totalMatched);

    corto_dealloc(routesCalled.buffer);

    return 0;
error:
    return -1;
/* $end */
}

int xMain(int argc, char *argv[]) {
/* $begin(main) */

    /* Insert implementation */

    return 0;
/* $end */
}
