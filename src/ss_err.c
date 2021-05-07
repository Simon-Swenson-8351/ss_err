#include "ss_err.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#include <stdio.h>
#include <inttypes.h>

const char *const SS_ERR_LVL_STRS[SS_ERR_LVL_COUNT] = {
    "warning",
    "error"
};

SS_ERR_TYPE_INIT(SS_ERR_TYPE_ALLOC_FAILED, 1, SS_ERR_LVL_ERR, "memory allocation failed");
SS_ERR_TYPE_INIT(SS_ERR_TYPE_DOMAIN, 2, SS_ERR_LVL_ERR, "input outside of function domain");
SS_ERR_TYPE_INIT(SS_ERR_TYPE_FILE_IO, 4, SS_ERR_LVL_ERR, "file I/O error");
SS_ERR_TYPE_INIT(SS_ERR_TYPE_FILE_FORMAT_WRONG, 3, SS_ERR_LVL_ERR, "file format different from expected");

void ss_err_set_str(ss_err_t *self) {
    // Desired string:
    // <level str>: <name> (<code>) in <filename>:<function>:<line>. <descr>. <site_info>.
    //
    // could probably do something with strcat to save some lines of code here, 
    // but strcat/strncat would risk a buffer overflow of the destination 
    // string.
    if(self->site_info == NULL || self->site_info[0] != '\0') {
        // full error string
        snprintf(
            self->str,
            SS_ERR_STR_LEN,
            "%s: %s (%"PRIu32") in %s:%s:%d. %s. %s.",
            SS_ERR_LVL_STRS[self->type->level],
            self->type->name,
            self->type->code,
            self->file,
            self->function,
            self->line,
            self->type->descr,
            self->site_info
        );
    } else {
        // no site info, so omit it.
        snprintf(
            self->str,
            SS_ERR_STR_LEN,
            "%s: %s (%"PRIu32") in %s:%s:%d. %s.",
            SS_ERR_LVL_STRS[self->type->level],
            self->type->name,
            self->type->code,
            self->file,
            self->function,
            self->line,
            self->type->descr
        );
    }
}

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus