/* Copyright 2000-2004 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INHERIT_H
#define INHERIT_H

#include "apr_inherit.h"

#define APR_INHERIT (1 << 24)    /* Must not conflict with other bits */

#define APR_IMPLEMENT_INHERIT_SET(name, flag, pool, cleanup)        \
APR_DECLARE(apr_status_t) apr_##name##_inherit_set(apr_##name##_t *the##name) \
{                                                                   \
    IF_WIN_OS_IS_UNICODE                                            \
    {                                                               \
        if (!SetHandleInformation(the##name->filehand,              \
                                  HANDLE_FLAG_INHERIT,              \
                                  HANDLE_FLAG_INHERIT))             \
            return apr_get_os_error();                              \
    }                                                               \
    ELSE_WIN_OS_IS_ANSI                                             \
    {                                                               \
        HANDLE temp, hproc = GetCurrentProcess();                   \
        if (!DuplicateHandle(hproc, the##name->filehand,            \
                             hproc, &temp, 0, TRUE,                 \
                             DUPLICATE_SAME_ACCESS))                \
            return apr_get_os_error();                              \
        CloseHandle(the##name->filehand);                           \
        the##name->filehand = temp;                                 \
    }                                                               \
    return APR_SUCCESS;                                             \
}                                                                   \
/* Deprecated */                                                    \
APR_DECLARE(void) apr_##name##_set_inherit(apr_##name##_t *the##name) \
{                                                                   \
    apr_##name##_inherit_set(the##name);                            \
}

#define APR_IMPLEMENT_INHERIT_UNSET(name, flag, pool, cleanup)      \
APR_DECLARE(apr_status_t) apr_##name##_inherit_unset(apr_##name##_t *the##name)\
{                                                                   \
    IF_WIN_OS_IS_UNICODE                                            \
    {                                                               \
        if (!SetHandleInformation(the##name->filehand,              \
                                  HANDLE_FLAG_INHERIT, 0))          \
            return apr_get_os_error();                              \
    }                                                               \
    ELSE_WIN_OS_IS_ANSI                                             \
    {                                                               \
        HANDLE temp, hproc = GetCurrentProcess();                   \
        if (!DuplicateHandle(hproc, the##name->filehand,            \
                             hproc, &temp, 0, FALSE,                \
                             DUPLICATE_SAME_ACCESS))                \
            return apr_get_os_error();                              \
        CloseHandle(the##name->filehand);                           \
        the##name->filehand = temp;                                 \
    }                                                               \
    return APR_SUCCESS;                                             \
}                                                                   \
/* Deprecated */                                                    \
APR_DECLARE(void) apr_##name##_unset_inherit(apr_##name##_t *the##name) \
{                                                                   \
    apr_##name##_inherit_unset(the##name);                          \
}

#endif	/* ! INHERIT_H */