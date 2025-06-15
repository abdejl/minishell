#include "minishell.h"

static void	free_env_node(t_env *node)
{
	free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

static void	print_invalid_identifier(char *key)
{
	ft_putstr_fd("unset: `", 2);
	ft_putstr_fd(key, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

void	ft_unset(t_env **env_list, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	if (!is_valid_identifier(key))
	{
		print_invalid_identifier(key);
		return ;
	}
	tmp = *env_list;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev == NULL)
				*env_list = tmp->next;
			else
				prev->next = tmp->next;
			free_env_node(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}