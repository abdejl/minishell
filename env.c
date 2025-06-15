#include "../../includes/minishell.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

// t_env *new_env_node(char *key, char *value)
// {
// 	t_env *node = malloc(sizeof(t_env));
// 	if (!node)
// 		return NULL;
// 	node->key = strdup(key);
// 	node->value = value ? strdup(value) : NULL;
// 	node->next = NULL;
// 	return node;
// }

// t_env *init_env_list(char **envp)
// {
// 	t_env *head = NULL;
// 	t_env *current = NULL;
// 	int i = 0;

// 	while (envp[i])
// 	{
// 		char *entry = envp[i];
// 		char *equal_sign = strchr(entry, '=');
// 		if (equal_sign)
// 		{
// 			int key_len = equal_sign - entry;
// 			char *key = strndup(entry, key_len);
// 			char *value = equal_sign + 1;

// 			t_env *node = new_env_node(key, value);
// 			free(key);

// 			if (!head)
// 				head = node;
// 			else
// 				current->next = node;

// 			current = node;
// 		}
// 		i++;
// 	}
// 	return head;
// }

int ft_env(t_env *env)
{
    while(env)
    {
        if(env->value)
            printf("%s /%s\n", env->key , env->value);
        env = env->next ;
    }
    return(0);
}

// int main(int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	(void)av;

// 	t_env *env_list = init_env_list(envp);

// 	printf("===== ENV TEST =====\n");
// 	ft_env(env_list);

// 	return 0;
// }
