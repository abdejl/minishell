absolute path dial bin makanch khdam fach kandir bin/ls makat9rach wkaytl3o lia had error:
minishell: command not found:/bin/ls

zedt lik had line f exec_external function f handle_cmd.c file: path = get_cmd_path(cmd->args[0], shell->env_list);

wzedt had condition:
if (ft_strchr(path, '/'))
    {
        execve(path, cmd->args, envp);
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putendl_fd(strerror(errno), STDERR_FILENO);
        free_str_array(envp);
        exit(126);
    }

bdelt had condition f executor.c:

if (cmd_list && (cmd_list->next || cmd_list->pipe_out))
		status = execute_pipeline(shell, cmd_list);

bhada:

if (cmd_list && cmd_list->pipe_out)
        status = execute_pipeline(shell, cmd_list);