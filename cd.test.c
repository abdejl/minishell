#include "../../includes/minishell.h"
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}
int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
		s1++, s2++;
	return (unsigned char)*s1 - (unsigned char)*s2;
}
char	*ft_strdup(const char *s1)
{
	char	*dest;
	size_t	i;

	i = 0;
	dest = malloc(sizeof(char) * ft_strlen(s1) + 1);
	if (dest == NULL)
	{
		return (NULL);
	}
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	h;

	h = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == h)
			return ((char *)&s[i]);
		i++;
	}
	if (h == '\0')
		return ((char *)&s[i]);
	return (NULL);
}
t_env *init_env_list(char **envp)
{
	t_env *head = NULL;
	t_env *new;
	t_env *last = NULL;
	int i = 0;

	while (envp[i])
	{
		char *equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			*equal = '\0'; // تقسّم السطر مكان '=' مؤقتاً
			new = malloc(sizeof(t_env));
			new->key = ft_strdup(envp[i]);
			new->value = ft_strdup(equal + 1);
			new->next = NULL;
			*equal = '='; // ترجّع '=' لمكانها

			if (!head)
				head = new;
			else
				last->next = new;
			last = new;
		}
		i++;
	}
	return head;
}
char	*get_env_value(t_env *env, char *key_ch)
{
    while(env)
    {
        if(ft_strcmp(env->key , "HOME") == 0)
        {
            return(env->value);
        }
        env = env->next;
    }
	return NULL;
}

int	set_env_value(t_env *env, char *key_ch, char *value)
{
	while(env)
	{
		if(ft_strcmp(env->key , key_ch)== 0)
		{
			env->value = value;
			return(0);
		}
		env = env->next;
	}
	return(1);
}

char	*get_cd_path(t_shell *shell, t_cmd *cmd)
{
	char *value;
	if((ft_strcmp(cmd->args[1] , "--")== 0) || !cmd->args[1]) 
	{
		value = get_env_value(shell->env_list, "HOME");
		if(!value)
		{
			//free_env_list(shell->env_list);
			ft_putendl_fd("ERROR HOME NOT SET",STDERR);
			return(NULL);
		}
		return(value);
	}
	else if(ft_strcmp(cmd->args[1] , "--")== 0)
	{
		value = get_env_value(shell->env_list, "OLDPWD");
		if(!value)
		{
			//free_env_list(shell->env_list);
			ft_putendl_fd("ERROR OLDPWD NOT SET",STDERR);
			return (NULL);
		}
		ft_putendl_fd(value ,stdout);
		return(value);
	}
	else
		return(cmd->args[1]);
}

int	cd_builtin(t_shell *shell, t_cmd *cmd)
{
	char cwd[1024];
	char *oldpwd;
	char *path;
	char *newpwd ;
	getcwd(cwd , sizeof(cwd));
	oldpwd = ft_strdup(cwd);
	
	path = get_cd_path(shell , cmd);
	if(!path)
		return(1);
	chdir(path);
	if(chdir(path) != 0)
	{
		ft_putendl_fd("cd: <path>: <strerror(errno)", 1);
	}
	getcwd(cwd , sizeof(cwd));
	newpwd = ft_strdup(cwd);
	set_env_value(shell->env_list , "OLDPWD" , oldpwd);
	set_env_value(shell->env_list , "PWD", newpwd);
	shell->exit_status = 0;
}
