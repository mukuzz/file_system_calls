#ifndef FILE_H
#define FILE_H

void create_norm_file(const char *path, int modes);
void create_pipe(const char *path, int modes);
void read_file(const char *path, int offset, int bytes);
void write_file(const char *path, int offset, int bytes);
void file_stat(const char *path);

#endif