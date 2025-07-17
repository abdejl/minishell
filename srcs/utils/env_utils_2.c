#include "minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	set_env_value(t_env *env, char *key, char *value)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			if(!current->value)
				return(1);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

void	add_env(t_env **env_list, char *key, char *value)
{
	t_env	*new;
	t_env	*tmp;

	new = gc_malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	if(!new->key)
		return;
	new->value = value ? ft_strdup(value) : NULL;
	if(!new->value)
		return;
	new->next = NULL;
	if (!*env_list)
	{
		*env_list = new;
		return ;
	}
	tmp = *env_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
