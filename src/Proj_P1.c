#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>
// #include <glib.h>
// #include <glib/gprintf.h>


char * readLineFromUser()
{
    char * scanner;
    int sizeBuffer = 16;
    scanner = (char*)malloc(sizeBuffer * sizeof(char));
    int c;
    int index = 0;

    if(0 == scanner)
    {
        printf("[ERROR] can not allocate memory.");
        return NULL;
    }
    while(1)
    {
        c = getchar();
        if(c == EOF || c == '\n')
            break;
        if(index >= sizeBuffer)
        {
            scanner = realloc(scanner, sizeBuffer * 2);
            sizeBuffer *= 2;
            if(0 == scanner)
            {
               printf("[ERROR] can not allocate memory.");
               return NULL;
            }
        }
        scanner[index] = c;
        index++;
    }
    return scanner;
}



char** splitArgumentsFromLine(char * readLineFromUser)
{
    if(NULL == *readLineFromUser)
    {
        printf("invalide arg");
        return NULL;
    }
    char * iter = *(&readLineFromUser);

    char ** res;
    int sizeBuff1 = 16;
    int sizeBuff = 16;
    int index = 0;
    int index1 = 0;

    *res = (char *) malloc (sizeof(char) * sizeBuff1);

    if(NULL == *res)
    {
        printf("cant allocate mem.");
        return NULL;
    }
    while(*iter != '\0' || *iter != '\n')
    {
        char c = *iter;
        char * oneArg = (char*) malloc (sizeof(char) * sizeBuff);
        if(*iter == ' ')
        {
            if(index1 >= sizeBuff1)
            {
                *res = realloc(*res, sizeBuff1 * 2);
                sizeBuff1 *= 2;
            }
            res[index1] = oneArg;
            index1++;
            free(oneArg);

        }

        if(index >= sizeBuff + 1)
        {
            oneArg = realloc(oneArg, sizeBuff * 2);
            sizeBuff *= 2;
        }
       
        oneArg[index] = c; 
        index++;
    }


}








void terminal_loop(bool* termStatus)
{
    char ** readLine;
    char ** arguments;


    do
    {
        *readLine = readLineFromUser();
        if(0 == *readLine)
        {
            printf("[ERROR] can not read line from the user");
            return;
        }
        arguments = splitArgumentsFromLine(&*readLine);
        if(0 == *arguments)
        {
            printf("[ERROR] can not splits line into arguments");
            free(*readLine);
            return;
        }

        // executeCommands(*arguments);
        // free(*readLine);
        // free(*arguments);

    }while(*terminal_loop != false);

}









int main(int argc, char ** argv)
{
    char * h = readLineFromUser();
    printf("%s", h);
    char ** g = splitArgumentsFromLine(h);
    while(NULL != g)
    {
        printf("%s\n", *g);
    }
    return 0;
}