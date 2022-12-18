#ifndef arba_INTERNAL_H
#define arba_INTERNAL_H

/* private data types go here */
/* These headers do not get installed and will not be available 
 * in the final "dev" package header installation. #include "internal.h"
 * helps protect the namespace of the final environment so it is not
 * overly polluted with library helper functions. the C standard
 * dictates that __function is reserved for the standard library impl
 * so a typical library developer will use _function. 
 */

#endif
