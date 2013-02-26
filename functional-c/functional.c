/*
 * functional.c by Marco Hinz <mh.codebro@gmail.com>
 * 
 * This is my playground for all sorts of typical functional
 * programming constructs.
 *
 * It is meant for demonstration purposes, hence I don't care
 * for memory leaks.
 */

//  Declarations  {{{1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct list {
    void        *value;
    struct list *next;
} List;

typedef struct {
    void *(*func)(List *);
    List *env;
} Closure;

typedef struct {
    void   *value;
    size_t  size;
} Environment;

typedef void *(*vpfunc)(void *, void *);
typedef void  (*vfunc) (void *, void *);
typedef bool  (*bfunc) (void *, void *);

static void *call(Closure *, Environment *);

//  Data structure handling  {{{1

static List *
list_new_node(void *value)
{
    List *new;

    if ((new = malloc(sizeof *new)) && value) {
        new->value = value;
        new->next  = NULL;
    }

    return new;
}

static List *
list_append(List *list, void *value)
{
    List *current = list;
    List *new     = list_new_node(value);

    if (!list)
        return new;

    while (current->next)
        current = current->next;

    current->next = new;

    return list;
}

/*
 * Make a shallow copy.
 */
static List *
list_copy(List *current)
{
    List *new = NULL;

    while (current) {
        new = list_append(new, current->value);
        current = current->next;
    }

    return new;
}

//  Typical functional constructs  {{{1

static List *
range(unsigned start, unsigned end)
{
    List *new = NULL;

    for (; start <= end; ++start) {
        int *number = malloc(sizeof *number);
        *number = start;
        new = list_append(new, number);
    }

    return new;
}

static void
iter(List *current, vfunc func, void *argument)
{
    while (current) {
        func(current->value, argument);
        current = current->next;
    }
}

static List *
filter(List *current, bfunc func, void *argument)
{
    List *new = NULL;

    while (current) {
        if (func(current->value, argument))
            new = list_append(new, current->value);
        current = current->next;
    }

    return new;
}

static List *
map(List *current, vpfunc func, void *argument)
{
    List *new = NULL;

    while (current) {
        new = list_append(new, func(current->value, argument));
        current = current->next;
    }

    return new;
}

static List *
lmap(List *list, Closure *clo)
{
    List *new = NULL;

    while (list) {
        new  = list_append(new, call(clo, (Environment *)list->value));
        list = list->next;
    }

    return new;
}

//  Closures & Environments  {{{1

static Environment *
env_item(void *value, size_t size)
{
    Environment *env;

    if ((env = malloc(size))) {
        env->value = value;
        env->size  = size;
    }

    return env;
}

static void *
unbox(List *list)
{
    Environment *env = list->value;

    return env->value;
}

static Environment *
lift_int(int integer)
{
    Environment *env = NULL;
    int         *value;

    if ((value = malloc(sizeof *value))) {
        *value = integer;
         env   = env_item((void *)value, sizeof *value);
    }

    return env;
}

static List *
lift_list(List *current, size_t size)
{
    List *new = NULL;

    while (current) {
        Environment *env = env_item(current->value, size);
        new              = list_append(new, env);
        current          = current->next;
    }

    return new;
}

static Closure *
bind(Closure *clo, void *(*func)(List *), Environment *env)
{
    Closure *c;

    if (!clo) {
        if ((c = malloc(sizeof *c))) {
            c->env  = NULL;
            c->func = func;
        }
    }
    else
        c = clo;

    c->env = list_append(c->env, env);

    return c;
}

static void *
call(Closure *clo, Environment *env)
{
    List *list = list_copy(clo->env);
    list       = list_append(list, env);

    return clo->func(list);
}

//  Helper functions  {{{1

static void
print_int(void *value, void *argument)
{
    (void)argument;

    printf("%d\n", *(int *)value);
}

static void * __attribute__((const))
add(void *value, void *argument)
{
    int *ip;

    if ((ip = malloc(sizeof *ip))) {
        *ip = *(int *)value + *(int *)argument;
        return ip;
    }

    return value;
}

static void *
add_list(List *list)
{
    int *a = unbox(list);
    int *b = unbox(list->next);
    int *new;

    if ((new = malloc(sizeof *new)))
        *new = *a + *b;

    return new;
}

static bool
odd(const void *value, void *argument)
{
    (void)argument;

    return (*(int *)value % 2) ? true : false;
}

//  Main  {{{1

int
main(void)
{
    iter(map(range(5, 10), add, &(int){ 100 }), print_int, NULL);
    iter(filter(range(0, 20), odd, NULL), print_int, NULL);

    Closure *add_five = bind(NULL, add_list, lift_int(5));
    printf("%d\n", *(int *)call(add_five, lift_int(36)));

    iter(lmap(lift_list(range(0, 20), sizeof (int)), add_five), print_int, NULL);
}
