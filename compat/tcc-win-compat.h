/* Déclarations Windows absentes des en-têtes de tcc, pour compiler LibreSSL.
 * N'inclut ni stdio.h ni errno.h : ils passeraient par include/compat de
 * LibreSSL avant que posix_win.c ait défini NO_REDEF_POSIX_FUNCTIONS, et
 * posix_fopen s'appellerait lui-même. */
#ifndef TCC_WIN_COMPAT_H
#define TCC_WIN_COMPAT_H
#ifndef ECONNREFUSED
#define EADDRINUSE 100
#define EADDRNOTAVAIL 101
#define EAFNOSUPPORT 102
#define EALREADY 103
#define ECONNABORTED 106
#define ECONNREFUSED 107
#define ECONNRESET 108
#define EHOSTUNREACH 110
#define EINPROGRESS 112
#define EISCONN 113
#define ENETDOWN 116
#define ENETRESET 117
#define ENETUNREACH 118
#define ENOBUFS 119
#define ENOTCONN 126
#define ENOTSOCK 128
#define EOPNOTSUPP 130
#define ETIMEDOUT 138
#define ENOPROTOOPT 123
#define EPROTONOSUPPORT 135
#define EPROTOTYPE 136
#define EMSGSIZE 115
#define EDESTADDRREQ 109
#define ENETUNREACH_ 118
#define ENOTSUP 129
#define EOVERFLOW 132
#define EWOULDBLOCK 140
#endif
#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif
#include <winsock2.h>
#include <windows.h>
#ifndef _UID_T_DEFINED
#define _UID_T_DEFINED
typedef int uid_t;
typedef int gid_t;
#endif
#ifndef INIT_ONCE_STATIC_INIT
typedef union _RTL_RUN_ONCE { PVOID Ptr; } RTL_RUN_ONCE, *PRTL_RUN_ONCE;
typedef RTL_RUN_ONCE INIT_ONCE, *PINIT_ONCE, *LPINIT_ONCE;
#define INIT_ONCE_STATIC_INIT {0}
typedef BOOL (WINAPI *PINIT_ONCE_FN)(PINIT_ONCE, PVOID, PVOID *);
WINBASEAPI BOOL WINAPI InitOnceExecuteOnce(PINIT_ONCE, PINIT_ONCE_FN, PVOID, LPVOID *);
#endif
#endif
