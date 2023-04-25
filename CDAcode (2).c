#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>



typedef unsigned long long var_t;	// Define a new data type var_t as an unsigned long long

void *
direct (var_t * file, *total_hit, *total_miss)
{				// Define the direct mapping function

  int index;
  int cind = 0;
  int miss = 0;
  int hit = 0;
  int HIT = 0;
  int num = 0;

  var_t *cache = calloc (32, sizeof (var_t));	// Allocate an array of size 32 * var_t

  // Loop through the 10000 elements in file
  for (int i = 1; i < 10000; i++)
    {
      for (int j = 0; j < 32; j++)
	{
	  num++;
	  // Check if the current element in the cache matches the current element in the file
	  if (cache[j] == file[i])
	    {
	      hit++;		// Increment hit counter
	      HIT = 1;		// Set HIT to true
	    }
	  i++;			// Increment i
	}

      if (!HIT)
	{			// If HIT is false
	  index = file[i] % 32;	// Compute the index
	  cache[index] = file[i];	// Update the cache
	  miss++;		// Increment miss counter
	}

      HIT = 0;			// Reset HIT
    }
  total_hit += hit;
  total_miss += miss;


  // Print the hit and miss counters
  printf ("Cache hits %d\n", hit);
  printf ("Cache misses %d\n", miss);
}

void *
twoway (var_t * file, *total_hit, *total_miss)
{				// Define the two-way mapping function

  int index;
  int cind = 0;
  int miss = 0;
  int hit = 0;
  int HIT = 0;
  int num = 0;
  int counter;

  var_t *cache = calloc (32, sizeof (var_t));	// Allocate an array of size 32 * var_t

  // Loop through the 10000 elements in file
  for (int i = 1; i < 10000; i++)
    {
      for (int j = 0; j < 32; j++)
	{
	  num++;
	  // Check if the current element in the cache matches the current element in the file
	  if (cache[j] == file[i])
	    {
	      hit++;		// Increment hit counter
	      HIT = 1;		// Set HIT to true
	    }
	}

      if (!HIT)
	{			// If HIT is false
	  index = file[i] % 32;	// Compute the index

	  if (index % 2 == 0)
	    counter = 1;
	  else
	    counter = 0;

	  int rindex;
	  rindex = (rand () % 2) + ((index - 1) + counter);
	  cache[rindex] = file[i];	// Update the cache
	  miss++;		// Increment miss counter

	}

      HIT = 0;			// Reset HIT
    }

  *total_hit += hit;
  *total_miss += miss;

  // Print the hit and miss counters
  printf ("Cache hits %d\n", hit);
  printf ("Cache misses %d\n", miss);
}



void *
full (var_t * file, *total_hit, *total_miss)
{

  int index;
  int cind = 0;
  int miss = 0;
  int hit = 0;
  int HIT = 0;
  int num = 0;
  int rindex;

  // Allocate space for a cache with 32 blocks
  var_t *cache = calloc (32, sizeof (var_t));

  // Loop over every memory access in the trace file
  for (int i = 1; i < 10000; i++)
    {
      // Loop over every block in the cache
      for (int j = 0; j < 32; j++)
	{
	  num++;
	  // If the memory access is already in the cache, increment the hit count and set HIT flag
	  if (cache[j] == file[i])
	    {
	      hit++;
	      HIT = 1;
	    }
	}

      // If the memory access is not in the cache
      if (!HIT)
	{

	  // If there is space in the cache, add the memory access to the cache at the current index
	  if (cind < 32)
	    {
	      cache[cind] = file[i];
	      cind++;
	    }
	  // If there is no space in the cache, replace a random block in the cache with the memory access
	  else
	    {
	      rindex = rand () % 32;
	      cache[rindex] = file[i];
	    }
	  // Increment the miss count
	  miss++;
	}

      // Reset HIT flag for next memory access
      HIT = 0;
    }
  total_hit += hit;
  total_miss += miss;
  // Print out the hit and miss counts
  printf ("Cache hits %d\n", hit);
  printf ("Cache misses %d\n", miss);
}



int
main (int argc, char *argv[])
{
  time_t t;
  srand ((unsigned) time (&t));

  FILE *fp = fopen ("traces.txt", "r");	//read addresses from file
  if (fp == NULL)
    {
      return 1;
    }
  // Allocate space for a buffer to read each line of the trace file
  char *b = (char *) malloc (sizeof (char) * 11);


  // Allocate space for an array of memory accesses from the trace file
  var_t *file = (var_t *) calloc (100000, sizeof (int));


  int i = 0;

  // Read each line of the trace file into the file array
  while (fgets (b, 11, fp))
    {
      if (!(b[0] == '\n' || b[0] == '\r'))
	{
	  b[strcspn (b, "\r\n")] = 0;
	  file[i] = strtol (b, NULL, 0);
	  i++;
	}
    }

  int *total_hit = calloc (1, sizeof (int));
  int *total_miss = calloc (1, sizeof (int));


  int choice = 0;

  while (choice != 1)
    {
      printf ("\n\n");
      printf
	("Type 1 to end program.\nType 2 for 2-way mapping.\nType 3 for direct mapping. \nType 4 for fully associative mapping.\nType 5 to see total hit/miss rate.\n");

      printf ("Which method would you like to choose from?__");
      scanf ("%d", &choice);
      printf ("\n\n");
      if (choice == 1)
	{
	  printf ("Program Ending :) \n");
	  choice = 1;
	}
      else if (choice == 2)
	{
	  twoway (file, &total_hit, &total_miss);
	}
      else if (choice == 3)
	{
	  direct (file, &total_hit, &total_miss);
	}
      else if (choice == 4)
	{
	  full (file, &total_hit, &total_miss);
	}
      else if (choice == 5)
	{

//printf("Fully Associative Mapping:\n");
//printf("Hit rate: %.2f%%\n", hit_rate);
//printf("Miss rate: %.2f%%\n", miss_rate);
	  printf ("Total cache hits %d\n", total_hit);
	  printf ("Total cache misses %d\n", total_miss);

	}
      else
	{
	  printf ("Please choose a valid option\n");
	}
    };
  // Free
  free (file);
  free (b);

  fclose (fp);



  return 0;
}