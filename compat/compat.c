/* Fonctions POSIX absentes du msvcrt de tcc, pour LibreSSL et Lynx. */
#include <winsock2.h>
#include <windows.h>
#include <stdint.h>

int gettimeofday(struct timeval *tp, void *tzp)
{
	FILETIME ft;
	uint64_t t;
	(void)tzp;
	GetSystemTimeAsFileTime(&ft);
	t = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	t -= 116444736000000000ULL;	/* 1601 -> 1970, en centaines de ns */
	tp->tv_sec = (long)(t / 10000000ULL);
	tp->tv_usec = (long)((t % 10000000ULL) / 10);
	return 0;
}

/* Intrinsèques de MSVC/mingw utilisés par les en-têtes de Windows. */
long _InterlockedExchangeAdd(long volatile *p, long v)
{
	__asm__ __volatile__("lock; xaddl %0, %1" : "+r"(v), "+m"(*p) : : "memory");
	return v;
}

void __stosb(unsigned char *d, unsigned char c, size_t n)
{
	while (n--)
		*d++ = c;
}

/* syslog : sans objet sous Windows, messages sur la sortie d'erreur. */
#include <stdio.h>
#include <stdarg.h>
void vsyslog(int pri, const char *fmt, va_list ap)
{
	(void)pri;
	vfprintf(stderr, fmt, ap);
	fputc('\n', stderr);
}

/* opendir/readdir/closedir minimaux (noms en ANSI). */
#include <stdlib.h>
#include <string.h>
struct dirent { char d_name[MAX_PATH]; };
typedef struct { HANDLE h; WIN32_FIND_DATAA fd; int premier; struct dirent e; } DIR;

DIR *opendir(const char *nom)
{
	char motif[MAX_PATH + 3];
	DIR *d;
	if (strlen(nom) > MAX_PATH)
		return NULL;
	snprintf(motif, sizeof motif, "%s\\*", nom);
	if ((d = calloc(1, sizeof *d)) == NULL)
		return NULL;
	d->h = FindFirstFileA(motif, &d->fd);
	if (d->h == INVALID_HANDLE_VALUE) {
		free(d);
		return NULL;
	}
	d->premier = 1;
	return d;
}

struct dirent *readdir(DIR *d)
{
	if (!d->premier && !FindNextFileA(d->h, &d->fd))
		return NULL;
	d->premier = 0;
	strncpy(d->e.d_name, d->fd.cFileName, MAX_PATH - 1);
	return &d->e;
}

int closedir(DIR *d)
{
	FindClose(d->h);
	free(d);
	return 0;
}
