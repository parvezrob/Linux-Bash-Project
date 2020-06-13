// http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h> 
#include <unistd.h>
#include <errno.h>





struct PID
{
     pid_t pid;
     struct PID* next;
     struct PID* prev;
};

void InsertAtHead(int x);
void PrintPID();
void DeleteANode(pid_t pid);
struct PID* GetNewNode(int x);


int parse(char *line, char **argv)
{
     //word count
     int word = 0;
     while (*line != '\0')
     { /* if not the end of line ....... */
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0'; /* replace white spaces with 0    */
          *argv++ = line;      /* save the argument position     */
          while (*line != '\0' && *line != ' ' &&
                 *line != '\t' && *line != '\n')
               line++; /* skip the argument until ...    */
          word++;
     }
     *argv = '\0'; /* mark the end of argument list  */
     return word;
}
void cfile();
void checkProcessID();
void ReversePrint();
void checkEndPID(pid_t x);

/* Global Variable */
struct PID* head;
int main(void)
{
     // char  *argv[64];              /* the command line argument      */

     head = NULL;
     while (1)
     {   
          int word_count;
          int status;
          printf("\nShell -> "); /*   display a prompt             */
          char line[1024]; /* the input line */
          scanf("%[^\n]%*c", line);
          if(strcmp(line, "finish") == 0)
               break;
          char *argv[64];    /* the command line argument      */
          word_count = parse(line, argv); /*   parse the line               */
          
          if (strcmp(*argv, "dc") == 0)
          {
               char s[100];
               // printing current working directory
               printf("%s\n", getcwd(s, 100));
               // using the command
               chdir(argv[1]);
               // printing current working directory
               printf("%s\n", getcwd(s, 100));
          }
          else if (strcmp(*argv, "df") == 0 || strcmp(*argv, "ddir") == 0)
          {
               if (remove(argv[1]) == -1)
                    printf("File/Folder doesn't exist");
          }
          else if (strcmp(*argv, "cfile") == 0)
          {
               cfile(argv[1], argv[2]);
          } else if(strcmp(*argv, "list") == 0 && strcmp(argv[1], "pids") == 0){
               PrintPID();
          }
          else
          {
               pid_t pid, endID;
               /* fork a child process */
               pid = fork();
               if (pid < 0)
               { /* error occurred */
                    fprintf(stderr, "Fork Failed");
                    return 1;
               }
               else if (pid == 0)
               { /* child process */
                    if (execvp(*argv, argv) == -1)
                    {
                         printf("Error : The command didn't execute succesfully.\n");
                         break;
                    }
               }
               else
               { /* parent process */
                    /* parent will wait for the child to complete */
                    if(strcmp(argv[word_count-1], "&") == 0){
                         printf("working on background");
                         
                         
                    } else {
                         wait(NULL);
                    }
                    
                    printf("PID: %d", pid );
                    InsertAtHead(pid);

                    
               }
          }

          checkProcessID();
     }
}

void cfile(char filename1[], char filename2[])
{
     FILE *fptr1, *fptr2;
     char c;
     // Open one file for reading
     fptr1 = fopen(filename1, "r");
     if (fptr1 == NULL)
     {
          printf("Cannot open file %s \n", filename1);
          exit(0);
     }
     // Open another file for writing
     fptr2 = fopen(filename2, "w");
     if (fptr2 == NULL)
     {
          printf("Cannot open file %s \n", filename2);
          exit(0);
     }
     // Read contents from file
     c = fgetc(fptr1);
     while (c != EOF)
     {
          fputc(c, fptr2);
          c = fgetc(fptr1);
     }
     printf("\nContents copied to %s\n", filename2);
     fclose(fptr1);
     fclose(fptr2);
}



/*************** Linked List ******************/
struct PID* GetNewNode(pid_t x) {
	struct PID* newNode
		= (struct PID*)malloc(sizeof(struct PID));
	newNode->pid = x;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

//Inserts a Node at head of doubly linked list
void InsertAtHead(pid_t x) {
	struct PID* newNode = GetNewNode(x);
	if(head == NULL) {
		head = newNode;
		return;
	}
	head->prev = newNode;
	newNode->next = head; 
	head = newNode;
}


//Prints all the elements in linked list in forward traversal order
void PrintPID() {
	struct PID* temp = head;
	printf("List of All PIDs:\n");

	while(temp != NULL) {
		printf("%d\n",temp->pid);
		temp = temp->next;
	}
}

void checkEndPID(pid_t x) {
	struct PID* temp = head;
	while(temp != NULL) {
          if(temp->pid == x)
               printf("FOUND");
	     temp = temp->next;
	}
}


void DeleteANode(pid_t pid){
     struct PID* temp = head;
     while(temp != NULL){
          if(temp->pid == pid){
               if(temp->prev == NULL){
                    head = NULL;
               } else if(temp->next == NULL){
                    temp->prev->next = NULL;
                    temp = NULL;
               } else{
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
               }
          }
          temp = temp->next;
     }
}

void checkProcessID(){
     struct PID* temp = head;
	while(temp != NULL) {
          
          int x = kill(temp->pid, 0);

          
          printf("\n%d, signal: %d", temp->pid, x);
          if(kill(temp->pid, 0) == -1){
               //DeleteANode(temp->pid);
               kill(temp->pid);
          }
		temp = temp->next;
	}
}
