#ifndef SS_ERR_H
#define SS_ERR_H

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#include <stdbool.h>
#include <inttypes.h>
#include <pthread.h>

typedef enum ss_err_lvl {
    SS_ERR_LVL_WARN,
    SS_ERR_LVL_ERR,
    // INSERT NEW VALUES ABOVE
    SS_ERR_LVL_COUNT
} ss_err_lvl_t;
extern const char *const SS_ERR_LVL_STRS[SS_ERR_LVL_COUNT];

typedef struct ss_err_type {
    uint32_t code;
    ss_err_lvl_t level;
    const char *name;
    const char *descr;
} ss_err_type_t;

enum { SS_ERR_STR_LEN = 512 };
typedef struct ss_err {
    const ss_err_type_t *type;
    pthread_rwlock_t lock;
    bool initd;
    const char *file;
    const char *function;
    uint32_t line;
    const char *site_info; // any extra info provided at the site the error happened.
                     // for example, a domain error alone might not be clear, 
                     // but this string might contain additional helpful info 
                     // like "buffer length given for 2nd buffer argument was 0"
    char str[SS_ERR_STR_LEN];
} ss_err_t;

extern const ss_err_type_t SS_ERR_TYPE_ALLOC_FAILED;
extern const ss_err_type_t SS_ERR_TYPE_DOMAIN;
extern const ss_err_type_t SS_ERR_TYPE_FILE_IO;
extern const ss_err_type_t SS_ERR_TYPE_FILE_FORMAT_WRONG;

void ss_err_set_str(ss_err_t *self);

// macro for initializing an error type
#define SS_ERR_TYPE_INIT(type_var_name, in_code, in_level, in_descr) \
    const ss_err_type_t type_var_name = {\
        .code = in_code,\
        .level = in_level,\
        .name = #type_var_name ,\
        .descr = in_descr\
    }

// Needs to be a macro to obtain proper values for __FILE__, __FUNCTION__, and 
// __LINE__
#define SS_ERR_VARNAME2(id) ss_err_ ## id
#define SS_ERR_VARNAME(id) SS_ERR_VARNAME2(id)
#define SS_ERR_COMMON(ret_or_set_stmt, type_ptr, f, fn, l, si)\
    do {\
        static ss_err_t SS_ERR_VARNAME(l) = {\
            .type = type_ptr,\
            .lock = PTHREAD_RWLOCK_INITIALIZER,\
            .initd = false,\
            .file = f,\
            .function = fn,\
            .line = l,\
            .site_info = si\
        };\
        pthread_rwlock_rdlock(&(SS_ERR_VARNAME(l).lock));\
        if(!SS_ERR_VARNAME(l).initd) {\
            pthread_rwlock_unlock(&(SS_ERR_VARNAME(l).lock));\
            pthread_rwlock_wrlock(&(SS_ERR_VARNAME(l).lock));\
            if(!SS_ERR_VARNAME(l).initd) {\
                ss_err_set_str(&SS_ERR_VARNAME(l));\
                SS_ERR_VARNAME(l).initd = true;\
            }\
        }\
        pthread_rwlock_unlock(&(SS_ERR_VARNAME(l).lock));\
        ret_or_set_stmt;\
    } while(0);
#define SS_ERR_RETURN_STMT(id)\
    do {\
        return &SS_ERR_VARNAME(id);\
    } while(0);
#define SS_ERR_SET_STMT(err_var, id)\
    do {\
        err_var = &SS_ERR_VARNAME(id);\
    } while(0);
#define SS_ERR_RETURN(type_ptr, file, fn, line, site_info)\
    SS_ERR_COMMON(SS_ERR_RETURN_STMT(line), type_ptr, file, fn, line, site_info)
#define SS_ERR_SET(err_var, type_ptr, file, fn, line, site_info)\
    SS_ERR_COMMON(SS_ERR_SET_STMT(err_var, line), type_ptr, file, fn, line, site_info)


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef SS_ERR_H