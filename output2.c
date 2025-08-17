#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
char* highTech=NULL;
char* gaming=NULL;
char* software=NULL;
char* hardware=NULL;
char* industrial=NULL;
{
int len;
len = strlen("Apple@Google@Microsoft@Nvidia@Adobe@Oracle@Sap");
if (highTech == NULL) highTech=malloc(len+1);
else 	highTech=realloc(highTech, len+1);
strcpy(highTech, Apple@Google@Microsoft@Nvidia@Adobe@Oracle@Sap);
}
{
char* unidied = RT_unifyCollections(highTech, "PayPal@Nice@Amdocs@OpenAI@Ford@Mercedes");
int len = strlen(unified);
if (highTech == NULL)	highTech=malloc(len+1);
else	highTech = realloc(highTech, strlen(highTech)+len+1);
strcpy(highTech, unified);
}
{
int len;
len = strlen("Sony@Apple@Microsoft@Google@Nintendo@Playtika");
if (gaming == NULL) gaming=malloc(len+1);
else 	gaming=realloc(gaming, len+1);
strcpy(gaming, Sony@Apple@Microsoft@Google@Nintendo@Playtika);
}
{
int len;
len = strlen("Apple@Microsoft@Oracle@Google@Sap@PayPal@Playtika@Amdocs@OpenAI");
if (software == NULL) software=malloc(len+1);
else 	software=realloc(software, len+1);
strcpy(software, Apple@Microsoft@Oracle@Google@Sap@PayPal@Playtika@Amdocs@OpenAI);
}
{
int len;
len = strlen("Apple@Nice@Sony@Google@Cummins@Nucor@Microsoft@Nvidia");
if (hardware == NULL) hardware=malloc(len+1);
else 	hardware=realloc(hardware, len+1);
strcpy(hardware, Apple@Nice@Sony@Google@Cummins@Nucor@Microsoft@Nvidia);
}
{
int len;
len = strlen("Caterpillar@Cummins@Nucor");
if (industrial == NULL) industrial=malloc(len+1);
else 	industrial=realloc(industrial, len+1);
strcpy(industrial, Caterpillar@Cummins@Nucor);
}
{
char* result;
char* intersection = RT_intersectCollections(software, hardware);
int len = strlen(intersection);
if (result == NULL) result = malloc(len + 1);
else result = realloc(result, strlen(result) + len + 1);
strcpy(result, intersection);
}
printf("Companies that sell hardware & software: ");
printf("{");
{
char* temp = malloc(strlen(software) + 1);
strcpy(temp, software);
char *token;
token = strtok(temp + 1, "@");
char* comma = "";
do {
	if (token) printf("%s%s", comma, token);
	comma = ", ";
	token = strtok(NULL, "@");
} while (token);
free(temp);
}
printf("}\n");
char* highSW=NULL;
{
char* intersection = RT_intersectCollections(software, highTech);
int len = strlen(intersection);
if (highSW == NULL) highSW = malloc(len + 1);
else highSW = realloc(highSW, strlen(highSW) + len + 1);
strcpy(highSW, intersection);
}
if (highSW == software)
printf("All software companies are high-tech companies: ");
printf("{");
{
char* temp = malloc(strlen(highSW) + 1);
strcpy(temp, highSW);
char *token;
token = strtok(temp + 1, "@");
char* comma = "";
do {
	if (token) printf("%s%s", comma, token);
	comma = ", ";
	token = strtok(NULL, "@");
} while (token);
free(temp);
}
printf("}\n");
else
printf("Not all software companies are high-tech companies: ");
printf("{");
{
char* temp = malloc(strlen(highSW) + 1);
strcpy(temp, highSW);
char *token;
token = strtok(temp + 1, "@");
char* comma = "";
do {
	if (token) printf("%s%s", comma, token);
	comma = ", ";
	token = strtok(NULL, "@");
} while (token);
free(temp);
}
printf("}\n");
{
char* unidied = RT_unifyCollections(highSW, "Playtika");
int len = strlen(unified);
if (highSW == NULL)	highSW=malloc(len+1);
else	highSW = realloc(highSW, strlen(highSW)+len+1);
strcpy(highSW, unified);
}
if (highSW == software)
printf("Now all software companies are high-tech companies: ");
printf("{");
{
char* temp = malloc(strlen(highSW) + 1);
strcpy(temp, highSW);
char *token;
token = strtok(temp + 1, "@");
char* comma = "";
do {
	if (token) printf("%s%s", comma, token);
	comma = ", ";
	token = strtok(NULL, "@");
} while (token);
free(temp);
}
printf("}\n");
else
printf("Not all software companies are high-tech companies: ");
printf("{");
{
char* temp = malloc(strlen(highSW) + 1);
strcpy(temp, highSW);
char *token;
token = strtok(temp + 1, "@");
char* comma = "";
do {
	if (token) printf("%s%s", comma, token);
	comma = ", ";
	token = strtok(NULL, "@");
} while (token);
free(temp);
}
printf("}\n");
{
char* result;
char* unified = RT_unifyCollections(software, hardware);
int len = strlen(unified);
if (result == NULL) result = malloc(len + 1);
else result = realloc(result, strlen(result) + len + 1);
strcpy(result, unified);
}
printf("Companies that do software or hardware: ");
printf("{");
{
char* temp = malloc(strlen(software) + 1);
strcpy(temp, software);
char *token;
token = strtok(temp + 1, "@");
char* comma = "";
do {
	if (token) printf("%s%s", comma, token);
	comma = ", ";
	token = strtok(NULL, "@");
} while (token);
free(temp);
}
printf("}\n");
if (industrial & software == {})
printf("No industrial companies sell software");
{
char* result;
char* intersection = RT_intersectCollections(software, gaming);
int len = strlen(intersection);
if (result == NULL) result = malloc(len + 1);
else result = realloc(result, strlen(result) + len + 1);
strcpy(result, intersection);
}
{
char* result;
char* difference = RT_differenceCollections(hardware, software);
int len = strlen(difference);
if (result == NULL) result = malloc(len + 1);
else result = realloc(result, strlen(result) + len + 1);
strcpy(result, difference);
}
printf("Companies that sell Hardware but not Gaming Software: ");
printf("{");
{
char* temp = malloc(strlen(hardware) + 1);
strcpy(temp, hardware);
char *token;
token = strtok(temp + 1, "@");
char* comma = "";
do {
	if (token) printf("%s%s", comma, token);
	comma = ", ";
	token = strtok(NULL, "@");
} while (token);
free(temp);
}
printf("}\n");
}
