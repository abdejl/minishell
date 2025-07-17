#include "minishell.h"

static t_mem_node *g_mem_list = NULL;

void gc_free_all(void)
{
    t_mem_node  *current;
    t_mem_node  *tmp;

    current = g_mem_list;
    while (current)
    {
        tmp = current->next;
        free(current->ptr);
        free(current);
        current = tmp;
    }
    g_mem_list = NULL;
}

void *gc_malloc(size_t size)
{
    void        *ptr;
    t_mem_node  *new_node;

    ptr = malloc(size);
    if (!ptr)
    {
        // On malloc failure, free everything allocated so far and exit.
        // This makes error handling much simpler.
        gc_free_all();
        // You might want to print an error message here
        exit(EXIT_FAILURE);
    }
    new_node = malloc(sizeof(t_mem_node));
    if (!new_node)
    {
        free(ptr);
        gc_free_all();
        exit(EXIT_FAILURE);
    }
    new_node->ptr = ptr;
    new_node->next = g_mem_list;
    g_mem_list = new_node;
    return (ptr);
}
