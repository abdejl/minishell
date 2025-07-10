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
	ft_putstr_fd("unset: `", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}
static void	remove_env_var(t_shell *shell, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = shell->env_list;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				shell->env_list = tmp->next;
			free_env_node(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
int	ft_unset(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			print_invalid_identifier(cmd->args[i]);
			shell->exit_status = 1;
		}
		else
			remove_env_var(shell, cmd->args[i]);
		i++;
	}
	return (0);
}
