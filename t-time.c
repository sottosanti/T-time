/*
  t-time - get alerts for when tee times open up
     
 ********************************************************************
   version: 1.0
   date:    August 2021
   author:  Stephen Sottosanti, stephensottosanti@gmail.com
 
   description: use t-time to get alerts when tee times open at Galloping Hill 18 in Kenilworth, NJ
 *******************************************************************/

#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_FILE "tee_times.txt"

size_t 
write_tee_times(char *buffer, size_t itemsize, size_t nitems, void* ignorethis);

void
process_tee_times(void);

int main(void)
{
    // CURL *curl = curl_easy_init();

    // if(!curl) {
    //     fprintf(stderr, "init failed\n");
    //     return EXIT_FAILURE;
    // }

    // /* Data Fields 
    // char p01[] = "p01=4549&"; // Golf Courses, 4549 is Galloping Hill 18
    // char p02[] = "p02=09/06/2021&"; // Date
    // char p03[] = "p03=5:00AM&"; // Start time for search
    // char p04[] = "p04=2:00PM&"; // End time for search
    // char p05[] = "p05=0&"; // IDK
    // char p06[] = "p06=4&"; // Number of players, 1 - 4
    // char p07[] = "p07=false"; // IDK
    // */

    // char data[] = "p01=4549&p02=09/06/2021&p03=5:00AM&p04=8:00PM&p05=0&p06=4&p07=false";  

    // // set options
    // curl_easy_setopt(curl, CURLOPT_URL, "https://unioncounty7day.ezlinksgolf.com/api/search/search");
    // curl_easy_setopt(curl, CURLOPT_POST, 1L);
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_tee_times);

    // // perform out action
    // CURLcode result = curl_easy_perform(curl);
    // if(result != CURLE_OK) {
    //     fprintf(stderr, "download problem: %s\n", curl_easy_strerror(result));
    // }
    // curl_easy_cleanup(curl);
    
    // Parse data in file and see if there are any tee times 
    process_tee_times();

    return EXIT_SUCCESS;
}

size_t write_tee_times(char *buffer, size_t itemsize, size_t nitems, void* ignorethis)
{
    FILE * fp = fopen(DEFAULT_FILE, "w+");
    size_t bytes = itemsize * nitems;

    for(int i = 0; i < bytes; i++) {
        fputc(buffer[i], fp);
    }

    fclose(fp);
    return bytes;
}

void process_tee_times(void)
{
    FILE * fp = fopen(DEFAULT_FILE, "r");

    char *buffer = NULL;
    size_t buffsize = 128;
    buffer = (char *)malloc(buffsize * sizeof(char));
    
    if (buffer == NULL) {
        exit(1);
    }

    int garbage_lines = 6;
    while(garbage_lines--) {
        getline(&buffer, &buffsize, fp);
    }

    // line with r06 which will contain tee times if there are any
    getline(&buffer, &buffsize, fp);

    char *token = NULL;    
    token = strchr(buffer, ']');
    
    if (!token) {
        // ']' is not present in the string so there are tee times available
        
        int count = 0;
        char body[128] = "\"";
        char comma[16] = ", "; 
        char dotdotdot[] = "...";
        char ending[] = "\"";
        
        while(getline(&buffer, &buffsize, fp) != EOF){
            if((token = strstr(buffer, "r24")) != NULL) {
                const char delim[] = "\"";
                token = strtok(buffer, delim); // nothing
                token = strtok(NULL, delim); // r24
                token = strtok(NULL, delim); // just a ":"
                token = strtok(NULL, delim); // the date we want

                switch(count++) {
                    case 0:
                        strcat(body, token);
                        break;
                    case 1 ... 3:
                        strcat(body, strcat(comma, token));
                        strcpy(comma, ", ");
                        break;
                    case 4:
                        strcat(body, dotdotdot);
                        break;
                    default:
                        break;
                }
            }
        } // end while
        strcat(body, ending);

        // summary for notification
        char summary[64] = "\"";
        char num[2];
        sprintf(num, "%d", count);
        strcat(summary, num);
        strcat(summary, " new Tee Times for 09/06/2021!\" ");

        //full command
        char command[128] = "\0";
        strcat(command, "notify-send ");
        strcat(command, summary);
        strcat(command, body);

        // run command
        system(command);
    }


    free(buffer);
    fclose(fp);
}