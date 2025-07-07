#include "minishell.h"

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

void	ft_free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}