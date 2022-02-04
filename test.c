#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<sys/stat.h>
#include<fcntl.h>


#define BUFFER_SIZE 1000

int compareFile(FILE * fPtr1, FILE * fPtr2, int * line, int * col);
int  isEmpty(const char *str);
void removeEmptyLines(FILE *srcFile, FILE *tempFile);
void printFile(FILE *fptr);

int takeInput(char* choice)
{
    char* buf;
  
    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(choice, buf);
        return 0;
    } else {
        return 1;
    }
}

void removeEmptyLines(FILE *srcFile, FILE *tempFile)
{
    char buffer[BUFFER_SIZE];

    while ((fgets(buffer, BUFFER_SIZE, srcFile)) != NULL)
    {
        if(!isEmpty(buffer))
            fputs(buffer, tempFile);
    }
}

int isEmpty(const char *str)
{
    char ch;

    do
    {
        ch = *(str++);

        if(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            return 0;

    } while (ch != '\0');

    return 1;
}

void printFile(FILE *fptr)
{
    char ch;

    while((ch = fgetc(fptr)) != EOF)
        putchar(ch);
}


int compareFile(FILE * fPtr1, FILE * fPtr2, int * line, int * col)
{
    char ch1, ch2;

    *line = 1;
    *col  = 0;

    do
    {
        ch1 = fgetc(fPtr1);
        ch2 = fgetc(fPtr2);
        
        if (ch1 == '\n')
        {
            *line += 1;
            *col = 0;
        }

        if (ch1 != ch2)
            return -1;

        *col  += 1;

    } while (ch1 != EOF && ch2 != EOF);


    if (ch1 == EOF && ch2 == EOF)
        return 0;
    else
        return -1;
}

int main()
{   
    FILE * fPtr1; 
    FILE * fPtr2;
    FILE *srcFile;
    FILE *tempFile;
    
    char x,y,t,spc,l,u;
    char z[20];
    char fil[30],fil2[30];

    int results = open("Results.txt", O_WRONLY | O_CREAT, 0777);

    if (results == -1)
    {
        return -1;
    }




    int diff;
    int line, col;
    int nr;

    fPtr1 = fopen("file1.txt", "r");
    fPtr2 = fopen("file2.txt", "r");

    int choice;

    struct stat st;

    mode_t mode;
    
    printf("\n\n\t\tMy Shell\n\n\n");

    while(1)
    {
        printf("\n\n1. Diff command with the parameter -q\n");
        printf("2. Diff command with the parameter -a\n");
        printf("3. Chmod command\n");
        printf("4. More command\n");
        printf("5. Help\n");
        printf("6. Exit\n\n\n");
        printf("Enter your choice :  ");
        scanf("%d",&choice);
        
        int results2 = dup2(results, STDOUT_FILENO);

        close(results);
        
        switch(choice)
        {
            case 1: 
            printf("Choose a file.\n");
            scanf("%s", fil);
            printf("What file do you want to compare with?\n");
            scanf("%s", fil2);
            fPtr1 = fopen(fil, "r");
            fPtr2 = fopen(fil2, "r");

            if (fPtr1 == NULL || fPtr2 == NULL)
            {
                printf("\nUnable to open file.\n");
                printf("Please check whether file exists and you have read privilege.\n");
                exit(EXIT_FAILURE);
            }

                    

                    diff = compareFile(fPtr1, fPtr2, &line, &col);

                    if (diff == 0)
                    {
                        printf("\nFiles are equal.");
                    }
                    else 
                    {
                        printf("\nFiles differ.");
                    }

                    fclose(fPtr1);
                    fclose(fPtr2);
                    break;
                
            case 2: 
            printf("Choose a file.\n");
            scanf("%s", fil);
            printf("What file do you want to compare with?\n");
            scanf("%s", fil2);
            fPtr1 = fopen(fil, "r");
            fPtr2 = fopen(fil2, "r");
                if (fPtr1 == NULL || fPtr2 == NULL)
                {
                    printf("\nUnable to open file.\n");
                    printf("Please check whether file exists and you have read privilege.\n");
                    exit(EXIT_FAILURE);
                }
                    
            diff = compareFile(fPtr1, fPtr2, &line, &col);
            printf("\n");
            while((x=fgetc(fPtr1))!=EOF)
                printf("%c",x);
            printf("\n");
            while((y=fgetc(fPtr2))!=EOF)
                printf("%c",y);
            fclose(fPtr1);
            fclose(fPtr2);
            break;

            case 3:
            printf("What file do you want to work with?\n");
            scanf("%s", fil);
            stat(fil, &st);
            mode = st.st_mode & 07777;
            printf("Who do you want to target?\n");
            printf("Owner, Group, Others or all?\n");
            scanf("%s", z);
            if (strcmp(z, "Owner")==0)
            {
                printf("Add or Remove?\n");
                scanf("%s", z);
                    if(strcmp(z,"Add")==0)
                    {
                        printf("Read, Write or Execute permission?\n");
                        scanf("%s", z);
                        if(strcmp(z,"Read")==0)
                        {
                            mode |= S_IRUSR;
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Write")==0)
                        {
                            mode |= S_IWUSR;
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Execute")==0)
                        {
                            mode |= S_IXUSR;
                            chmod(fil, mode);
                        }

                    }else if(strcmp(z,"Remove")==0)
                    {
                        printf("Read, Write or Execute permission?\n");
                        scanf("%s", z);
                        if(strcmp(z,"Read")==0)
                        {
                            mode &= ~(S_IRUSR);
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Write")==0)
                        {
                            mode &= ~(S_IWUSR);;
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Execute")==0)
                        {
                            mode &= ~(S_IXUSR);;
                            chmod(fil, mode);
                        }

                    }      
            }
            if (strcmp(z, "Group")==0)
            {
                printf("Add or Remove?\n");
                scanf("%s", z);
                    if(strcmp(z,"Add")==0)
                    {
                        printf("Read, Write or Execute permission?\n");
                        scanf("%s", z);
                        if(strcmp(z,"Read")==0)
                        {
                            mode |= S_IRGRP;
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Write")==0)
                        {
                            mode |= S_IWGRP;
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Execute")==0)
                        {
                            mode |= S_IXGRP;
                            chmod(fil, mode);
                        }

                    }else if(strcmp(z,"Remove")==0)
                    {
                        printf("Read, Write or Execute permission?\n");
                        scanf("%s", z);
                        if(strcmp(z,"Read")==0)
                        {
                            mode &= ~(S_IRGRP);
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Write")==0)
                        {
                            mode &= ~(S_IWGRP);;
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Execute")==0)
                        {
                            mode &= ~(S_IXGRP);;
                            chmod(fil, mode);
                        }
                    }      
            }
            if (strcmp(z, "Others")==0)
            {
                printf("Add or Remove?\n");
                scanf("%s", z);
                    if(strcmp(z,"Add")==0)
                    {
                        printf("Read, Write or Execute permission?\n");
                        scanf("%s", z);
                        if(strcmp(z,"Read")==0)
                        {
                            mode |= S_IROTH;
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Write")==0)
                        {
                            mode |= S_IWOTH;
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Execute")==0)
                        {
                            mode |= S_IXOTH;
                            chmod(fil, mode);
                        }

                    }else if(strcmp(z,"Remove")==0)
                    {
                        printf("Read, Write or Execute permission?\n");
                        scanf("%s", z);
                        if(strcmp(z,"Read")==0)
                        {
                            mode &= ~(S_IROTH);
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Write")==0)
                        {
                            mode &= ~(S_IWOTH);;
                            chmod(fil, mode);
                        }

                        if(strcmp(z,"Execute")==0)
                        {
                            mode &= ~(S_IXOTH);;
                            chmod(fil, mode);
                        }

                    }      
            }
            break;

            case 4:
            printf("Which file do you want to open?\n");
            scanf("%s", fil);
            fPtr1 = fopen(fil, "r");
            if(fPtr1 == NULL)
            {
                printf("\nUnable to open file.\n");
                printf("Please check whether file exists and you have read privilege.\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("Which parameters?: -num, -d, -s\n");
                scanf("%s", z);
                if(strcmp(z,"-num")==0)
                    {
                        printf("\nHow many per page?\n");
                        scanf("%d", &nr);
                        int  i;
                        printf("\n");
                        do
                        {
                            for(i = 1; i <= nr; i++)
                                {
                                    while((t=fgetc(fPtr1))!='\n' && t!=EOF)
                                        printf("%c", t);
                                    printf("\n");
                                }                          
                        }while(getchar() == '\n' && t != EOF && getchar() != 'q');
                    }
                    else if (strcmp(z, "-d") == 0)
                        {
                            do
                            {
                                int i;
                                for(i = 1; i <= 10; i++)
                                {
                                    while((t=fgetc(fPtr1))!='\n' && t!=EOF)
                                        printf("%c", t);
                                    printf("\n");
                                }
                                printf("\n[Press enter to continue, 'q' to quit.]\n");
                            }while(getchar() == '\n' && t != EOF && getchar() != 'q');
                        }
                    else if (strcmp(z, "-s") == 0)
                        {
                            fPtr1 = fopen(fil, "r");
                            int ok = 0;
                            printf("Now I will squeeze the file.\n\n");
                            while(t != EOF)
                            {
                                t=fgetc(fPtr1);
                                printf("%c", t);
                                if(t=='\n')
                                {
                                    t=fgetc(fPtr1);
                                    if(t==EOF)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                    while(t=='\n')
                                    {
                                        ok = 1;

                                        t=fgetc(fPtr1);
                                    }
                                    if(ok == 1)
                                        printf("\n");
                                    printf("%c", t);
                                    }


                                }
                                ok = 0;
                            }

                        }
                fclose(fPtr1);
            }     

            break;
            case 5:
            printf("The diff command = compares 2 files.\n The -a treat all files as text.\nThe -q compares them\n");
            printf("The chmod command = change file mode bits.\n");
            printf("The more command = let's you see the file.\n With -num you can choose how many to pop up on screen.\n With -d you see a fancier interface.\n With -s you will squeeze the file and remove the excess spaces.");
            break;
            case 6: 
            exit(0);
        }
    }


    return 0;
}