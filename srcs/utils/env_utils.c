#include "minishell.h"

char **env_to_array(t_env *env_list)
{
    char **envp;
    char *temp;
    int i = 0;
    t_env *tmp = env_list;
    int size = 0;

    while (tmp && ++size)
        tmp = tmp->next;
    envp = malloc(sizeof(char *) * (size + 1));
    if (!envp)
        return (NULL);
    tmp = env_list;
    while (tmp)
    {
        temp = ft_strjoin(tmp->key, "=");
        envp[i] = ft_strjoin(temp, tmp->value);
        free(temp);
        if (!envp[i]) {
            while (i-- > 0)
                free(envp[i]);
            free(envp);
            return (NULL);
        }
        i++;
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return (envp);
}

t_env *get_env_node(t_env *env_list, char *key)
{
    while (env_list)
    {
        if (ft_strcmp(env_list->key, key) == 0)
            return env_list;
        env_list = env_list->next;
    }
    return NULL;
}

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