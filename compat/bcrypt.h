/* bcrypt.h minimal pour tcc : seulement ce qu'utilise LibreSSL (getentropy). */
#ifndef TCC_BCRYPT_H
#define TCC_BCRYPT_H
#include <windows.h>
#ifndef NTSTATUS
typedef LONG NTSTATUS;
#endif
typedef PVOID BCRYPT_ALG_HANDLE;
#define BCRYPT_USE_SYSTEM_PREFERRED_RNG 0x00000002
__declspec(dllimport) NTSTATUS WINAPI BCryptGenRandom(BCRYPT_ALG_HANDLE hAlgorithm, PUCHAR pbBuffer, ULONG cbBuffer, ULONG dwFlags);
#endif
