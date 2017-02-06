
#ifndef COMM_H
#define COMM_H

unsigned short init_cmd_colors();

void restore_cmd_colors(unsigned short);

void print_header();

void print_failed();

void print_ok();

int init_socket();

void write_log(wchar_t *);

void send_log(wchar_t *);

int init_socket();

void clean_socket();

#endif
