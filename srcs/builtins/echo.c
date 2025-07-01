#include "minishell.h"

int is_dash_n(char *s)
{
    int i = 1;

    if (!s || s[0] != '-')
        return (0);
    while (s[i])
    {
        if (s[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

int ft_echo(t_cmd *cmd)
{
    int i = 1;
    int flag = 0;
    int first_arg = 1;

    if (!cmd || !cmd->args)
        return (1);
    while (cmd->args[i] && is_dash_n(cmd->args[i]))
    {
        flag = 1;
        i++;
    }
    while (cmd->args[i])
    {
        if (!first_arg)
            write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
        first_arg = 0;
        i++;
    }
    if (flag == 0)
        write(STDOUT_FILENO, "\n", 1);
    fflush(stdout);
    return (0);
}
