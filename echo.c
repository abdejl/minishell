#include "../../includes/minishell.h"
#include <stdio.h>
int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
		s1++, s2++;
	return (unsigned char)*s1 - (unsigned char)*s2;
}
int ft_echo(t_cmd *cmd)
{
    int flag = 0;
    int i = 1;
    if(!cmd->args[i])
        return(1);
    while(cmd->args[i] && ft_strcmp(cmd->args[i] , "-n") == 0)
    {
        flag = 1;
        i++;
    }
    if(ft_strcmp(cmd->args[0] , "echo") == 0)
    {
        while(cmd->args[i])
        {
            printf("%s", cmd->args[i]);
            if (cmd->args[i + 1])
                printf(" ");
            i++;
        }
        if(flag == 0)
            printf("\n");
        return (0);
    }
    return(1);
}

// int main(void)
// {
//     t_cmd cmd;

//     char *args[] = {"echo","-n","-n","-n","brahim", "baazi", NULL};

//     cmd.args = args;

//     ft_echo(&cmd);
//     return 0;
// }