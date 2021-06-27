/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;
int MAX_WORD_LENGTH = 1024;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(22255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string) and is already done for you here 
 * for convenience.
 */
unsigned int stringHash(void *s) {
  char *string = (char *)s;
  // -- TODO --
  int length = 0;
  unsigned int result = 0;
  while (string[length] != '\0') {
    result += (unsigned int) string[length] * string[length] * string[length];
    length++;
    if (result >= 99999999) {
      result /= 99999999;
    }
  }
  return result;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  // -- TODO --
  //fprintf(stderr, "compare %s and %s is %d\n", (char*)s1, (char*)s2, !strcmp(string1, string2));
  return !strcmp(string1, string2);
}

/*
 * This function should read in every word from the dictionary and
 * store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(0)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with new lines in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *dictName) {
  // -- TODO --
  FILE* dic_file = fopen(dictName, "r");
  if (!dic_file) {
    fprintf(stderr, "Error: Dictionary \"%s\" could not be opened.\n", dictName);
    exit(0);
  }
  char* buffer = malloc(MAX_WORD_LENGTH);
  int length = 0;
  while(1) {
    length = fscanf(dic_file, "%s", buffer);
    if (length == -1) {
      break;
    }
    insertData(dictionary, buffer, buffer);
  }
  free(buffer);
  fclose(dic_file);
}

char* toLower(char *s, int from) {
  int i = from;
  for (i = from; s[i] != '\0'; i++) {
    if (s[i] >= 'A' && s[i] <= 'Z') {
      s[i] += 'a' - 'A';
    }
  }
  return s;
}

/*
 * This should process standard input (stdin) and copy it to standard
 * output (stdout) as specified in the spec (e.g., if a standard 
 * dictionary was used and the string "this is a taest of  this-proGram" 
 * was given to stdin, the output to stdout should be 
 * "this is a teast [sic] of  this-proGram").  All words should be checked
 * against the dictionary as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not found by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final 20% of your grade, you cannot assume words have a bounded length.
 */
void processInput() {
  // -- TODO --
  char* buffer = malloc(2);
  char* word = malloc(MAX_WORD_LENGTH);
  char* temp = malloc(MAX_WORD_LENGTH);
  int length = 0;
  int found = 0;
  int eof = 0;

  while (1) {
    // For any of the char, first buffer it.
    //fprintf(stderr, "file: %d\n", feof(stdin));
    fgets(buffer, 2, stdin);
    //fprintf(stderr, "get: %c,", (char)buffer[0]);
    if (feof(stdin)) {
      // check eof
      eof = 1;
    }
    if ((!('a' <= buffer[0] && buffer[0] <= 'z') && !('A' <= buffer[0] && buffer[0] <= 'Z')) || eof) {
      // if not a letter, first compare, output and clear the word before it,
      // then output buffer
      // todo
      //fprintf(stderr, "not a letter, check: %s...\n", word);
      if (length == 0) {
        fputs(buffer, stdout);
        if (eof) {
          break;
        }
        continue;
      } else {
        word[length] = '\0';
      }

      found = 0;
      strcpy(temp, word);
      //fprintf(stderr, "1:%s...", temp);
      if (findData(dictionary, word)) {
        // Level 1 key found.
        found = 1;
      } else {

        toLower(word, 1);
        //fprintf(stderr, "3:%s...", temp);
        if (findData(dictionary, word)) {
          // Level 3 key found.
          found = 3;
        } else {
          toLower(word, 0);
          //fprintf(stderr, "2:%s...", temp);
          if (findData(dictionary, word)) {
            // Level 2 key found.
            found = 2;
          } else {
            // Not found.
            //fprintf(stderr, "%s NotFound\n", word);
          }
        }
      }
      fputs(temp, stdout);
      word[0] = '\0';
      length = 0;
      if (!found) {
        fputs(" [sic]", stdout);
      }
      if (eof) {
        break;
      }
      fputs(buffer, stdout);


      //fprintf(stderr, "%s!!!", (char*)(dictionary->data[389])->data);

    } else {
      //fprintf(stderr, " a letter\n");
      // if a letter, save it in the word.
      strcpy(word + length++, buffer);
    }
  }
  fprintf(stderr, "End of File.\n");
  //fprintf(stderr, "%s!!!\n", (char*)findData(dictionary, "web"));
  free(word);
  free(temp);
  free(buffer);
}
