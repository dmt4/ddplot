  clock_t begin = clock();
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time spent = %0.1f s\n", time_spent);
