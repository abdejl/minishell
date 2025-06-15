#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

/* التحقق من صلاحية المعرف */
int	is_valid_identifier(char *key)
{
	int	i;

	i = 0;
	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* فصل المفتاح والقيمة */
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

/* عرض المتغيرات البيئية المصدرة بتنسيق export */
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

/* إضافة متغير بيئي للقائمة */
void	add_env(t_env **env_list, char *key, char *value)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = strdup(key);
	new->value = value ? strdup(value) : NULL;
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

/* معالجة أمر export */
int	handle_invalid_key(char *key, t_shell *shell)
{
	ft_putstr_fd("minishell: export: `", STDERR);
	ft_putstr_fd(key, STDERR);
	ft_putendl_fd("': not a valid identifier", STDERR);
	shell->exit_status = 1;
	return (0);
}

int	ft_export(t_shell *shell, t_cmd *cmd)
{
	int		i;
	char	*key;
	char	*value;

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
			handle_invalid_key(key, shell);
		else if (get_env_value(shell->env_list, key))
			set_env_value(shell->env_list, key, value);
		else
			add_env(&shell->env_list, key, value);
		free(key);
		if (value)
			free(value);
		i++;
	}
	return (0);
}
