#include "PlatformCompat.h"



int _stricmp(const char* a, const char* b) {
    return strcmp(a, b);
    /* TODO return strcasecmp(a, b); */
}
int _wcsicmp(const wchar_t* a, const wchar_t* b) {
    return wcscmp(a, b);
    /* TODO return wcscasecmp(a, b); */
}
