#include "minishell.h"

char    *ft_strndup(const char *s, size_t n)
{
    char    *dup;
    size_t  len;
    size_t  i;

    len = 0;
    while (s[len] && len < n)
        len++;
    dup = (char *)gc_malloc(len + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[len] = '\0';
    return (dup);
}

char	*ft_strdup(const char *s)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	while (s[i])
		i++;
	str = (char *)gc_malloc(i + 1);
	if (!str)
		return (NULL);
	j = 0;
	while (s[j])
	{
		str[j] = s[j];
		j++;
	}
	str[j] = '\0';
	return (str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	slen;
	size_t	finish;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	finish = 0;
	if (start < slen)
		finish = slen - start;
	if (finish > len)
		finish = len;
	new = (char *)gc_malloc(sizeof(char) * (finish + 1));
	if (!new)
		return (NULL);
	ft_strlcpy(new, s + start, finish + 1);
	return (new);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	k;
	char	*ptr;

	if (s1 == NULL || s2 == NULL)
	{
		return (NULL);
	}
	i = 0;
	ptr = gc_malloc(ft_strlen(s1) + ft_strlen(s2) * sizeof(char) + 1);
	if (ptr == NULL)
		return (NULL);
	while (i < ft_strlen(s1))
	{
		ptr[i] = s1[i];
		i++;
	}
	k = 0;
	while (k < ft_strlen(s2))
	{
		ptr[i + k] = s2[k];
		k++;
	}
	ptr[i + k] = '\0';
	return (ptr);
}