#include "minishell.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

t_env *new_env_node(char *key, char *value)
{
	t_env *node = malloc(sizeof(t_env));
	if (!node)
		return NULL;
	node->key = ft_strdup(key);
	node->value = value ? ft_strdup(value) : NULL;
	node->next = NULL;
	return node;
}

t_env *init_env_list(char **envp)
{
	t_env *head;
	t_env *current;
	int i;

	head = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		char *entry = envp[i];
		char *equal_sign = ft_strchr(entry, '=');
		if (equal_sign)
		{
			int key_len = equal_sign - entry;
			char *key = ft_strndup(entry, key_len);
			char *value = equal_sign + 1;
			t_env *node = new_env_node(key, value);
			free(key);
			if (!head)
				head = node;
			else
				current->next = node;
			current = node;
		}
		i++;
	}
	return head;
}

int ft_env(t_env *env)
{
    if(!env)
        return(1);
    while(env)
    {
        if(env->value)
            printf("%s=%s\n", env->key , env->value);
        env = env->next ;
    }
    return(0);
}
