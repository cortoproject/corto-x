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

    corto_id buffer;
    char *line;
    corto_object *obj;
    corto_any param = {NULL}, result = {corto_object_o, &obj};
    while ((line = corto_fileReadLine((corto_file)f, buffer, sizeof(buffer)))) {
        if (corto_router_match(instance, line, param, result)) {
            corto_lasterr(); // Suppress uncatched error warning
        }
    }

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
