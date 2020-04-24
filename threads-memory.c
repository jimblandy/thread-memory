/* After https://gist.github.com/tdryer/7ef02a89169252552978b6773c731109 */

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BYTES_PER_THREAD 1024 * 1024  // 1 MB

void *child_main(void *num_threads_ready) {
  /*
  char *data = malloc(BYTES_PER_THREAD);
  if (data == NULL) {
    fputs("failed to allocate memory", stderr);
    exit(1);
  }

  for (int i = 0; i < BYTES_PER_THREAD; i++) {
    data[i] = 0;
  }
  */
  atomic_fetch_add((atomic_uint *)num_threads_ready, 1);
  pause();
  return 0;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fputs("Usage: threads-memory NUM_THREADS\n", stderr);
    exit(1);
  }

  size_t num_threads = atoi(argv[1]);
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  if (!threads) {
    fputs("out of memory\n", stderr);
    exit(2);
  }

  pid_t pid = getpid();
  atomic_uint num_threads_ready;
  atomic_init(&num_threads_ready, 0);

  for (int i = 0; i < num_threads; i++) {
    if (pthread_create(&threads[i], NULL, child_main, &num_threads_ready)) {
      fputs("failed to create thread\n", stderr);
      exit(1);
    }
  }
  while (atomic_load(&num_threads_ready) < num_threads) {
    sleep(1);
  }

  // Measure kernel memory usage. Noisy, but I got some nice straight lines out of it.
  // const char template[] = "sudo slabtop -o";

  const char template[] = "pmap -x %d";

  char command[200];
  if (snprintf(command, sizeof(command), template, pid) >= sizeof(command)) {
    fputs("command weirdly long", stderr);
    exit(1);
  }
  int result = system(command);
  if (result != 0) {
    fprintf(stderr, "executing command failed with status %d: %s\n", result, command);
    exit(1);
  }

  return 0;
}
