#include "minishell.h"

void	split_key_value(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

void	print_exported_env(t_env *env_list)
{
	while (env_list)
	{
		ft_putstr_fd("declare -x ", STDOUT);
		ft_putstr_fd(env_list->key, STDOUT);
		if (env_list->value)
		{
			ft_putstr_fd("=\"", STDOUT);
			ft_putstr_fd(env_list->value, STDOUT);
			ft_putstr_fd("\"", STDOUT);
		}
		ft_putchar_fd('\n', STDOUT);
		env_list = env_list->next;
	}
}

int is_valid_identifier(char *key)
{
    int i;

    i = 0;
    if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
        return (0);
    if(ft_strchr(key , '='))
        return(0);
    while (key[i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int handle_invalid_key(char *key, t_shell *shell)
{
    ft_putstr_fd("minishell: export: `", STDERR);
    ft_putstr_fd(key, STDERR);
    ft_putendl_fd("': not a valid identifier", STDERR);
    shell->exit_status = 1;
    return (1);
}

int ft_export(t_shell *shell, t_cmd *cmd)
{
    int     i;
    char    *key;
    char    *value;
    t_env   *existing;

    i = 1;
    if (!cmd->args[1])
    {
        print_exported_env(shell->env_list);
        return (0);
    }
    while (cmd->args[i])
    {
        split_key_value(cmd->args[i], &key, &value);
        if (!is_valid_identifier(key))
        {
            handle_invalid_key(cmd->args[i], shell);
            free(key);
            if (value) free(value);
            i++;
            continue;
        }
        existing = get_env_node(shell->env_list, key);
        if (existing)
        {
            if (value)
            {
                free(existing->value);
                existing->value = ft_strdup(value);
            }
        }
        else
            add_env(&shell->env_list, key, value);
        free(key);
        if (value)
            free(value);
        i++;
    }
    return (0);
}
