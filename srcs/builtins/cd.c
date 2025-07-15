/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brbaazi <brbaazi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:18:05 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/15 13:18:05 by brbaazi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
		s1++, s2++;
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	*get_cd_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!cmd->args[1] || (ft_strcmp(cmd->args[1], "--") == 0)
		|| (ft_strcmp(cmd->args[1], "~") == 0))
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
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putendl_fd("cd: too many arguments", STDERR);
		shell->exit_status = 1;
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
		oldpwd = ft_strdup(cwd);
	path = get_cd_path(shell, cmd);
	if (!path)
		return (1);
	if (chdir(path) != 0)
		return (handle_chdir_error(path, shell, oldpwd));
	if (!get_env_value(shell->env_list, "OLDPWD"))
		add_env(&shell->env_list, "OLDPWD", oldpwd);
	else
		set_env_value(shell->env_list, "OLDPWD", oldpwd);
	free(oldpwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_value(shell->env_list, "PWD", cwd);
	shell->exit_status = 0;
	return (0);
}
