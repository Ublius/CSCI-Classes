#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pzip.h"

pthread_mutex_t frequency_mutex;

pthread_barrier_t barrier;

typedef struct {
  int thread_id;
  int start_index;
  int end_index;
  char *input_chars;
  struct zipped_char *local_result;
  int *local_result_count;
  int *char_frequency;
} thread_data_t;

static void *thread_function(void *arg)
{
  thread_data_t *data = (thread_data_t *)arg;
  int local_count = 0;

  for (int i = data->start_index; i < data->end_index; i++) {
    char c = data->input_chars[i];
    int count = 1;

    while (i + 1 < data->end_index && data->input_chars[i + 1] == c) {
      count++;
      i++;
    }

    data->local_result[local_count].character = c;
    data->local_result[local_count].occurence = count;
    local_count++;

    pthread_mutex_lock(&frequency_mutex);
    data->char_frequency[c - 'a'] += count;
    pthread_mutex_unlock(&frequency_mutex);
  }

  *data->local_result_count = local_count;
  pthread_barrier_wait(&barrier);
  return NULL;
}


/**
 * pzip() - zip an array of characters in parallel
 *
 * Inputs:
 * @n_threads:		   The number of threads to use in pzip
 * @input_chars:		   The input characters (a-z) to be zipped
 * @input_chars_size:	   The number of characaters in the input file
 *
 * Outputs:
 * @zipped_chars:       The array of zipped_char structs
 * @zipped_chars_count:   The total count of inserted elements into the zipped_chars array
 * @char_frequency: Total number of occurences
 *
 * NOTE: All outputs are already allocated. DO NOT MALLOC or REASSIGN THEM !!!
 *
 */
void pzip(int n_threads, char *input_chars, int input_chars_size,
	  struct zipped_char *zipped_chars, int *zipped_chars_count,
	  int *char_frequency)
{
  pthread_mutex_init(&frequency_mutex, NULL);
  pthread_barrier_init(&barrier, NULL, n_threads);

  pthread_t threads[n_threads];
  thread_data_t thread_data[n_threads];

  int chunk_size = input_chars_size / n_threads;

  struct zipped_char *local_results[n_threads];
  int local_result_counts[n_threads];

  for (int i = 0; i < n_threads; i++) {
    local_results[i] = malloc(chunk_size * sizeof(struct zipped_char));
    local_result_counts[i] = 0;

    thread_data[i].thread_id = i;
    thread_data[i].start_index = i * chunk_size;
    thread_data[i].end_index = (i + 1) * chunk_size;
    thread_data[i].input_chars = input_chars;
    thread_data[i].local_result = local_results[i];
    thread_data[i].local_result_count = &local_result_counts[i];
    thread_data[i].char_frequency = char_frequency;

    pthread_create(&threads[i], NULL, thread_function, &thread_data[i]);
  }

  for (int i = 0; i < n_threads; i++) {
    pthread_join(threads[i], NULL);
  }


  int global_index = 0;
  for (int i = 0; i < n_threads; i++) {
    for (int j = 0; j < local_result_counts[i]; j++) {
      zipped_chars[global_index++] = local_results[i][j];
    }
    free(local_results[i]);
  }

  *zipped_chars_count = global_index;

  pthread_mutex_destroy(&frequency_mutex);
  pthread_barrier_destroy(&barrier);
}
