#include "minishell.h"

int ft_env(t_env *env)
{
    if(!env)
        return(1);
    while(env)
    {
        if(env->value)
            printf("%s=%s\n", env->key , env->value);
        env = env->next ;
    }
    return(0);
}
