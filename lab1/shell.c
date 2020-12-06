//+
// File:	shell.c
//
// Purpose:	This program implements a simple shell program. It does not start
//		processes at this point in time. However, it will change directory
//		and list the contents of the current directory.
//
//		The commands are:
//		   cd name -> change to directory name, print an error if the directory doesn't exist.
//		              If there is no parameter, then change to the home directory.
//		   ls -> list the entries in the current directory.
//			      If no arguments, then ignores entries starting with .
//			      If -a then all entries
//		   pwd -> print the current directory.
//		   exit -> exit the shell (default exit value 0)
//
//		if the command is not recognized an error is printed.
//-

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctype.h>

#define CMD_BUFFSIZE 1024
#define MAXARGS 10

int splitCommandLine(char* commandBuffer, char* args[], int maxargs);
void doCommand(char* args[], int nargs);
void exitF(char* args[], int nargs);
void GetcwdF(char* arg[], int nargs);
void cdF(char* args[], int nargs);
void lsF(char* args[], int nargs);
int checkDotDocument(const struct dirent* passInFile);


int main() {

	char commandBuffer[CMD_BUFFSIZE];
	char* args[MAXARGS];

	// print prompt.. fflush is needed because
	// stdout is line buffered, and won't
	// write to terminal until newline
	printf("%%> ");
	fflush(stdout);

	while (fgets(commandBuffer, CMD_BUFFSIZE, stdin) != NULL) {

		// Remove newline at end of buffer
		// TODO Step 2: remove newline from end of buffer
		char* tv = commandBuffer;
		*(tv + strlen(commandBuffer) - 1) = '\0';

		int i;

		// Split command line into words.
		// TODO Step 2: call splitCommandLine with the right parameters
		int nargs = splitCommandLine(commandBuffer, args, MAXARGS);

		// Debugging for step 2
		printf("%d\n", nargs);

		if (nargs == 0) {
			printf("%%> ");
			fflush(stdout);
		}

		for (i = 0; i < nargs; i++) {
			printf("%d: %s\n", i, args[i]);

			// Execute the command
			// TODO: Step 3 call doCommand with the right arguments
			// Remember to check if there is a command (i.e. value of nargs)
			doCommand(args + i, nargs);

			// print prompt
			printf("%%> ");
			fflush(stdout);
		}

	}
}

////////////////////////////// String Handling ///////////////////////////////////

//+
// Function:	skipChar
//
// Purpose:	This function skips over a given char in a string
//		For security, the function will not skip null chars.
//
// Parameters:
//    charPtr	Pointer to string
//    skip	character to skip
//
// Returns:	Pointer to first character after skipped chars.
//		Identity function if the string doesn't start with skip,
//		or skip is the null character
//-

char* skipChar(char* charPtr, char skip) {
	if (skip == ' ') {         // skip all ' '
		while (*charPtr == ' ' || *(charPtr + 1) == ' ') {
			charPtr++;
		}
		return charPtr;
	}
	else if (skip == '\0') {
		return charPtr;
	}
	else {
		while (*charPtr != ' ' && *charPtr != '\0') {  //skip to find the first char 
			charPtr++;
		}
		return charPtr;
	}
}
//  // TODO Step 2: skip over instances of the char skip
//  //    return input value of charPtr if *char is null char

//+
// Function:	splitCommandLine
//
// Purpose:	This splits a string into an array of strings.
//		The array is passed as an argument. The string
//		is modified by replacing some of the space characters
//		with null characters to terminate each of the strings.
//
// Parameters:
//	commandBuffer	The string to split
//	args		An array of char pointers
//	maxargs		Size of array args (max number of arguments)
//
// Returns:	Number of arguments (< maxargs).
//
//-

