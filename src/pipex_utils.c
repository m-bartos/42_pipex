/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartos <mbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:51:55 by mbartos           #+#    #+#             */
/*   Updated: 2023/11/29 14:03:40 by mbartos          ###   ########.fr       */
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
