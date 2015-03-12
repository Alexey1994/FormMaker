#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

void warning(const char *warning_description);
void file_not_found(const char *file_name);
void contents_error(const char *file_name);
void warning_divider();

void error(const char *error_description);

#endif // ERROR_H_INCLUDED
