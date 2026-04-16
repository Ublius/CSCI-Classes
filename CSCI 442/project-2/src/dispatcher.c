#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "dispatcher.h"
#include "shell_builtins.h"
#include "parser.h"

/**
 * dispatch_external_command() - run a pipeline of commands
 *
 * @pipeline:   A "struct command" pointer representing one or more
 *              commands chained together in a pipeline.  See the
 *              documentation in parser.h for the layout of this data
 *              structure.  It is also recommended that you use the
 *              "parseview" demo program included in this project to
 *              observe the layout of this structure for a variety of
 *              inputs.
 *
 * Note: this function should not return until all commands in the
 * pipeline have completed their execution.
 *
 * Return: The return status of the last command executed in the
 * pipeline.
 */
static int dispatch_external_command(struct command *pipeline)
{
	/*
	 * Note: this is where you'll start implementing the project.
	 *
	 * It's the only function with a "TODO".  However, if you try
	 * and squeeze your entire external command logic into a
	 * single routine with no helper functions, you'll quickly
	 * find your code becomes sloppy and unmaintainable.
	 *
	 * It's up to *you* to structure your software cleanly.  Write
	 * plenty of helper functions, and even start making yourself
	 * new files if you need.
	 *
	 * For D1: you only need to support running a single command
	 * (not a chain of commands in a pipeline), with no input or
	 * output files (output to stdout only).  In other words, you
	 * may live with the assumption that the "input_file" field in
	 * the pipeline struct you are given is NULL, and that
	 * "output_type" will always be COMMAND_OUTPUT_STDOUT.
	 *
	 * For D2: you'll extend this function to support input and
	 * output files, as well as pipeline functionality.
	 *
	 * Good luck!
	 */

  int input_fd = -1, output_fd = -1;
  int pipe_fds[2];
  // pid_t pid;
  pid_t pids[100];  // Assuming at most 100 commands
  int num_commands = 0;
  // int status;
  int final_status = 0;
  struct command *cmd = pipeline;

  if (cmd->input_filename) {
    // fprintf(stderr, "Opening input file: %s\n", cmd->input_filename);
    input_fd = open(cmd->input_filename, O_RDONLY);
    if (input_fd < 0) {
      fprintf(stderr, "Error opening input file: %s\n", cmd->input_filename);
      return -1;
    }
  }

  while (cmd) {
    if (cmd->pipe_to && cmd->output_type == COMMAND_OUTPUT_PIPE) {
      // fprintf(stderr, "Creating pipe\n");
      if (pipe(pipe_fds) < 0) {
        fprintf(stderr, "Error creating pipe\n");
        return -1;
      }
    }

    // fprintf(stderr, "Forking process\n");
    pid_t pid = fork();
    if (pid == 0) {
      // fprintf(stderr, "In child process\n");

      if (input_fd != -1) {
        // fprintf(stderr, "Redirecting input\n");
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
      }

      if (cmd->pipe_to && cmd->output_type == COMMAND_OUTPUT_PIPE) {
        // fprintf(stderr, "Redirecting output to pipe\n");
        close(pipe_fds[0]);
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[1]);
      } else {
        switch (cmd->output_type) {
          case COMMAND_OUTPUT_FILE_TRUNCATE:
            // fprintf(stderr, "Opening output file (truncate): %s\n", cmd->output_filename);
            output_fd = open(cmd->output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd < 0) {
              fprintf(stderr, "Error opening output file: %s\n", cmd->output_filename);
              exit(EXIT_FAILURE);
            }
            // fprintf(stderr, "Output file: %s\n", cmd->output_filename);
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
            break;
          case COMMAND_OUTPUT_FILE_APPEND:
            output_fd = open(cmd->output_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd < 0) {
              fprintf(stderr, "Error opening output file: %s\n", cmd->output_filename);
              exit(EXIT_FAILURE);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
            break;
          case COMMAND_OUTPUT_STDOUT:
          default:
            break;
        }
      }

      execvp(cmd->argv[0], cmd->argv);
      fprintf(stderr, "Error executing command: %s\n", cmd->argv[0]);
      exit(EXIT_FAILURE);
    } else if (pid < 0) {
      fprintf(stderr, "Error forking process\n");
      return -1;
    } else {
      fprintf(stderr, "Parent process\n");
      pids[num_commands++] = pid;
      if (cmd->pipe_to && cmd->output_type == COMMAND_OUTPUT_PIPE) {
        close(pipe_fds[1]);
        if (input_fd != -1) {
          close(input_fd);
        }
        input_fd = pipe_fds[0];
      }

      // waitpid(pid, &status, 0);
      // if (WIFEXITED(status)) {
      //   if (WEXITSTATUS(status) == 0) {
      //     return 0;
      //   } else {
      //     return -1;
      //   }
      // } else {
      //   return -1;
      // }

      // if (waitpid(pid, &status, 0) == -1) {
      //   fprintf(stderr, "Error waiting for child process\n");
      //   final_status = -1;
      // } else if (WIFEXITED(status)) {
      //   //use last commands exit status
      //   if (cmd->output_type != COMMAND_OUTPUT_PIPE) {
      //     fprintf(stderr, "Previous error code\n");
      //     final_status = WEXITSTATUS(status) == 0 ? 0 : -1;
      //   }
      // } else if (WIFSIGNALED(status)) {
      //    if (WIFSIGNALED(status) != 1) {
      //      final_status = -1;
      //    } else {
      //      final_status = 0;
      //    }
      // } else {
      //   final_status = -1;
      // }
    }

    if(cmd->pipe_to && cmd->output_type == COMMAND_OUTPUT_PIPE) {
      cmd = cmd->pipe_to;
    } else {
      break;
    }

    // cmd = cmd->pipe_to;
  }

  for (int i = 0; i < num_commands; i++) {
    // fprintf(stderr, "DEBUG: command #: %d\n", i);
    // fprintf(stderr, "DEBUG: total commands: %d\n", num_commands);
    int status;
    if (waitpid(pids[i], &status, 0) == -1) {
        // perror("Error: wait failed");
        final_status = -1;
    } else if (WIFEXITED(status)) {
        // fprintf(stderr, "DEBUG: Process %d exited normally with status %d\n", pids[i], WEXITSTATUS(status));
        if (i == num_commands - 1) {  // Use last command's exit status
          fprintf(stderr, "DEBUG: Using last command's exit status\n");
            final_status = WEXITSTATUS(status) == 0 ? 0 : -1;
            // fpritnf(stderr, "WEXITSTATUS: %d\n", final_status);
        }
    } else if (WIFSIGNALED(status)) {
        // fprintf(stderr, "DEBUG: Process %d was terminated by signal %d\n", pids[i], WTERMSIG(status));
        // fprintf(stderr, "DEBUG: Signal number: %d\n", WIFSIGNALED(status));
        if (WIFSIGNALED(status) != 1) {
            final_status = -1;
        } else {
            final_status = 0;
        }
    } else {
      // fprintf(stderr, "DEBUG: Process %d exited abnormally\n", pids[i]);
        final_status = -1;
    }

  }

  // fprintf(stderr, "final status: %d\n", final_status);
  return final_status;
}

