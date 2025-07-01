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

char *strjoin_free(char *s1, char *s2)
{
    char *result = ft_strjoin(s1, s2);
    free(s1);
    return (result);
}

void free_str_array(char **array)
{
    int i = 0;
    if (!array)
        return;
    while (array[i])
        free(array[i++]);
    free(array);
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