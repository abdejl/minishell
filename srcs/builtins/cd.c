/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29                              #+#    #+#             */
/*   Updated: 2025/04/29                             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
		s1++, s2++;
	return (unsigned char)*s1 - (unsigned char)*s2;
}

void ft_free_env_list(t_env *env)
{
    t_env *tmp;
    
    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

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
			return (0);
		}
		current = current->next;
	}
	return (1);
}

char	*get_cd_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "--" )== 0)
	{
		path = get_env_value(shell->env_list, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR);
			shell->exit_status = 1;
			return (NULL);
		}
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		path = get_env_value(shell->env_list, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", STDERR);
			shell->exit_status = 1;
			return (NULL);
		}
		ft_putendl_fd(path, STDOUT);
	}
	else
		path = cmd->args[1];
	return (path);
}

static int	handle_chdir_error(char *path, t_shell *shell, char *oldpwd)
{
	ft_putstr_fd("cd: ", STDERR);
	ft_putstr_fd(path, STDERR);
	ft_putstr_fd(": ", STDERR);
	ft_putendl_fd(strerror(errno), STDERR);
	free(oldpwd);
	shell->exit_status = 1;
	return (1);
}

int	cd_builtin(t_shell *shell, t_cmd *cmd)
{
	char	cwd[1024];
	char	*oldpwd;
	char	*path;
	
	oldpwd = NULL;
	if (getcwd(cwd, sizeof(cwd)))
		oldpwd = ft_strdup(cwd);
	path = get_cd_path(shell, cmd);
	if (!path)
		return (1);
	if (chdir(path) != 0)
		return (handle_chdir_error(path, shell, oldpwd));
	if(!get_env_value(shell->env_list , "OLDPWD"))
		add_env(&shell->env_list , "OLDPWD", oldpwd);
	else
		set_env_value(shell->env_list, "OLDPWD", oldpwd);
	free(oldpwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_value(shell->env_list, "PWD", cwd);
	shell->exit_status = 0;
	return (0);
}
