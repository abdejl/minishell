/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brbaazi <brbaazi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:34:10 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/18 20:58:39 by brbaazi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(t_cmd *cmd)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);
    if (!ft_strcmp(cmd->args[0], "cd") ||
        !ft_strcmp(cmd->args[0], "echo") ||
        !ft_strcmp(cmd->args[0], "pwd") ||
        !ft_strcmp(cmd->args[0], "export") ||
        !ft_strcmp(cmd->args[0], "unset") ||
        !ft_strcmp(cmd->args[0], "env") ||
        !ft_strcmp(cmd->args[0], "exit"))
        return (1);
    return (0);
}

int exec_builtin(t_shell *shell, t_cmd *cmd)
{
    if (!ft_strcmp(cmd->args[0], "cd"))
        return (cd_builtin(shell, cmd));
    else if (!ft_strcmp(cmd->args[0], "echo"))
        return (ft_echo(cmd));
    else if (!ft_strcmp(cmd->args[0], "pwd"))
        return (ft_pwd());
    else if (!ft_strcmp(cmd->args[0], "export"))
        return (ft_export(shell, cmd));
    else if (!ft_strcmp(cmd->args[0], "unset"))
        return (ft_unset(shell, cmd));
    else if (!ft_strcmp(cmd->args[0], "env"))
        return (ft_env(shell->env_list));
    else if (!ft_strcmp(cmd->args[0], "exit"))
         return (mini_exit(shell, cmd));
    return (1);
}


void	exec_child(t_shell *shell, t_cmd *cmd)
{
	set_signal_child();
	if (check_redirections(cmd) == -1)
		exit(1);
	if (is_builtin(cmd))
		exit(exec_builtin(shell, cmd));
	exit(exec_external(shell, cmd));
}

void	handle_execve_error(char *path, char **envp)
{
	if (access(path, F_OK) != 0)
	{
		perror(path);
		free(path);
		free_str_array(envp);
		exit(127);
	}
	else if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		free(path);
		free_str_array(envp);
		exit(126);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
		free(path);
		free_str_array(envp);
		exit(126);
	}
}