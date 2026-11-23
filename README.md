# lynx-windows

Lynx (navigateur texte) pour Windows x86-64, compilé sur deb avec **tcc seulement** : tcc croisé, LibreSSL pour le HTTPS, PDCurses pour la console, zlib pour les pages compressées. Sert à `lynx -dump <url>` sous Windows comme sous Linux (lisibilité d'un site sans CSS ni JavaScript, ce qu'un robot lit).

## Compiler

Sur deb (Debian 13 : `tcc`, `cmake`, `make`, `patch`, `curl`, `rc`, `wak`) :

```bash
./compiler.rc
```

Le script télécharge les sources dans `sources/` (vérifiées par empreinte SHA-256, voir l'en-tête du script), compile dans `construction/` et dépose `lynx.exe`, `lynx.cfg` et `cert.pem` dans `dist/lynx/` (environ 30 s). Aucun de ces trois dossiers n'est versionné.

| Composant | Version | Vérification |
|---|---|---|
| tinycc | commit `fc1fc50` (le même que rcsh et wak) | archive GitHub, empreinte |
| LibreSSL | 4.4.1 | signature GPG de Brent Cook, puis empreinte |
| zlib | 1.4 | signature GPG de Mark Adler, puis empreinte |
| PDCurses | 3.9 | archive GitHub de l'étiquette, empreinte |
| Lynx | 2.9.3 | signature GPG de Thomas E. Dickey (`1988 2D92 DDA4 C400 C22C 0D56 CC2A F447 2167 BE03`), puis empreinte |

## Installer sous Windows

Copier le contenu de `dist/lynx/` dans `C:\outils\lynx` (chemin attendu par `SSL_CERT_FILE` dans `lynx.cfg`) et définir `LYNX_CFG=C:\outils\lynx\lynx.cfg`. Puis :

```
lynx -dump -nolist https://www.glads.fr/
```

Vérifié dans la VM Windows 11 : glads.fr et fr.wikipedia.org s'affichent (UTF-8) ; certificat expiré, autosigné ou au mauvais nom refusé (code de sortie 1).

## Ce que la recette ajoute

- `compat/` : ce qui manque à tcc pour LibreSSL et Lynx. `tcc-win-compat.h` (inclus de force dans LibreSSL : codes d'erreur réseau, `INIT_ONCE`, `uid_t` ; il n'inclut ni `stdio.h` ni `errno.h`, sinon `posix_fopen` de LibreSSL s'appelle lui-même et le programme meurt d'un dépassement de pile), `bcrypt.h` (seulement `BCryptGenRandom`, source d'aléa de LibreSSL), `compat.c` (`gettimeofday`, `opendir`/`readdir`, `vsyslog`, deux intrinsèques de MSVC), en-têtes `alloca.h`, `winsock.h`, `sys/param.h`, et les listes d'imports `bcrypt.def` et `advapi32.def`.
- `lynx-certfile.patch` : avec OpenSSL ou LibreSSL, Lynx ne lit `SSL_CERT_FILE` de `lynx.cfg` que pour GnuTLS ; LibreSSL ne cherche alors que son chemin compilé, inexistant sous Windows, et refuse tous les sites. Le correctif charge ce fichier avec `SSL_CTX_load_verify_locations`.
- `glads.cfg` : ajouté à la fin du `lynx.cfg` d'origine (`SSL_CERT_FILE`, `CHARACTER_SET:utf-8`).
- Lynx est configuré pour mingw (`--host=x86_64-w64-mingw32`) ; tcc ne définissant pas `__MINGW32__`, le script l'ajoute, et retire l'icône (pas de `windres`).

## Limites

- zlib 1.4 compilée avec tcc : pages et fichiers en gzip ou deflate. Pas de bzip2, pas d'IDNA (noms de domaine accentués), pas de traduction (`--disable-nls`).
- `cert.pem` est celui de LibreSSL 4.4.1 : le renouveler en recompilant avec une version plus récente.
- La console de PowerShell affiche mal les accents ; la sortie elle-même est en UTF-8 (redirigée vers un fichier, elle est juste).

## Licences

Lynx : GPL v2. LibreSSL : licences OpenSSL et ISC. PDCurses : domaine public. tinycc : LGPL 2.1. Fichiers de ce dépôt : mêmes conditions que le composant qu'ils complètent.