/**
 * dispatch_parsed_command() - run a command after it has been parsed
 *
 * @cmd:                The parsed command.
 * @last_rv:            The return code of the previously executed
 *                      command.
 * @shell_should_exit:  Output parameter which is set to true when the
 *                      shell is intended to exit.
 *
 * Return: the return status of the command.
 */
static int dispatch_parsed_command(struct command *cmd, int last_rv,
				   bool *shell_should_exit)
{
	/* First, try to see if it's a builtin. */
	for (size_t i = 0; builtin_commands[i].name; i++) {
		if (!strcmp(builtin_commands[i].name, cmd->argv[0])) {
			/* We found a match!  Run it. */
			return builtin_commands[i].handler(
				(const char *const *)cmd->argv, last_rv,
				shell_should_exit);
		}
	}

	/* Otherwise, it's an external command. */
	return dispatch_external_command(cmd);
}

int shell_command_dispatcher(const char *input, int last_rv,
			     bool *shell_should_exit)
{
	int rv;
	struct command *parse_result;
	enum parse_error parse_error = parse_input(input, &parse_result);

	if (parse_error) {
		fprintf(stderr, "Input parse error: %s\n",
			parse_error_str[parse_error]);
		return -1;
	}

	/* Empty line */
	if (!parse_result)
		return last_rv;

	rv = dispatch_parsed_command(parse_result, last_rv, shell_should_exit);
	free_parse_result(parse_result);
	return rv;
}
