

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

int splitCommandLine(char * commandBuffer, char* args[], int maxargs);
void doCommand(char * args[], int nargs);

int main() {

    char commandBuffer[CMD_BUFFSIZE];
    char *args[MAXARGS];

    // print prompt.. fflush is needed because
    // stdout is line buffered, and won't
    // write to terminal until newline
    printf("%%> ");
    fflush(stdout);

    while(fgets(commandBuffer,CMD_BUFFSIZE,stdin) != NULL){

	// Remove newline at end of buffer
	char* newLine = strchr(commandBuffer, '\n');
	*newLine = '\0';
	// Split command line into words.
        int nargs = splitCommandLine(commandBuffer, args, MAXARGS); 
	// Debugging for step 2
	printf("%d\n", nargs);
	int i;
	for (i = 0; i < nargs; i++){
	printf("%d: %s\n",i,args[i]);
	}

	// Check if there is command, and if there is execute it
        if (nargs > 0) {
	  doCommand(args, nargs);
	}
	// print prompt
	printf("%%> ");
	fflush(stdout);
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

char * skipChar(char * charPtr, char skip){
    // Return the input value of charPtr if *char is the null character
    if (skip == '\0')  {
	return charPtr;
    }
    // Skip over the provided character until you reach a character that is not "skip"
    while (*charPtr == skip) {
	charPtr++;
    }
    
    return charPtr;
}

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

int splitCommandLine(char * commandBuffer, char* args[], int maxargs){
    // Initialize a value to be returned that represents the number of words in the command	    
    int argsCounter = 0;
    // Check that the parameters are valid
    if (maxargs < 1) {
	fprintf(stderr, "The max number of arguments must be greater than 0");
	return 0;
    }	    
    if (*args == NULL) {
	fprintf(stderr, "The array of arguments has not been properly initialized.");
	return 0;
    }
    
    // Loop through the command until you reach the null character which represents the end of the line
    while (*commandBuffer != '\0') {
	// Set the commandBuffer pointer to the next non-space character
	commandBuffer = skipChar(commandBuffer, ' ');
	if (*commandBuffer != '\0'){
	    // Report an error if too many arguments are entered
	    if (argsCounter == maxargs){
		fprintf(stderr, "Your command has too many words. The max number of words you may enter is %d.", MAXARGS); 
		return 0;
	    }
	    // Save a pointer to the start of the current word to args[] 
	    args[argsCounter] = commandBuffer;
	    argsCounter++;
	    commandBuffer = strchr(commandBuffer, ' ');
	    // If the end of the command has been reached, break out of the loop
	    if (commandBuffer == NULL) {
	      break;
	    } else {
	      *commandBuffer = '\0';
	      commandBuffer++;
	    }
	}
    }
  
    return argsCounter;
}


////////////////////////////// Command Handling ///////////////////////////////////

// Typedef for pointer to command handling functions
typedef void (*cmdFunction)(char*, int);
 
typedef struct {
    char* commandName;
    cmdFunction commandFunction;
} cmdStruct;


// Prototypes for command handling functions
void exitFunc(char* args[], int nargs);
void pwdFunc(char* args[], int nargs);
void cdFunc(char* args[], int nargs);
void lsFunc(char* args[], int nargs);

// Array that provides the list commands and functions
cmdStruct cmdArray[] = {
    {"exit", exitFunc},
    {"pwd", pwdFunc},
    {"cd", cdFunc},
    {"ls", lsFunc},
    {NULL, NULL}
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

void doCommand(char * args[], int nargs){
    //  This is the command search loop.
    int i = 0;
    while (cmdArray[i].commandName != NULL) {
	// If the first argument is equal to the current function, call that function.
	if (strcmp(args[0], cmdArray[i].commandName)==0) {
	    cmdFunction useThisFunction = cmdArray[i].commandFunction;
	    useThisFunction(args, nargs);
	    break;
	} 
	i++;
	// If the current function has reached NULL then no array was found.
	if (cmdArray[i].commandName== NULL) {
	    fprintf(stderr, "Command not found.\n");
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

// Exit command to close the shell.
void exitFunc(char * args[], int nargs){
    exit(0);
}

// Print working directory command.
void pwdFunc(char* args[], int nargs){
    char* cwd = getcwd(NULL, 0);
    fprintf(stderr, "Your current working directory is: %s \n", cwd);
    free(cwd);
}

// Change directory command.
void cdFunc(char* args[], int nargs){
    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL){
	fprintf(stderr, "Unable to access password file entry for user.\n");
    }
    // If no argument is provided, change the current directory to the home directory.
    if (nargs == 1) {
	if (chdir(pw->pw_dir) != 0) {
	    fprintf(stderr, "Not a valid directory.\n");
	}
    }
    // Check that if an argument was provided that it is a valid directory.
    else if (nargs == 2) {
	if (chdir(args[1]) != 0) {
	    fprintf(stderr, "Not a valid directory.\n");
	}
    }
    // Print an error saying there are too many arguments.
    else {
	fprintf(stderr, "Too many arguments for a cd command.\n");
    }
}

// Helper function for the ls command to find hidden files.
int hiddenFileCheck(const struct dirent* d) {
    if (d->d_name[0] == '.') {
	return 0;
    }
    else {
	return 1;
    }
}

// List command.
void lsFunc (char* args[], int nargs){
    struct dirent **namelist;
    int numEnts;
    // List files excluding hidden files if no arguments are provided.
    if (nargs == 1){
	numEnts = scandir(".", &namelist, hiddenFileCheck, NULL);
	int i;
	for (i = 0; i < numEnts; i++){
	    fprintf(stderr, "%s\t", namelist[i]->d_name);
	}
    }
    // List all files including hidden files if the -a argument is provided.
    else if (nargs == 2){
	if (strcmp(args[1], "-a") == 0) {
	    numEnts = scandir(".", &namelist, NULL, NULL);
	    int i;
	    for (i = 0; i < numEnts; i++){
		fprintf(stderr, "%s\t", namelist[i]->d_name);
	    }
	}
	// Print an error if the second argument is not -a
	else {
	    fprintf(stderr, "Not a valid argument.\n");
	}
    }
    // Print an error if more than two arguments were provided.
    else {
	fprintf(stderr, "Too many arguments for a ls command.\n");
    }
    printf("\n");
    return;
}
