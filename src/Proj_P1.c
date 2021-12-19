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
    if(NULL == readLineFromUser)
    {
        printf("invalide arg");
        return NULL;
    }
    char * iter = &(*readLineFromUser);

    char ** res;
    int sizeBuff1 = 16;
    int sizeBuff = 16;
    int index = 0;
    int index1 = 0;

    res = (char **) malloc (sizeof(char*) * sizeBuff1);

    if(NULL == *res)
    {
        printf("cant allocate mem.");
        return NULL;
    }
    do
    {
        int c = *iter;
        char * oneArg = (char*) malloc (sizeof(char) * sizeBuff);
        if(c == ' ')
        {
            if(index1 >= sizeBuff1)
            {
                res = realloc(res, sizeBuff1 * 2);
                sizeBuff1 *= 2;
            }
            res[index1] = oneArg;
            index1++;
            free(oneArg);
            iter++;

        }

        if(index >= sizeBuff && c != ' ')
        {
            oneArg = realloc(oneArg, sizeBuff * 2);
            sizeBuff *= 2;
        }
       if(c != ' ')
       {
            oneArg[index] = c; 
            index++;
            iter++;
       }
    }while(*iter != '\0' || *iter != '\n');
    return res;


}



char** splitArgumentsFromLine(char * readLineFromUser)
{
    const char * dil;
    char dilm = ' ';
    dil = &dilm;
    if(NULL == readLineFromUser)
    {
        printf("invalide arg");
        return NULL;
    }

    char ** res;
    char * oneArg;
    int sizeBuff = 16;
    int index = 0;

    res = (char **) malloc (sizeof(char*) * sizeBuff);

    if(NULL == *res)
    {
        printf("cant allocate mem.");
        return NULL;
    }
    oneArg = strtok(readLineFromUser, dil);
    while(NULL != oneArg)
    {
      if(index >= sizeBuff)
      {
          *res = realloc(*res, sizeBuff * 2);
          if(NULL == *res)
          {
              printf("cant allocate mem.");
              return NULL;
          }
          sizeBuff *= 2;
          
      }  
      res[index] = oneArg;
      index++;
      oneArg = strtok(NULL, dil);
    }
    res[index] = NULL;
    return res;


}



// #define LSH_TOK_BUFSIZE 64
// #define LSH_TOK_DELIM " \t\r\n\a"
// char** splitArgumentsFromLine(char * readLineFromUser)
// {
//   int bufsize = LSH_TOK_BUFSIZE, position = 0;
//   char **tokens = malloc(bufsize * sizeof(char*));
//   char *token;

//   if (!tokens) {
//     fprintf(stderr, "lsh: allocation error\n");
//     exit(EXIT_FAILURE);
//   }

//   token = strtok(readLineFromUser, LSH_TOK_DELIM);
//   while (token != NULL) {
//     tokens[position] = token;
//     position++;

//     if (position >= bufsize) {
//       bufsize += LSH_TOK_BUFSIZE;
//       tokens = realloc(tokens, bufsize * sizeof(char*));
//       if (!tokens) {
//         fprintf(stderr, "lsh: allocation error\n");
//         exit(EXIT_FAILURE);
//       }
//     }

//     token = strtok(NULL, LSH_TOK_DELIM);
//   }
//   tokens[position] = NULL;
//   return tokens;
// }








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
        arguments = splitArgumentsFromLine(*readLine);
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
    char ** tmp = &(*g);
    while(NULL != *tmp)
    {
        printf("%s\n", *tmp);
        tmp++;
    }
    
    return 0;
}