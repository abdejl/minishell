/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:17:50 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/20 10:23:56 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_and_return(t_env *env_list)
{
	print_exported_env(env_list);
	return (0);
}

static int	handle_invalid(t_shell *shell, char *key, char *value, char *arg)
{
	handle_invalid_key(arg, shell);
	free(key);
	if (value)
		free(value);
	return (1);
}

static void	update_or_add(t_shell *shell, t_env *existing, char *key,
		char *value)
{
	if (existing)
	{
		if (value)
		{
			free(existing->value);
			existing->value = ft_strdup(value);
			if (!existing->value)
				return ;
		}
	}
	else
		add_env(&shell->env_list, key, value);
}

static int	handle_argument(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	t_env	*existing;

	split_key_value(arg, &key, &value);
	if (!is_valid_identifier(key))
		return (handle_invalid(shell, key, value, arg));
	existing = get_env_node(shell->env_list, key);
	update_or_add(shell, existing, key, value);
	free(key);
	if (value)
		free(value);
	return (0);
}

int	ft_export(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!cmd->args[1])
		return (print_and_return(shell->env_list));
	shell->exit_status = 0;
	while (cmd->args[i])
	{
		if (handle_argument(shell, cmd->args[i]) == 1)
			break ;
		i++;
	}
	return (shell->exit_status);
}
