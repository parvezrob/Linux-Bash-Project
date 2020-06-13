// http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h> 
#include <unistd.h>

void parse(char *line, char **argv)
{
     while (*line != '\0')
     { /* if not the end of line ....... */
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0'; /* replace white spaces with 0    */
          *argv++ = line;      /* save the argument position     */
          while (*line != '\0' && *line != ' ' &&
                 *line != '\t' && *line != '\n')
               line++; /* skip the argument until ...    */
     }
     *argv = '\0'; /* mark the end of argument list  */
}
void cfile();

int main(void)
{
     // char  *argv[64];              /* the command line argument      */

     while (1)
     {
          printf("\nShell -> "); /*   display a prompt             */
          char line[1024]; /* the input line */
          scanf("%[^\n]%*c", line);
          if(strcmp(line, "finish") == 0)
               break;
          char *argv[64];    /* the command line argument      */
          parse(line, argv); /*   parse the line               */
          int size = sizeof(argv) / sizeof(argv[0]);
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
          }
          else
          {
               pid_t pid;
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
                    wait(NULL);
                    printf("PID: %d", pid );
               }
          }
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