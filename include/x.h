/* x.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef CORTO_X_H
#define CORTO_X_H

#include <corto/corto.h>
#include <corto/x/_project.h>
#include <corto/core/c/c.h>
#include <corto/lang/c/c.h>

#include <corto/x/_type.h>
#include <corto/x/_api.h>
#include <corto/x/_load.h>

#ifdef __cplusplus
extern "C" {
#endif


CORTO_X_EXPORT corto_int16 _x_parseFile(
    corto_object instance,
    corto_string file,
    corto_object visitor);
#define x_parseFile(instance, file, visitor) _x_parseFile(instance, file, visitor)
#include <corto/x/callback.h>
#include <corto/x/parser.h>
#include <corto/x/pattern.h>
#include <corto/x/pattern_parameter.h>
#include <corto/x/rule.h>
#include <corto/x/token.h>
#include <corto/x/visitor.h>

#ifdef __cplusplus
}
#endif
#endif

