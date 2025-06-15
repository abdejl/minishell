/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29                              #+#    #+#             */
/*   Updated: 2025/04/29                             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	exit_with_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR);
	ft_putstr_fd(arg, STDERR);
	ft_putendl_fd(": numeric argument required", STDERR);
	exit(255);
}

static void	too_many_args_error(t_shell *shell)
{
	ft_putendl_fd("minishell: exit: too many arguments", STDERR);
	shell->exit_status = 1;
}

int	mini_exit(t_shell *shell, t_cmd *cmd)
{
	char	**args;
	int		code;

	args = cmd->args;
	ft_putendl_fd("exit", STDOUT);
	if (!args[1])
		exit(shell->exit_status);
	if (is_numeric(args[1]) == 0)
		exit_with_error(args[1]);
	if (args[2])
	{
		too_many_args_error(shell);
		return (1);
	}
	code = ft_atoi(args[1]);
	exit(code);
}