/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:45:26 by brbaazi           #+#    #+#             */
/*   Updated: 2025/07/19 10:39:01 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_and_update_exit_status(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (shell->exit_status);
}

char	*get_cmd_path(char *cmd_name, t_env *env_list)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	i = -1;
	path_var = get_env_value(env_list, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	while (paths[++i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = strjoin_free(full_path, cmd_name);
		if (access(full_path, X_OK) == 0)
		{
			free_str_array(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_str_array(paths);
	return (NULL);
}

int	exec_external(t_shell *shell, t_cmd *cmd)
{
	char	**envp;
	char	*path;

	envp = env_to_array(shell->env_list);
	if (!cmd->args[0] || cmd->args[0][0] == '\0')
	{
		free_str_array(envp);
		exit(0);
	}
	if (ft_strchr(cmd->args[0], '/'))
		path = ft_strdup(cmd->args[0]);
	else
		path = get_cmd_path(cmd->args[0], shell->env_list);
	if (!path)
	{
		print_error("minishell: ", cmd->args[0], ": command not found\n");
		free_str_array(envp);
		exit(127);
	}
	execve(path, cmd->args, envp);
	handle_execve_error(path, envp);
	return (1);
}

void	ft_handle_cmd(t_cmd *cmd)
{
	int	i;
	int	first_arg_idx;

	if (cmd && cmd->args)
	{
		first_arg_idx = 0;
		while (cmd->args[first_arg_idx] && cmd->args[first_arg_idx][0] == '\0')
			first_arg_idx++;
		if (first_arg_idx > 0)
		{
			i = 0;
			while (cmd->args[first_arg_idx + i])
			{
				cmd->args[i] = cmd->args[first_arg_idx + i];
				i++;
			}
			while (i < first_arg_idx + i)
			{
				free(cmd->args[i]);
				cmd->args[i] = NULL;
				i++;
			}
		}
	}
}

int	handle_cmd(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;

	ft_handle_cmd(cmd);
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd) && !cmd->next && !cmd->redirs)
		return (exec_builtin(shell, cmd));
	pid = fork();
	if (pid == 0)
		exec_child(shell, cmd);
	else if (pid > 0)
		return (wait_and_update_exit_status(pid, shell));
	perror("minishell");
	shell->exit_status = errno;
	return (shell->exit_status);
}
