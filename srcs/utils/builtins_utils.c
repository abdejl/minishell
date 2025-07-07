
#include "minishell.h"

// int	ft_isdigit(int c)
// {
// 	if (c >= '0' && c <= '9')
// 		return (1);
// 	return (0);
// }

// int	ft_isalnum(int c)
// {
// 	return (ft_isalpha(c) || ft_isdigit(c));
// }

// int	ft_atoi(const char *str)
// {
// 	int	i;
// 	int	result;
// 	int	sign;

// 	i = 0;
// 	result = 0;
// 	sign = 1;
// 	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
// 		i++;
// 	if (str[i] == '-' || str[i] == '+')
// 	{
// 		if (str[i] == '-')
// 			sign *= -1;
// 		i++;
// 	}
// 	while (str[i] && str[i] <= '9' && str[i] >= '0')
// 	{
// 		result = result * 10;
// 		result += str[i] - 48;
// 		i++;
// 	}
// 	return (result * sign);
// }

// void	ft_putendl_fd(char *s, int fd)
// {
// 	int	i;

// 	i = 0;
// 	if (!s)
// 		return ;
// 	while (s[i])
// 	{
// 		write(fd, &s[i], 1);
// 		i++;
// 	}
// 	write(fd, "\n", 1);
// }


// void ft_putstr_fd(char *s, int fd)
// {
// 	int	i;

// 	i = 0;
// 	if (!s)
// 		return ;
// 	while (s[i])
// 	{
// 		write(fd, &s[i], 1);
// 		i++;
// 	}
// }

// void	ft_putchar_fd(char c, int fd)
// {
// 	write(fd, &c, 1);
// }


