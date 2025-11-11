#ifndef SYMTAB_H
#define SYMTAB_H

/* simple symbol table (string -> int) */
typedef struct Sym {
    char *name;
    int val;
    struct Sym *next;
} Sym;

void set_var(const char *name, int val);
int get_var(const char *name);


#endif // SYMTAB_H