/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brbaazi <brbaazi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:16:22 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/16 09:24:07 by brbaazi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_oldpwd_or_error(t_shell *shell)
{
	char	*path;

	path = get_env_value(shell->env_list, "OLDPWD");
	if (!path)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", STDERR);
		shell->exit_status = 1;
		return (NULL);
	}
	ft_putendl_fd(path, STDOUT);
	return (path);
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
		path = get_oldpwd_or_error(shell);
		if (!path)
			return (NULL);
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

static int	check_cd_args(t_cmd *cmd, t_shell *shell)
{
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putendl_fd("cd: too many arguments", STDERR);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

int	cd_builtin(t_shell *shell, t_cmd *cmd)
{
	char	cwd[1024];
	char	*oldpwd;
	char	*path;

	oldpwd = NULL;
	if (check_cd_args(cmd, shell))
		return (1);
	if (save_oldpwd(cwd, &oldpwd))
		return (1);
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
