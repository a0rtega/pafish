
#ifndef COMM_H
#define COMM_H

void init_cmd_colors();

void print_header();

void print_traced();

void print_not_traced();

void print_suspicious();

void write_log(char msg[]);

void write_trace(char product[]);

#endif
