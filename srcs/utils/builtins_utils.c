
#include "minishell.h"
static void	split_key_value_no_equal(char *arg, char **key, char **value)
{
	*key = ft_strdup(arg);
	if (!*key)
	{
		*value = NULL;
		return;
	}
	*value = NULL;
}

void	split_key_value(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		if(!*key)
		{
			*value = NULL;
			return;
		}
		*value = ft_strdup(equal + 1);
		if(!*value)
		{
			free(*key);
			return;
		}
	}
	else
	{
		split_key_value_no_equal(arg, key, value);
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

