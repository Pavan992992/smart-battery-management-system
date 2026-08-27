#include <stdio.h>
#include "bms.h"

void logFault(const char *message)
{
    FILE *fp;

    fp = fopen("fault_log.txt", "a");

    if (fp == NULL)
    {
        printf("Error opening fault log file.\n");
        return;
    }

    fprintf(fp, "%s\n", message);

    fclose(fp);
}


void displayFaultLog(void)
{
    FILE *fp;
    char line[100];

    fp = fopen("fault_log.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo fault log found.\n");
        return;
    }

    printf("\n**************************************\n");
    printf("             FAULT LOG\n");
    printf("**************************************\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }

    fclose(fp);

    printf("**************************************\n");
}


void clearFaultLog(void)
{
    FILE *fp;

    fp = fopen("fault_log.txt", "w");

    if (fp == NULL)
    {
        printf("\nError clearing fault log.\n");
        return;
    }

    fclose(fp);

    printf("\nFault log cleared successfully.\n");
}