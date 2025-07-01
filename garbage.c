// t_env *init_env_list(char **envp)
// {
// 	t_env *head = NULL;
// 	t_env *new;
// 	t_env *last = NULL;
// 	int i = 0;

// 	while (envp[i])
// 	{
// 		char *equal = ft_strchr(envp[i], '=');
// 		if (equal)
// 		{
// 			*equal = '\0'; // تقسّم السطر مكان '=' مؤقتاً
// 			new = malloc(sizeof(t_env));
// 			new->key = ft_strdup(envp[i]);
// 			new->value = ft_strdup(equal + 1);
// 			new->next = NULL;
// 			*equal = '='; // ترجّع '=' لمكانها

// 			if (!head)
// 				head = new;
// 			else
// 				last->next = new;
// 			last = new;
// 		}
// 		i++;
// 	}
// 	return head;
// }

// int main(int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	(void)av;
// 	t_shell shell;
// 	t_cmd cmd;
// 	char cwd[1024];


// 	// إعداد shell
// 	shell.env_list = init_env_list(envp);
// 	shell.exit_status = 0;
	
// 	if(getcwd(cwd , sizeof(cwd))!= NULL)
// 		set_env_value(shell.env_list , "PWD" , cwd);
// 	// ✅ test cd إلى path لي عطيتيني
// 	char *args[] = { NULL};

// 	cmd.args = args;
// 	cmd.redirs = NULL;
// 	cmd.next = NULL;

// 	// عرض قبل cd
// 	printf("📍 PWD before cd: %s\n", get_env_value(shell.env_list, "PWD"));
// 	//fr
// 	printf("📂 test de : %s\n", cmd.args[1]);

// 	// تنفيذ cd
// 	cd_builtin(&shell, &cmd);

// 	// عرض بعد cd
// 	printf("📍 PWD after cd: %s\n", get_env_value(shell.env_list, "PWD"));
// 	ft_free_env_list(shell.env_list);
// 	return (shell.exit_status);
// }

// int ft_strcmp(const char *s1, const char *s2)
// {
//     while (*s1 && *s1 == *s2)
//         s1++, s2++;
//     return (unsigned char)*s1 - (unsigned char)*s2;
// }

// int main(int ac , char **av, char **envp)
// {
//     (void)ac;
// 	(void)av;
//     (void)envp;
//     t_cmd cmd;


//     char *args[] = {"echo", "-n","brahim", "baazi", NULL};

//     cmd.args = args;

//     ft_echo(&cmd);
//     return 0;
// }

// void ft_free_env_list(t_env *env)
// {
// 	t_env *tmp;
// 	while(env)
// 	{
// 		tmp = env->next;
// 		free(env->key);
// 		free(env->value);
// 		free(env);
// 		env = tmp;
// 	}
// }

// int main(int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	(void)av;

// 	t_env *env_list = init_env_list(envp);

// 	printf("===== ENV TEST =====\n");
// 	ft_env(env_list);
//     ft_free_env_list(env_list);
// 	return 0;
// }

// int main(void)
// {
//     char cwd[1024];

//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//         printf("%s\n", cwd);
//     else
//     {
//         perror("getcwd");
//         return 1;
//     }
//     return 0;
// }


// int main(void)
// {
//     t_cmd cmd;
//     t_redirect redir;
//     char buffer[100];
//     int bytes;

//     // ✏️ بدّل بين هادو باش تجرب:
//     // redir.type = TOKEN_REDIR_OUT;   // ➤ echo > out.txt
//     // redir.type = TOKEN_APPEND;      // ➤ echo >> out.txt
//     redir.type = TOKEN_REDIR_IN;       // ➤ cat < out.txt

//     redir.file = "output.txt";
//     redir.next = NULL;

//     cmd.args = NULL;
//     cmd.redirs = &redir;
//     cmd.next = NULL;

//     if (check_redirections(&cmd) == -1)
//     {
//         perror("Redirection failed");
//         return (1);
//     }

//     if (redir.type == TOKEN_REDIR_IN)
//     {
//         bytes = read(STDIN_FILENO, buffer, 99);
//         if (bytes > 0)
//         {
//             buffer[bytes] = '\0';
//             printf("Read from file: %s\n", buffer);
//         }
//         else
//         {
//             perror("read failed");
//         }
//     }
//     else
//     {
//         printf("Hello, this should go into the file!\n");
//     }

//     return (0);
// }

// int main(void)
// {
//     t_cmd cmd;
//     t_redirect redir;

//     // ✏️ إعداد redirection من نوع OUTPUT (جرب تغيرها لاحقًا)
//     redir.type = TOKEN_REDIR_OUT;             // بدلها بـ TOKEN_REDIR_IN أو TOKEN_APPEND للتجربة
//     redir.file = "output.txt";                // الاسم ديال الملف لي غادي نخدمو عليه
//     redir.next = NULL;

//     // إعداد أمر وهمي فيه redirection وحدة
//     cmd.args = NULL;
//     cmd.redirs = &redir;
//     cmd.next = NULL;

//     // تجربة check_redirections
//     if (check_redirections(&cmd) == -1)
//     {
//         printf("Redirection failed.\n");
//         return (1);
//     }

//     // 👇 دابا، إيلا كان `>`, راه stdout تبدّل
//     printf("Hello, this should go into the file!\n");

//     return (0);
// }

