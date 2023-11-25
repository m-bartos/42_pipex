/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartos <mbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:22:30 by mbartos           #+#    #+#             */
/*   Updated: 2023/11/25 20:33:50 by mbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_paths(char **envp)
{
	char	*path;
	char	*new_path;
	char	**paths;

	path = NULL;
	while (*envp && path == NULL)
	{
		path = ft_strnstr(*envp, "PATH=", 5);
		envp++;
	}
	new_path = ft_substr(path, 5, (ft_strlen(path) - 5));
	paths = ft_split(new_path, ':');
	free(new_path);
	return (paths);
}

char	**get_cmd_paths(char **paths, char **cmd)
{
	char	**new_array;
	char	*new_path;
	size_t	i;
	size_t	len;

	len = 0;
	while (*(paths + len))
	{
		len++;
	}
	new_array = (char **) malloc(sizeof(char *) * (len + 1));
	if (!new_array)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		new_path = ft_strjoin(paths[i], "/");
		new_array[i] = ft_strjoin(new_path, cmd[0]);
		free(new_path);
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

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

int	main(int argc, char **argv, char **envp)
{
	char	**paths;
	char	***cmd_paths;
	char	***cmd;
	int		i;
	int		j;
	int		num_of_cmds;
	int		fd_in_file;
	int		fd_out_file;
	int		p_id;
	int		p_fd[2];

	if (argc < 5)
	{
		ft_putstr_fd("Not enough parameters.\n"
		"Use this format: file1 cmd1 cmd2 .. cmdn file2\n", 2);
		return (1);
	}
	// if (access(argv[1], F_OK | R_OK) < 0)
	// {
	// 	perror("First file");
	// 	return (1);
	// }
	fd_in_file = open(argv[1], O_RDONLY);
	if (fd_in_file == -1)
	{
		perror("Problem opening first file");
		return (0);
	}
	fd_out_file = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd_out_file == -1)
	{
		ft_putstr_fd("Problem opening second file\n", 2);
		return (1);
	}

	num_of_cmds = argc - 3;
	cmd_paths = malloc(sizeof(char **) * (num_of_cmds + 1));
	if(cmd_paths == NULL)
	{
		ft_putstr_fd("Malloc problem\n", 2);
		return (1);
	}
	cmd = malloc(sizeof(char **) * (num_of_cmds + 1));
	if(cmd == NULL)
	{
		ft_putstr_fd("Malloc problem\n", 2);
		return (1);
	}
	cmd_paths[num_of_cmds] = NULL;
	cmd[num_of_cmds] = NULL;
	paths = get_paths(envp);
	i = 0;
	while (i < num_of_cmds)
	{
		cmd[i] = ft_split(argv[2 + i], ' ');
		cmd_paths[i] = get_cmd_paths(paths, cmd[i]);
		i++;
	}

	i = 0;
	while (cmd_paths[0][i] && access(cmd_paths[0][i], X_OK) != 0)
		i++;
	if (cmd_paths[0][i] == NULL)
	{
		ft_putstr_fd("Command not found\n", 2);
	}
	j = 0;
	while (cmd_paths[1][j] && access(cmd_paths[1][j], X_OK) != 0)
		j++;
	if (cmd_paths[1][j] == NULL)
	{
		ft_putstr_fd("Command not found\n", 2);
	}

/* Forking, piping: */
	if (pipe(p_fd) == -1)
	{
		perror("Error creating pipe");
		return (1);
	}
	p_id = fork();
	if (p_id == -1)
	{
		perror("Error forking");
		return (1);
	}
	if (p_id == 0)
	{
		close(p_fd[0]);
		close(fd_out_file);
		dup2(fd_in_file, STDIN_FILENO);
		close(fd_in_file);
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
		execve(cmd_paths[0][i], cmd[0], envp);
	}
	p_id = fork();
	if (p_id == -1)
	{
		perror("Error forking");
		return (1);
	}
	if (p_id == 0)
	{
		close(p_fd[1]);
		close(fd_in_file);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
		dup2(fd_out_file, STDOUT_FILENO);
		close(fd_out_file);
		execve(cmd_paths[1][j], cmd[1], envp);
	}
	close(fd_in_file);
	close(fd_out_file);
	close(p_fd[0]);
	close(p_fd[1]);
	wait(NULL);
	/*freeing*/
	free_array(paths);
	i = 0;
	while (i < num_of_cmds)
	{
		free_array(*(cmd + i));
		free_array(*(cmd_paths + i));
		i++;
	}
	free(cmd);
	free(cmd_paths);
	return (0);
}