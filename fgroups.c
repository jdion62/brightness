//-------------------------------------
// Standard C Library Includes
//-------------------------------------
#include <stdio.h>
#include <stdlib.h>



//-------------------------------------
// External Library Includes
//-------------------------------------
#include <table.h>
#include <set.h>
#include <atom.h>
#include <array.h>
#include <mem.h>



//-------------------------------------
// Custom Library Inclues
//-------------------------------------
// No custom libraries to include for fgroups.c



//-------------------------------------
// Globals
//-------------------------------------
#define MAX_FINGERPRINT_SIZE 512
#define MAX_NAME_LENGTH 2048



//-------------------------------------
// Function Headers
//-------------------------------------
static int is_whitespace_char(char);
static int isolate_fingerprint(char*, unsigned int);
static char flush_stdin_whitespace();
static int isolate_name(char* array);
static void clear_stdin();



//-------------------------------------
// Function Definitions
//-------------------------------------

/* is_whitespace_char(char input_char)
* Description: 
* A function to test if a character represents whitespace
*
* Inputs:
* @param input_char: a character to test
*
* Returns:
* 1 if the input_char represents a whitespace character
* 0 otherwise
*/
static int is_whitespace_char(char input_char) {
	if(input_char == ' ') return 1;
	if(input_char == '\t') return 1;
	if(input_char == '\v') return 1;
	if(input_char == '\f') return 1;
	if(input_char == '\r') return 1;
	return 0;
}


/* clear_stdin()
* Description:
* A function to clear out stdin (for use when a bad input line is given)
*
* Inputs:
* This function requires no input as all operations are performed on stdin
*
* Returns:
* This function doesn't explicitly return anything : see side effects
*
* Side Effects:
* The line currently in stdin will be discarded
*/
static void clear_stdin() {
	char ch = 'a';
	while(ch != '\n') {
		ch = fgetc(stdin);
	}
}


/* isolate_fingerprint(char * fingerprint_str)
* Description:
* A function to parse the first field of fgroups input: the fingerprint
*
* Inputs:
* @param fingerprint_str: the character array to read characters into
*						  (passed by reference)
*
* Returns:
* 0 if the fingerprint was read correctly
* 1 if the fingerprint was too large
* 2 if the fingerprint reads a \n character, indicating a malformed input line
*
* Side Effects:
* This function will write characters into
* the character array passed in from the calling component
*/
static int isolate_fingerprint(char* fingerprint_str, unsigned int arr_size) {
	
	// Read in a char here to initialize the following loop
	char input_char = fgetc(stdin);
	unsigned int pos = 1;
	
	// The fingerprint field is defined as stopping at the first
	// whitespace character so read while not a whitespace char
	while(!is_whitespace_char(input_char)) {

		// Error Checking (malformed line)
		if(input_char == '\n') {
			fprintf(stderr, "Malformed input line: line discarded\n");
			return 2;
		}

		// Error checking (size check)
		if (pos >= MAX_FINGERPRINT_SIZE) {
			fprintf(stderr, "Fingerprint too large: line discarded\n");
			return 1;
		}
		
		// Error checking (bounds check)
		// This error is technically not possible with the current version of
		// fgroups, it is an artifact of an earlier version of fgroups that 
		// didn't use the MAX_FINGERPRINT_SIZE constant
		if(pos == arr_size - 1) {
			fprintf(stderr, "Fingerprint too large: line discarded\n");
			return 1;
		}

		fingerprint_str[pos++] = input_char;
		input_char = fgetc(stdin);

		if(input_char == '\n') {
			fprintf(stderr, "Malformed input line: line discarded\n");
			return 2;
		}
	}
	
	// Add a null to the end of string
	fingerprint_str[pos] = 0;
	return 0;
}


/* flush_stdin_whitespace()
* Description:
* a function to flush whitespace characters out of stdin
*
* Inputs:
* No explicit inputs as this function only operates on stdin
*
* Returns:
* The first non-whitespace character encountered from stdin
*			(for maintaining input integrity)
*
* Side Effects
* Moves the file pointer for stdin to the second non-whitespace character
*/
static char flush_stdin_whitespace() {
	char ch = fgetc(stdin);

	while(is_whitespace_char(ch)) {
		ch = fgetc(stdin);
	}

	// Return the character to maintain the integrity of input data
	return ch;
}


/* isolate_name()
* Description:
* a function to isolate the name field of the input
*
* Inputs:
* @param name: the character array to read the name into
*
* Outputs:
* The length of the name read from stdin
*
* Side Effects:
* Reads the name field into the string pointed to by name 
*/
static int isolate_name(char* name) {
	// Pos is set to 1 by default as a result of the functionality of
	// flush_stdin_whitespace() which will read the first character 
	// of the name field
	char ch = fgetc(stdin);
	int pos = 1;

	while(ch != '\n') {
		
		// Error state
		if (pos >= MAX_NAME_LENGTH) {
			fprintf(stderr, "Name length exceeds max, truncating name...\n");
			break;
		}

		name[pos++] = ch;
		ch = fgetc(stdin);
	}
	
	name[pos] = 0;
	return pos;
}


