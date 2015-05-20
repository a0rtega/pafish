
#ifndef UTILS_H
#define UTILS_H

int pafish_disable_wow64_fs_redirection(void * old);

int pafish_revert_wow64_fs_redirection(void * old);

int pafish_iswow64();

inline int pafish_exists_regkey(HKEY hKey, char * regkey);

inline int pafish_exists_regkey_value_str(HKEY, char *, char *, char *);

inline int pafish_exists_file(char * filename);

#endif
