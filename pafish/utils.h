
#ifndef UTILS_H
#define UTILS_H

inline int pafish_exists_regkey(HKEY hKey, char * regkey);

inline int pafish_exists_regkey_value_str(HKEY, char *, char *, char *);

inline int pafish_exists_file(char * filename);

#endif
