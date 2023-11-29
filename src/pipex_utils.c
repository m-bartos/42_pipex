/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartos <mbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:51:55 by mbartos           #+#    #+#             */
/*   Updated: 2023/11/29 15:09:04 by mbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	free_array(char **array)
{
	int	i;

	i = 0;
	while (*(array + i))
	{
		free(*(array + i));
		i++;
	}
	free(array);
	return (1);
}

char	**get_arr_of_paths(char **env)
{
	char	*path;
	char	**paths;

	path = NULL;
	while (*env && path == NULL)
	{
		path = ft_strnstr(*env, "PATH=", 5);
		env++;
	}
	path = ft_substr(path, 5, (ft_strlen(path) - 5));
	paths = ft_split(path, ':');
	free(path);
	return (paths);
}

char	*get_path(char *argv, char **env)
{
	char	**cmd;
	char	*path;
	char	**paths;
	int		i;

	cmd = ft_split(argv, ' ');
	path = NULL;
	paths = get_arr_of_paths(env);
	i = 0;
	while (paths[i] != NULL)
	{
		path = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = ft_strjoin(path, cmd[0]);
		free(path);
		if (access(paths[i], X_OK) == 0)
			break ;
		i++;
	}
	if (paths[i] == NULL)
		path = NULL;
	else
		path = ft_strdup(paths[i]);
	return (free_array(paths), path);
}

char	*strip_newline(char *str)
{
	int		len;
	char	*new_line;
	int		i;

	if (str == NULL)
		return (NULL);
	len = ft_strlen(str);
	new_line = malloc (len * sizeof(char));
	i = 0;
	while (i < len - 1)
	{
		new_line[i] = str[i];
		i++;
	}
	new_line[i] = '\0';
	return (new_line);
}

void	ft_read_heredoc(char *limiter)
{
	char	*line;
	char	*clean_line;
	int		heredoc_fd;

	heredoc_fd = open("here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	clean_line = strip_newline(line);
	while (ft_strncmp(clean_line, limiter, ft_strlen(limiter)) != 0 \
		|| ft_strlen(limiter) != ft_strlen(clean_line))
	{
		write(heredoc_fd, line, ft_strlen(line) * sizeof(char));
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		free(clean_line);
		free(line);
		line = get_next_line(STDIN_FILENO);
		clean_line = strip_newline(line);
	}
	free(line);
	free(clean_line);
	close(heredoc_fd);
}