int splitCommandLine(char* commandBuffer, char* args[], int maxargs)
{
	int i = 0;
	while (*commandBuffer != '\0')
	{
		args[i++] = commandBuffer = skipChar(commandBuffer, ' ');
		char* end = strchr(commandBuffer, ' ');
		if (!end)
			break;
		*end = '\0';
		commandBuffer = ++end;
	}
	if (i > maxargs)
		printf("err");
	else
		return i;
}


////////////////////////////// Command Handling ///////////////////////////////////

// typedef for pointer to command handling functions

// TODO STEP 3a write the typedef for the function handler pointer to
// use in the structure immediately below.
// See the description of the function prototypes at the bottom of
// the file in the comments.
typedef void (*commandFPtr)(char* args[], int nargs);


// commandType type:
// Associates a command name with a command handling function
// TODO STEP 3b use the typedef above (Step 3a) to make a two element
// struct. The first is a char * for the name of the command
// the other is a function pointer for the command handling function
struct commandType
{
	char* commandName;
	commandFPtr commandFunc;

	char** args;
	int nargs;
};

// prototypes for command handling functions
// TODO STEP 4b,6: add a function prototype
// for each command handling function
void callFunction(commandFPtr func, char* args[], int nargs) {
	func(args, nargs);
}

// Array that provides the list commands and functions
// must be terminated by {NULL, NULL}
// in a real shell, this would be a hashtable.
// TODO Step 4a: add a global array of
// the type above that contains initializers
// of strings and command handling funciton names
struct commandType commandArrayName[] = {
	{"ls", lsF},
	{"cd", cdF},
	{"pwd", GetcwdF},
	{"exit", exitF},

	{NULL,NULL}
};

//+
// Function:	doCommand
//
// Purpose:	This command calls a command handling funciton from
//		the commands array based on the first argument passed
//		in the args array.
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

void doCommand(char* args[], int nargs) {

	int a = 0;
	int b = 0;

	while (1)
	{
		if (b == nargs) {		
			break;
		}
		if (commandArrayName[a].commandName == NULL) {
			break;
		}
		if (strcmp(commandArrayName[a].commandName, args[b]) == 0) {
			callFunction(commandArrayName[a].commandFunc, args, nargs);

			a = 0;
			b++;
		}
		else {
			a++;      
		}
	}
}

//////////////////////////////////////////////////
//            command Handling Functions        //
//////////////////////////////////////////////////
// all command handling functions have the same
// parameter types and return values.
//////////////////////////////////////////////////

//+
// Function:	all handling functions
//
// Purpose:	this command performs the funcdtion
//		associated with the commands.
//		The name of the comomand is in first argument.
//	 	Since it is called by doCommand, it is known
//		that arg is at least one element long
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

// TODO step 4b put command handling function for exit here
void exitF(char* args[], int nargs) {
	exit(0);
}


// TODO step 6 put rest of command handling functions here
void GetcwdF(char* arg[], int nargs) {
	char* cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
}

void cdF(char* args[], int nargs) {
	struct passwd* pw = getpwuid(getuid());
		
	if (pw->pw_dir == NULL) {
		printf("cannnot find dirctory");
		exit(0);
	};
	if (chdir(pw->pw_dir) == 0) {
		printf("%s\n", pw->pw_dir);
		printf("cd successful \n");
	}
	else {
		printf("cd failed\n");
	}

}

void lsF(char* args[], int nargs) {
	int i = 0;
	char* tv;

	struct dirent** namelist;
	int numE = scandir(".", &namelist, NULL, NULL);

	if (nargs == 1) {
		numE = scandir(".", &namelist, checkDotDocument, NULL);
	}
	else if (*(args + 1) == "-a") {
		numE = scandir(".", &namelist, NULL, NULL);
	}

	for (i = 0; i < numE; i++) {
		tv = namelist[i]->d_name;
		printf("%s\n", tv);
	}
}

int checkDotDocument(const struct dirent* passInFile) {

	int judge = 0;
	if (passInFile->d_name[0] != '.')
	{
		judge = 1;
	}

	return judge;
}
