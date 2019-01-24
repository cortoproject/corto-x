/* x.h
 * This is the main package file. Include this file in other projects.
 * Only modify inside the header-end and body-end sections.
 */

#ifndef CORTO_X_H
#define CORTO_X_H

#include "bake_config.h"

#define CORTO_X_ETC ut_locate("corto.x", NULL, UT_LOCATE_ETC)

/* $header() */
/* Enter additional code here. */
/* $end */

#include "_type.h"
#include "_interface.h"
#include "_load.h"
#include "_binding.h"

#include <corto.x.c>

/* $body() */
/* Enter code that requires types here */
/* $end */

#endif

