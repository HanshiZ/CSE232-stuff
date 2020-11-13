/*		Project 2
Taking in a password and checking
against a list of common passwords

This improved version cuts out unneeded
functions, includes better memory
managemant and changes c-casts to static casts.
Although not yet taught, project was completed
with arrays, as they are my favorite.
*/

#include <cstdio>
#include <cstring>

#include <algorithm>

// this is important for sizing of array
const int ptrsz = sizeof(char*);
// this is the function that sort will call to determine order
int sortfunc(const char* str1, const char* str2);

int main()
{
	char test[12];   // the test password goes here and 12 is enough space

	char** best = static_cast<char**>(malloc(ptrsz));
	// array of char * (strings) that contain best words
	printf("Give me a password:\n");
	scanf("%11s", test);
	printf("You provided a password of %s\n", test);
	printf("The most similar passwords to %s are:\n", test);

	int count = static_cast<int>(strlen(test));
	// we start with assumption that max difference = count = size
	const int tsize = count; // tsize is size of string

	FILE* file = fopen("common_passwords.txt", "r");
	if (!file) { perror("Error"); }

	int matches = 0;  // matches for word
	int loc = 0;    // location counter for num of characters
	int chr;   // to store letter from stream

	int best_size = 0;
	fpos_t start;
	fgetpos(file, &start);
	/* the last two lines are bookmarks that allows me
	   to jump to the appropriate location to read entire
	   string */

	while ((chr = fgetc(file)) != EOF)
	{
		if (chr == 10) // this is newline where we consider if a word is close enough
		{

			int current = (loc > tsize ? loc : tsize) - matches;
			/* take the maxaimum bewteen the sizes of two strings and remove matches
			   to check for difference */
			if (current <= count)
			{
				if (current < count)
				{   // if better difference
					for (int count1 = 0; count1 < best_size; count1++)
					{
						free(best[count1]);
						if (count1 == best_size - 1) free(best); // making sure to free everything
					}
					char** best = static_cast<char**>(malloc(ptrsz)); // new array
					count = current;
					best_size = 0;
				}
				fsetpos(file, &start); // go to start of string

				char* word = static_cast<char*>(malloc(loc + 1));
				// create a new char * to fill with good word
				fread(word, 1, loc + 1, file); // reading in 
				word[loc] = 0;  // null termination
				best = static_cast<char**>(realloc(best, (best_size + 1) * ptrsz));
				// realloc for more memory
				best[best_size] = word;
				// this is eventually freed in last loop
				best_size = best_size + 1; // keeping track of size of best array

				if (!(count)) { break; } // break for perect match
			}

			loc = 0;       //reset location in string and matches
			matches = 0;
			fgetpos(file, &start); // reset start
		}
		else
		{   // this section counts the matches and string length
			if ((loc < tsize) && (test[loc] == chr)) { matches = matches + 1; }
			loc = loc + 1;
		}

	}

	fclose(file);
	// above line closese while below is to sort
	std::stable_sort(&best[0], &best[best_size], sortfunc);
	for (int count3 = 0; count3 < best_size; count3++)
	{
		printf("%s, ", best[count3]);
		free(best[count3]); // cleaning up used memory
	}

	printf("\nAll of which are %d character(s) different.\n", count);

	free(best); // freeing up the heap

	return 0;
}


int sortfunc(const char* str1, const char* str2)
{
	if (strncmp((str1), (str2), 4) < 0) return 1;
	// four is shortest possible string so that is used
	return 0;
}


