
#include "minishell.h"

int ft_pwd(void)
{
    char cwd[PATH_MAX];
    
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        ft_putendl_fd(cwd, STDOUT_FILENO);
        return 0;
    }
    ft_putstr_fd("minishell: pwd", STDERR_FILENO);
    return 1;
}