/* print_output_from_set()
* Description:
* A function for use with Set_map() to print output from sets
*
* Inputs:
* @param member: a pointer to the member within the set
* @param c1: an additonal argument for passing information through Set_map
*
* Returns:
* This function doesn't explicitly return anything : see side effects
*
* Side Effects:
* Prints the string pointed to by member to stdout
*/
static void print_output_from_set(const void* member, void* c1) {
	(void) c1;
	printf("%s\n", (char*)member);
}


/* get_output_from_table()
* Description:
* A function for use with Table_map for getting output from internal Sets
*
* Inputs:
* @param key: The atom used as the index of the table
* @param value: The value stored at fgroups[key] (A Set_T)
* @param c1: A count of tables printed (for matching output spec)
*
* Returns:
* This function doesn't explicitly return anything : see side effects
*
* Side Effects:
* Uses Set_map to get output from every Set currently stored in the table
*/
static void get_output_from_table(const void* key, void** value, void* c1) {
	(void) key;
	
	int* count = c1;
	// If there is more than one name in an fgroup print out all names in
	// the set (this is done to match output specifications)
	if(Set_length(*value) > 1) {	
		if(*count > 0) printf("\n");
		Set_map(*value, print_output_from_set, NULL);
		*count += 1;
	}

}


/* set_free_wrapper()
* Description:
* A function to wrap the Set_free() functionality for use with Table_map
*
* Inputs:
* @param key: The atom used as the index of the table
* @param value: The value stored at fgroups[key] (A Set_T)
* @param c1: A parameter to pass additional information alongside Table_map
*
* Returns:
* This function doesn't explicitly return anything : see side effects
*
* Side Effects:
* Uses Table_map to free any memory used by internal Sets in the table
*/
static void set_free_wrapper(const void* key, void** value, void* c1) {
	(void) key;
	(void) c1;	
	
	// If the set isn't empty free it
	// This is done since Hanson indicates it is a checked runtime error
	// to call Set_free on an empty set
	if(Set_length(*value) > 0) {
		Set_free((Set_T*)value);
	}
}



//-------------------------------------
// Main Function
//-------------------------------------
int main(int argc, char* argv[]) {
	// fgroups will only take input from stdin
	// so void out argc and argv for compilation
	(void)argc;
	(void)argv;
	
	// Define storage arrays
	char fingerprint[MAX_FINGERPRINT_SIZE + 1];	
	unsigned int array_size = (sizeof(fingerprint) / sizeof(fingerprint[0]));
	
	// Begin Parsing input lines
	char name[MAX_NAME_LENGTH + 1];
	char ch = fgetc(stdin);
	
	Table_T fgroups = Table_new(10, NULL, NULL);

	while (ch != EOF) {
		// Parse the input
		fingerprint[0] = ch;
		int valid_line = isolate_fingerprint(fingerprint, array_size);	

		if(valid_line == 1) {
			clear_stdin();
			continue;
		}

		if(valid_line == 2) {
			ch = fgetc(stdin);
			continue;
		}
		
		name[0] = flush_stdin_whitespace();
		
		// Isolate_name() returns the length of the read string this
		// functionality is an artifact from an earlier version of 
		// fgroups and isn't used in this version, hence the void conversion
		int name_length = isolate_name(name);
		(void)name_length;
		
		// Convert input strings for use in Table_T's and Set_T's
		// (This is done to avoid having to write a custom hash function)
		// Atoms inherently leak memory, as such these are never freed
		const char* atom_fingerprint = Atom_string(fingerprint);
		const char* atom_name = Atom_string(name);
		
		// Check if fingerprint is already in fgroups table
		Set_T associated_set = Table_get(fgroups, atom_fingerprint);

		// Add fingerprint to table (if necessary)
		if(associated_set != NULL) {
			// add name to set thats already there
			Set_put(associated_set, atom_name);
		}
		
		else {
			// Construct a new set
			Set_T temp_set = Set_new(10, NULL, NULL);
			Table_put(fgroups, atom_fingerprint, temp_set);
			Set_put(temp_set, atom_name);	
		}

		ch = fgetc(stdin);
	}
	
	// Run through table and print the names to command line
	int count = 0;
	Table_map(fgroups, get_output_from_table, &count);

	// Clean memory
	// Once again Atoms are not freed as they inherently leak memory
	Table_map(fgroups, set_free_wrapper, NULL);
	Table_free(&fgroups);
}