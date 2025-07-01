
// #include "minishell.h" 
// int is_builtin(t_cmd *cmd)
// {
//     if (!cmd || !cmd->args || !cmd->args[0])
//         return (1); // Not builtin (error)

//     if (!ft_strcmp(cmd->args[0], "cd") ||
//         !ft_strcmp(cmd->args[0], "echo") ||
//         !ft_strcmp(cmd->args[0], "pwd") ||
//         !ft_strcmp(cmd->args[0], "export") ||
//         !ft_strcmp(cmd->args[0], "unset") ||
//         !ft_strcmp(cmd->args[0], "env") ||
//         !ft_strcmp(cmd->args[0], "exit"))
//         return (0); // builtin found

//     return (1); // Not builtin
// }
// int exec_builtins(t_shell *shell , t_cmd *cmd)
// {
//     if(!cmd || !cmd->args[0] || !cmd->args)
//         return 1;
//     if(!ft_strcmp(cmd->args[0] , "cd"))
//         return cd_builtin(shell , cmd);
//     else if (!ft_strcmp(cmd->args[0], "echo"))
// 	    return ft_echo(cmd);
//     else if (!ft_strcmp(cmd->args[0], "pwd"))
// 	    return ft_pwd();
//     else if (!ft_strcmp(cmd->args[0], "exit"))
// 	    return mini_exit(shell, cmd);
//     else if (!ft_strcmp(cmd->args[0], "env"))
// 	    return ft_env(shell->env_list);
//     else if (!ft_strcmp(cmd->args[0], "export"))
// 	    return ft_export(shell, cmd);
//     else if (!ft_strcmp(cmd->args[0], "unset"))
//     {
//         ft_unset(shell->env_list , cmd->args[0])
//         return 0;
//     }
// }
// void handel(t_shell *shell , t_cmd *cmd)
// {
//     if(!is_builtin(cmd) || !cmd->next || !cmd->redirs)
//     {
//         shell->exit_status = exec_builtins(shell , cmd);
//         return(shell->exit_status);
//     }
//     else if

// }