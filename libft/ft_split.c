/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abjellal <abjellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 11:08:45 by abjellal          #+#    #+#             */
/*   Updated: 2024/11/19 21:51:28 by abjellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_word(const char *s, char c)
{
	int	i;
	int	count;
	int	word;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] != c && word == 0)
		{
			count++;
			word = 1;
		}
		else if (s[i] == c)
			word = 0;
		i++;
	}
	return (count);
}

static int	len_word(const char *s, char c)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	*allocate_word(const char *s, int len)
{
	char	*word;
	int		i;

	if (!s || len < 0)
		return (NULL);
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	*free_split(char **arr, int index)
{
	int	i;

	if (!arr)
		return (NULL);
	i = 0;
	while (i < index)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	char	**p;
	int		words;
	int		j;
	int		lenw;

	if (!s)
		return (NULL);
	words = count_word(s, c);
	p = (char **)malloc((words + 1) * sizeof(char *));
	if (!p)
		return (NULL);
	j = 0;
	while (*s && j < words)
	{
		while (*s && *s == c)
			s++;
		lenw = len_word(s, c);
		p[j] = allocate_word(s, lenw);
		if (!p[j])
			return (free_split(p, j));
		s += lenw;
		j++;
	}
	p[j] = NULL;
	return (p);
}

/*int main()
{
	char str[] = "       hello world    ";
	char c = ' ';
	int i;
	i = 0;
	char **result = ft_split(str, c);
	while (result[i])
	{
		printf("%s\n", result[i]);
		i++;
	}
}*/
