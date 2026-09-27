//Joseph Kunitsky - Assignment 1 writer-app

//Accept two runtime arguments (directory and text string)
//Validate the arguments and directory
//Creates new file with name and path
//Does NOT overwrite existing file and create path if it doesn't exist
//print statement if file could not be 
// adds syslog using LOG_USER facility
// using syslog (LOG_DEBUG), write message "Writing <string> to <file>"
// <string> is the text string written to the file
// <file> is the file created by the script
// logs unexpected errors with LOG_ERR

#include <stdio.h> // declaring functions
#include <syslog.h> // declaring logging functions
#include <errno.h>
#include <string.h>

int main(int arg1, char *arg2[]) // arg1 = count, arg2 = strings
{
	openlog("writer",0, LOG_USER); // sets up connection to system logger
	
	if (arg1 != 3) // if argument count is not equal to three (command is [0] (./writer), path is [1], and text is [2])
	{
		syslog(LOG_ERR, "Two arguments required"); // sends error to system logger
		fprintf(stderr, "Usage: ./writer <file> <text>\n"); // writes message to standard error in the terminal
		closelog(); // closes the logging connection
		return 1; // 1 reports failure, 0 reports success
	}
	
	FILE *file = fopen(arg2[1], "w"); //open the file, replace contents
	
	if (file == NULL) { // fopen returns NULL upon failure
		int saved_errno = errno;
		syslog(LOG_ERR, "Cannot open %s: %s", arg2[1], strerror(saved_errno));
		fprintf(stderr, "Cannot open %s: %s\n", arg2[1], strerror(saved_errno));
		closelog();
		return 1;
		}
		
	syslog(LOG_DEBUG, "Writing %s to %s", arg2[2], arg2[1]); //sends debug-level message to system logger. records write attempt
		
	int status = 0;
		
	int openresult = fputs(arg2[2], file); //writes string from text (arg2[2]) into the file path (arg2[1])
		
	if (openresult == EOF) { // returns EOF if error occurs
		int saved_errno = errno; // eerno identifies the error, save it to saved_errno
		syslog(LOG_ERR, "Cannot write to %s", arg2[1], strerror(saved_errno)); //strerror converts error into readable description
		fprintf(stderr, "Cannot write to %s: %s\n", arg2[1], strerror(saved_errno));
		status = 1;
		}
			
	int closeresult = fclose(file); //close the file
			
	if (closeresult == EOF) { //check for output error
		int saved_errno = errno;
		syslog(LOG_ERR, "Cannot close %s: %s", arg2[1], strerror(saved_errno));
		fprintf(stderr, "Cannot close %s: %s\n", arg2[1], strerror(saved_errno));
		status = 1;
		}
	
//	//temp code to display outputs for testing
	
//	printf("file: %s\n", arg2[1]); //prints string and starts a new line
//	printf("text: %s\n", arg2[2]); 
	
	closelog();
	return status;
}
