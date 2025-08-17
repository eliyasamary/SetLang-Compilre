%{
void yyerror (char *s);
int yylex();
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "symTab.h"

#define TRUE 1
#define FALSE 0

extern char* yytext;


char* CopyStr(char* str){
    char *new, *p;

    if (str[0] == '\"') {
        new = malloc(strlen(str));
        if ((p = strchr(str+1, '\"')))
            *p = '\0';
        strcpy(new, str);
    }
    else {
        new = malloc(strlen(str)+1);
        strcpy(new, str);
    }

    return new;
}

char* AddStrToList(char* list, char* str){

	if (str[0] == '\"') {
        str++;
    }
    char *p;
    if ((p = strchr(str, '\"'))) {
        *p = '\0';
    }

    char *new = realloc(list, strlen(list) + strlen(str) + 2);
    if (!new) {
        printf("Memory allocation failed.\n");
        return list;
    }

    strcat(new, "@");
    strcat(new, str);

    return new;
}

static int idx=0;

void insert(char* varName, varType typ){
	SymTable[idx].name = malloc(strlen(varName)+1);
	strcpy(SymTable[idx].name, varName);
    SymTable[idx].typ = typ;
	++idx;
}

varType getTyp(char* var){
    for (int i=0 ; i<idx ; ++i){
        if (strcmp(SymTable[i].name, var)==0)
            return SymTable[i].typ;
	}
    return 0;
}

// COLLECTION TYPE FUNCTIONS *******************************************

void GenerateColDef(char* colVar){
	fprintf(stdout, "char* %s=NULL;\n", colVar);
	insert(colVar, Collection);
}

void GenerateColListDef(char* varList){
	char* start = varList;
    char* end = varList;

	while (*end != '\0') {
        if (*end == '@') {
            *end = '\0';
            while (isspace((unsigned char)*start)) start++;
            if (*start != '\0') {
                GenerateColDef(start);
            }
            *end = '@';
            start = end + 1;
        }
        end++;
    }
    while (isspace((unsigned char)*start)) start++;
    if (*start != '\0') {
        GenerateColDef(start);
    }
}

void GenerateColAssign(char* var, char* coll){
    char msg[32];
    int count = 0;
	char eq[] = "{}";

	if(strcmp(coll, eq) != 0){
		if (getTyp(var) != Collection) {
			sprintf(msg, "%s not defined as a collection", var);
			yyerror(msg);
		}

		if ((coll[0] != '\"') && getTyp(coll) != Collection) {
			sprintf(msg, "%s not defined as a collection", coll);
			yyerror(msg);
		}

		for (int i = 0; coll[i] != '\0'; i++) {
			if (coll[i] == '@') {
				count++;
			}
		}
		count++;

		fprintf(stdout, "{\n");
		fprintf(stdout, "int len;\n");

		if (coll[0] == '\"') {
			if (coll[0] == '\"') {
			memmove(coll, coll + 1, strlen(coll));
		}
			fprintf(stdout, "len = strlen(\"%s\");\n", coll);
		} else {
			fprintf(stdout, "strcpy(%s, %s);\n", var, coll);
		}

		fprintf(stdout, "if (%s == NULL) %s=malloc(len+1);\n", var, var);
		fprintf(stdout, "else 	%s=realloc(%s, len+1);\n", var, var);

		if (coll[0] == '\"') {
			if (count % 2 == 0) {
				fprintf(stdout, "strcpy(%s,\"\\%s\"\");\n", var, coll);
			} else {
				fprintf(stdout, "strcpy(%s,\"\\%s\");\n", var, coll);
			}
		} else {
			fprintf(stdout, "strcpy(%s, %s);\n", var, coll);
		}

		fprintf(stdout, "}\n");
	} else {
		fprintf(stdout, "%s= {};\n", var);
	}
}

void GenerateColOut(char* str, char* coll){
	char msg[32];
	// printf("// Enter GenerateColOut\n");
	// printf("// str: %s\n", str);
	// printf("// coll: %s\n", coll);

	if (coll == NULL) {
		fprintf(stdout, "printf(%s\");\n", str);
	} else {
		if ((coll[0] != '\"') && getTyp(coll) != Collection) {
			sprintf(msg, "%s not defined as a collection", coll);
			yyerror(msg);
		}

		fprintf(stdout, "printf(%s \");\n", str);
		fprintf(stdout, "printf(\"{\");\n");

		if (coll[0] == '\"') {
			char* temp = malloc(strlen(coll) + 1);
			strcpy(temp, coll);
			char *token;
			token = strtok(temp + 1, "@");
			char* comma = "";
			do {
				if (token) fprintf(stdout, "printf(\"%s%s\");\n", comma, token);
				comma = ", ";
				token = strtok(NULL, "@");
			} while (token);
			free(temp);
		} else {
			fprintf(stdout, "{\n");
			fprintf(stdout, "char* temp = malloc(strlen(%s) + 1);\n", coll);
			fprintf(stdout, "strcpy(temp, %s);\n", coll);
			fprintf(stdout, "char *token;\n");
			fprintf(stdout, "token = strtok(temp + 1, \"@\");\n");
			fprintf(stdout, "char* comma = \"\";\n");
			fprintf(stdout, "do {\n");
			fprintf(stdout, "\tif (token) printf(\"%%s%%s\", comma, token);\n");
			fprintf(stdout, "\tcomma = \", \";\n");
			fprintf(stdout, "\ttoken = strtok(NULL, \"@\");\n");
			fprintf(stdout, "} while (token);\n");
			fprintf(stdout, "free(temp);\n");
			fprintf(stdout, "}\n");
		}

		fprintf(stdout, "printf(\"}\\n\");\n");
	}
}

void GenerateColInput(char* prompt, char* var) {
    char msg[32];

    if (getTyp(var) != Collection) {
        sprintf(msg, "%s not defined as a collection", var);
        yyerror(msg);
    }

    fprintf(stdout, "printf(\"%s\");\n", prompt);

    fprintf(stdout, "{\n");
    fprintf(stdout, "char buffer[1024];\n");
    fprintf(stdout, "fgets(buffer, sizeof(buffer), stdin);\n");

    fprintf(stdout, "buffer[strcspn(buffer, \"\\n\")] = 0;\n");

    fprintf(stdout, "if (%s[0] == '\\0') {\n", var);
    fprintf(stdout, "\tstrcpy(%s, \"\\\"\");\n", var);
    fprintf(stdout, "}\n");

    fprintf(stdout, "char *token = strtok(buffer, \",\");\n");
    fprintf(stdout, "while (token != NULL) {\n");
    fprintf(stdout, "\tstrcat(%s, token);\n", var);
    fprintf(stdout, "\tstrcat(%s, \"@\");\n", var);
    fprintf(stdout, "\ttoken = strtok(NULL, \",\");\n");
    fprintf(stdout, "}\n");

    fprintf(stdout, "if (strlen(%s) > 1) {\n", var);
    fprintf(stdout, "\t%s[strlen(%s) - 1] = '\\0';\n", var, var);
    fprintf(stdout, "}\n");

    fprintf(stdout, "strcat(%s, \"\\\"\");\n", var);

    fprintf(stdout, "}\n");
}

char* RT_unifyCollections(char* var, char* coll);
char* RT_addStrToCollection(char* collection, char* str);

int check_if_varName_matches(const char* varName) {
    regex_t regex;
    int result;

    const char* pattern = "^\"[^\"]*\"$";

    result = regcomp(&regex, pattern, REG_EXTENDED);
    if (result) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    result = regexec(&regex, varName, 0, NULL, 0);
    if (!result) {
        regfree(&regex);
        return 1;
    } else if (result == REG_NOMATCH) {
        regfree(&regex);
        return 0;
    } else {
        fprintf(stderr, "Regex match failed\n");
        regfree(&regex);
        return 0;
    }
}

void GenerateColUnify(char* varResultName, char* varName, char* coll){
	char msg[32];
	int count =0;

	if ((varResultName[0]!='\"') && getTyp(varResultName)!=Collection) {
		sprintf(msg, "%s not defined as a collection", varResultName);
		yyerror(msg);
	}

	if ((varName[0]!='\"') && getTyp(varName)!=Collection) {
		sprintf(msg, "%s not defined as a collection", varName);
		yyerror(msg);
	}

	if ((coll[0]!='\"') && getTyp(coll)!=Collection) {
		sprintf(msg, "%s not defined as a collection", coll);
		yyerror(msg);
	}

    for (int i = 0; coll[i] != '\0'; i++) {
        if (coll[i] == '@') {
            count++;
        }
    }
    count++;

	fprintf(stdout, "{\n");

	if (coll[0] == '\"') {
        memmove(coll, coll + 1, strlen(coll));
		fprintf(stdout, "char* unidied = RT_unifyCollections(%s, \"%s\");\n", varName, coll);
    }else {
		fprintf(stdout, "char* unified = RT_unifyCollections(%s, %s);\n", varName, coll);
    }

	fprintf(stdout, "int len = strlen(unified);\n");

	fprintf(stdout, "if (%s == NULL)	%s=malloc(len+1);\n", varResultName, varResultName);
	fprintf(stdout, "else	%s = realloc(%s, strlen(%s)+len+1);\n", varResultName, varResultName, varResultName);

	fprintf(stdout, "strcpy(%s, unified);\n", varResultName);
	fprintf(stdout, "}\n");
}

void unionCol (char* col1, char* col2){
	char msg[32];

	if ((col1[0]!='\"') && getTyp(col1)!=Collection) {
		sprintf(msg, "%s not defined as a collection", col1);
		yyerror(msg);
	}

	if ((col2[0]!='\"') && getTyp(col2)!=Collection) {
		sprintf(msg, "%s not defined as a collection", col2);
		yyerror(msg);
	}

	fprintf(stdout, "{\n");
	fprintf(stdout, "char* result;\n");
	if (col1[0]=='\"')
		fprintf(stdout, "char* unified = RT_unifyCollections(%s, \"\\%s\");\n", col1, col2);
	else
		fprintf(stdout, "char* unified = RT_unifyCollections(%s, %s);\n", col1, col2);

	fprintf(stdout, "int len = strlen(unified);\n");

	fprintf(stdout, "if (result == NULL) result = malloc(len + 1);\n");
	fprintf(stdout, "else result = realloc(result, strlen(result) + len + 1);\n");

	fprintf(stdout, "strcpy(result, unified);\n");
	fprintf(stdout, "}\n");
}

void GenerateColDifference(char* varResultName, char* varName, char* coll) {
	printf("GenerateColDifference\n");
    char msg[32];

    if (getTyp(varResultName) != (Collection | Str)) {
		printf("varResultName: %s\n", varResultName);
        sprintf(msg, "%s not defined as a collection", varResultName);
        yyerror(msg);
    }

    if (getTyp(varName) != (Collection  | Str)) {
		printf("varName: %s\n", varName);
        sprintf(msg, "%s not defined as a collection", varName);
        yyerror(msg);
    }

    if ((coll[0] != '\"') && getTyp(coll) != (Collection | Str)) {
		printf("coll: %s\n", coll);
        sprintf(msg, "%s not defined as a collection", coll);
        yyerror(msg);
    }

    fprintf(stdout, "{\n");
    if (coll[0] == '\"')
        fprintf(stdout, "char* difference = RT_differenceCollections(%s, \"\\%s\");\n", varName, coll);
    else
        fprintf(stdout, "char* difference = RT_differenceCollections(%s, %s);\n", varName, coll);

    fprintf(stdout, "int len = strlen(difference);\n");

    fprintf(stdout, "if (%s == NULL) %s = malloc(len + 1);\n", varResultName, varResultName);
    fprintf(stdout, "else %s = realloc(%s, strlen(%s) + len + 1);\n", varResultName, varResultName, varResultName);

    fprintf(stdout, "strcpy(%s, difference);\n", varResultName);
    fprintf(stdout, "}\n");
}

void subCol(char* col1, char*col2){
	char msg[32];

	if ((col1[0]!='\"') && getTyp(col1)!=Collection) {
		sprintf(msg, "%s not defined as a collection", col1);
		yyerror(msg);
	}

	if ((col2[0]!='\"') && getTyp(col2)!=Collection) {
		sprintf(msg, "%s not defined as a collection", col2);
		yyerror(msg);
	}

	fprintf(stdout, "{\n");
	fprintf(stdout, "char* result;\n");
	if (col1[0]=='\"')
		fprintf(stdout, "char* difference = RT_differenceCollections(%s, \"\\%s\");\n", col1, col2);
	else
		fprintf(stdout, "char* difference = RT_differenceCollections(%s, %s);\n", col1, col2);

	fprintf(stdout, "int len = strlen(difference);\n");

	fprintf(stdout, "if (result == NULL) result = malloc(len + 1);\n");
	fprintf(stdout, "else result = realloc(result, strlen(result) + len + 1);\n");

	fprintf(stdout, "strcpy(result, difference);\n");
	fprintf(stdout, "}\n");
}

void GenerateColIntersection(char* varResultName, char* varName, char* coll) {
    char msg[32];

    if ((varResultName[0] != '\"') &&getTyp(varResultName) != Collection) {
        sprintf(msg, "%s not defined as a collection", varResultName);
        yyerror(msg);
    }

    if (getTyp(varName) != Collection) {
        sprintf(msg, "%s not defined as a collection", varName);
        yyerror(msg);
    }

    if ((coll[0] != '\"') && getTyp(coll) != Collection) {
        sprintf(msg, "%s not defined as a collection", coll);
        yyerror(msg);
    }

    fprintf(stdout, "{\n");
    if (coll[0] == '\"')
        fprintf(stdout, "char* intersection = RT_intersectCollections(%s, \"\\%s\");\n", varName, coll);
    else
        fprintf(stdout, "char* intersection = RT_intersectCollections(%s, %s);\n", varName, coll);

    fprintf(stdout, "int len = strlen(intersection);\n");

    fprintf(stdout, "if (%s == NULL) %s = malloc(len + 1);\n", varResultName, varResultName);
    fprintf(stdout, "else %s = realloc(%s, strlen(%s) + len + 1);\n", varResultName, varResultName, varResultName);

    fprintf(stdout, "strcpy(%s, intersection);\n", varResultName);
    fprintf(stdout, "}\n");
}

void intersectionCol (char* col1, char* col2){
	char msg[32];

	if ((col1[0] != '\"') &&getTyp(col1) != Collection) {
		sprintf(msg, "%s not defined as a collection", col1);
		yyerror(msg);
	}

	if ((col2[0] != '\"') &&getTyp(col2) != Collection) {
		sprintf(msg, "%s not defined as a collection", col2);
		yyerror(msg);
	}

	fprintf(stdout, "{\n");
	fprintf(stdout, "char* result;\n");
    if (col1[0] == '\"')
        fprintf(stdout, "char* intersection = RT_intersectCollections(%s, \"\\%s\");\n", col1, col2);
    else
        fprintf(stdout, "char* intersection = RT_intersectCollections(%s, %s);\n", col1, col2);

    fprintf(stdout, "int len = strlen(intersection);\n");

    fprintf(stdout, "if (result == NULL) result = malloc(len + 1);\n");
    fprintf(stdout, "else result = realloc(result, strlen(result) + len + 1);\n");

    fprintf(stdout, "strcpy(result, intersection);\n");
    fprintf(stdout, "}\n");

}

// INT TYPE FUNCTIONS *******************************************

char* CopyInt(char* str) {
    char* result = malloc(strlen(str) + 1);
    strcpy(result, str);
    return result;
}

char* AddIntToList(char* list, char* intStr) {
    char* new;
    if (list == NULL) {
        list = malloc(strlen(intStr) + 1);
        strcpy(list, intStr);
        return list;
    }
    new = realloc(list, strlen(list) + strlen(intStr) + 2);
    strcat(new, "@");
    strcat(new, intStr);
    return new;
}

void GenerateIntDef(char* intVarName){
    fprintf(stdout, "int %s=NULL;\n", intVarName);
    insert(intVarName, Int);
}

void GenerateIntListDef(char* varList){
    char* token = strtok(varList, "@");
    while (token != NULL) {
        while (isspace((unsigned char)*token)) token++;
        if (*token == '\0') {
            token = strtok(NULL, "@");
            continue;
        }
        GenerateIntDef(token);
        insert(token, Int);
        token = strtok(NULL, "@");
    }
}

void GenerateIntAssign(char* var, char* value) {
    char msg[32];

    if (getTyp(var) != Int) {
        sprintf(msg, "%s not defined as an integer", var);
        yyerror(msg);
    }

    int intValue = atoi(value);

    fprintf(stdout, "%s = %d;\n", var, intValue);
}

void GenerateIntAdd(char* varResultName, char* var1Name, char* var2Name) {
    char msg[32];
	int num;

    if (getTyp(varResultName) != Int && (sscanf(varResultName, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", varResultName);
        yyerror(msg);
    }

    if (getTyp(var1Name) != Int && (sscanf(var1Name, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", var1Name);
        yyerror(msg);
    }

    if (getTyp(var2Name) != Int && (sscanf(var2Name, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", var2Name);
        yyerror(msg);
    }

    fprintf(stdout, "{\n");
    fprintf(stdout, "int sum = %s + %s;\n", var1Name, var2Name);
    fprintf(stdout, "%s = sum;\n", varResultName);
    fprintf(stdout, "}\n");
}

void GenerateIntSub(char* varResultName, char* var1Name, char* var2Name) {
    char msg[32];
	int num;

    if (getTyp(varResultName) != Int && (sscanf(varResultName, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", varResultName);
        yyerror(msg);
    }

    if (getTyp(var1Name) != Int && (sscanf(var1Name, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", var1Name);
        yyerror(msg);
    }

    if (getTyp(var2Name) != Int && (sscanf(var2Name, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", var2Name);
        yyerror(msg);
    }

    fprintf(stdout, "{\n");
    fprintf(stdout, "int difference = %s - %s;\n", var1Name, var2Name);
    fprintf(stdout, "%s = difference;\n", varResultName);
    fprintf(stdout, "}\n");
}

void GenerateIntMul(char* varResultName, char* var1Name, char* var2Name) {
    char msg[32];
	int num;

    if (getTyp(varResultName) != Int && (sscanf(varResultName, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", varResultName);
        yyerror(msg);
    }

    if (getTyp(var1Name) != Int && (sscanf(var1Name, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", var1Name);
        yyerror(msg);
    }

    if (getTyp(var2Name) != Int && (sscanf(var2Name, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", var2Name);
        yyerror(msg);
    }

    fprintf(stdout, "{\n");
    fprintf(stdout, "int product = %s * %s;\n", var1Name, var2Name);
    fprintf(stdout, "%s = product;\n", varResultName);
    fprintf(stdout, "}\n");
}

void GenerateIntDiv(char* varResultName, char* var1Name, char* var2Name) {
    char msg[32];
	int num;

    if (getTyp(varResultName) != Int && (sscanf(varResultName, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", varResultName);
        yyerror(msg);
    }

    if (getTyp(var1Name) != Int && (sscanf(var1Name, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", var1Name);
        yyerror(msg);
    }

    if (getTyp(var2Name) != Int && (sscanf(var2Name, "%d", &num) == 0)) {
        sprintf(msg, "%s not defined as an integer", var2Name);
        yyerror(msg);
    }

    fprintf(stdout, "{\n");
    fprintf(stdout, "if (%s == 0) {\n", var2Name);
    fprintf(stdout, "    fprintf(stderr, \"Division by zero error\\n\");\n");
    fprintf(stdout, "    exit(1);\n");
    fprintf(stdout, "}\n");
    fprintf(stdout, "int quotient = %s / %s;\n", var1Name, var2Name);
    fprintf(stdout, "%s = quotient;\n", varResultName);
    fprintf(stdout, "}\n");
}


// STR TYPE FUNCTIONS *******************************************

void GenerateStringDef(char* strVar){
	fprintf(stdout, "char* %s=NULL;\n", strVar);
	insert(strVar, Str);
}

void GenerateStrListDef(char* varList){
    char* token = strtok(varList, "@");
    while (token != NULL) {
        while (isspace((unsigned char)*token)) token++;
        if (*token == '\0') {
            token = strtok(NULL, "@");
            continue;
        }
        GenerateStringDef(token);
        insert(token, Str);
        token = strtok(NULL, "@");
    }
}

void GenerateStrAssign(char* var, char* str) {
    char msg[32];
    char *strippedStr;
    size_t strLen;

    if (getTyp(var) != Str) {
        sprintf(msg, "%s not defined as a string", var);
        yyerror(msg);
    }

    if (str[0] == '\"') {
        strLen = strlen(str) - 1;
        strippedStr = malloc(strLen + 1);
        if (strippedStr == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        strncpy(strippedStr, str + 1, strLen);
        strippedStr[strLen] = '\0';

        fprintf(stdout, "{\n");
        fprintf(stdout, "int len = strlen(\"%s\");\n", strippedStr);
        fprintf(stdout, "if (%s == NULL) %s = malloc(len + 1);\n", var, var);
        fprintf(stdout, "else %s = realloc(%s, len + 1);\n", var, var);
        fprintf(stdout, "strcpy(%s, \"%s\");\n", var, strippedStr);
        fprintf(stdout, "}\n");
        free(strippedStr);
    } else {
        if (getTyp(str) != Str) {
            sprintf(msg, "%s not defined as a string", str);
            yyerror(msg);
        }
        fprintf(stdout, "{\n");
        fprintf(stdout, "int len = strlen(%s);\n", str);
        fprintf(stdout, "if (%s == NULL) %s = malloc(len + 1);\n", var, var);
        fprintf(stdout, "else %s = realloc(%s, len + 1);\n", var, var);
        fprintf(stdout, "strcpy(%s, %s);\n", var, str);
        fprintf(stdout, "}\n");
    }
}

// SET TYPE FUNCTIONS *******************************************

void GenerateSetDef(char* setVar){
	fprintf(stdout, "int* %s=NULL;\n", setVar);
	insert(setVar, Set);
}

void GenerateSetListDef(char* varList){
    char* token = strtok(varList, "@");
    while (token != NULL) {
        while (isspace((unsigned char)*token)) token++;
        if (*token == '\0') {
            token = strtok(NULL, "@");
            continue;
        }
        GenerateSetDef(token);
        insert(token, Set);
        token = strtok(NULL, "@");
    }
}

int is_number(const char *input) {
	const char *p = input;
    int found_digit = FALSE;

    while (*p != '\0') {
        if (*p == '@') {
            return found_digit;
        }

        if (isdigit(*p)) {
            found_digit = TRUE;
        } else {
            return FALSE;
        }
        p++;
    }
    return found_digit;
}

void GenerateSetAssign(char* var, char* set){
	char msg[32];
	int num;
	char eq[] = "\"";

	if(strcmp(set, eq) != 0){
		if (getTyp(var)!= Set) {
			sprintf(msg, "%s not defined as a set", var);
			yyerror(msg);
		}

		if (getTyp(set)!= Set && (is_number(set) == FALSE)) {
			sprintf(msg, "%s not defined as a set", set);
			yyerror(msg);
		}

		fprintf(stdout, "{\n");
		fprintf(stdout, "int len;\n");

		if (set[0]=='\"')
			fprintf(stdout, "len = strlen(\"\\%s\");\n", set);
		else
			fprintf(stdout, "len = strlen(%s);\n", set);

		fprintf(stdout, "if (%s == NULL) %s=malloc(len+1);\n", var, var);
		fprintf(stdout, "else 	%s=realloc(%s, len+1);\n", var, var);

		if (set[0]=='\"')
			fprintf(stdout, "strcpy(%s,\"\\%s\");\n", var, set);
		else
			fprintf(stdout, "strcpy(%s, %s);\n", var, set);

		fprintf(stdout, "}\n");
	} else {
		fprintf(stdout, "%s= [];\n", var);
	}
}

%}

%union {char *str; int num;}
%token <str> t_STRING t_ID t_INT
%token t_COLLECTION_CMD t_OUTPUT_CMD t_INPUT_CMD t_IF t_ELSE t_EQ t_CurlB t_INTER
%token t_INT_CMD t_STR_CMD t_SET_CMD
%type <str> STRING_LIST INT_LIST t_OPERATOR
%type <str> VAR VAR_LIST COLLECTION SET INT STR IF_STATEMENT ELSE_STATEMENT EXPRESSION


%%
Prog :				SENTENCE
	|				Prog SENTENCE
SENTENCE:			t_COLLECTION_CMD VAR ';'				{GenerateColDef($2);}
	|				t_COLLECTION_CMD VAR_LIST ';'			{GenerateColListDef($2);}
	|				t_SET_CMD VAR ';'						{GenerateSetDef($2);}
	|				t_SET_CMD VAR_LIST ';'					{GenerateSetListDef($2);}
	|				t_INT_CMD VAR ';'						{GenerateIntDef($2);}
	|				t_INT_CMD VAR_LIST ';'					{GenerateIntListDef($2);}
	|				t_STR_CMD VAR ';'						{GenerateStringDef($2);}
	|				t_STR_CMD VAR_LIST ';'					{GenerateStrListDef($2);}
	|				VAR '=' COLLECTION ';'					{GenerateColAssign($1,$3);}
	|				VAR '=' INT ';'							{GenerateIntAssign($1,$3);}
	|				VAR '=' STR ';'							{GenerateStrAssign($1,$3);}
	|				VAR '=' SET ';'							{GenerateSetAssign($1,$3);}
	|				VAR '=' INT '+' INT ';'					{GenerateIntAdd($1, $3, $5);}
	|	 			VAR '=' VAR '+' VAR ';'					{GenerateIntAdd($1, $3, $5);}
	|	 			VAR '=' VAR '+' INT ';'					{GenerateIntAdd($1, $3, $5);}
	|	 			VAR '=' INT '+' VAR ';'					{GenerateIntAdd($1, $3, $5);}
	|				VAR '=' INT '-' INT ';'					{GenerateIntSub($1, $3, $5);}
	|	 			VAR '=' VAR '-' VAR ';'					{GenerateIntSub($1, $3, $5);}
	|	 			VAR '=' VAR '-' INT ';'					{GenerateIntSub($1, $3, $5);}
	|	 			VAR '=' INT '-' VAR ';'					{GenerateIntSub($1, $3, $5);}
	|				VAR '=' INT '*' INT ';'					{GenerateIntMul($1, $3, $5);}
	|	 			VAR '=' VAR '*' VAR ';'					{GenerateIntMul($1, $3, $5);}
	|	 			VAR '=' VAR '*' INT ';'					{GenerateIntMul($1, $3, $5);}
	|	 			VAR '=' INT '*' VAR ';'					{GenerateIntMul($1, $3, $5);}
	|				VAR '=' INT '/' INT ';'					{GenerateIntDiv($1, $3, $5);}
	|	 			VAR '=' VAR '/' VAR ';'					{GenerateIntDiv($1, $3, $5);}
	|	 			VAR '=' VAR '/' INT ';'					{GenerateIntDiv($1, $3, $5);}
	|	 			VAR '=' INT '/' VAR ';'					{GenerateIntDiv($1, $3, $5);}
	| 				VAR '=' VAR '+' COLLECTION ';'          {GenerateColUnify($1, $3, $5);}
	| 				VAR '=' COLLECTION '+' VAR ';'          {GenerateColUnify($1, $3, $5);}
	| 				VAR '=' VAR '+' t_STRING {$5=CopyStr(yytext);} ';'   							{GenerateColUnify($1, $3, $5);}
    /* | 				VAR '=' VAR '-' COLLECTION ';'          { GenerateColDifference($1, $3, $5); } */
    | 				VAR '=' VAR '&' COLLECTION ';'          { GenerateColIntersection($1, $3, $5); }
    /* | 				VAR '=' '|' COLLECTION '|' ';'          { GenerateColSize($1, $4); } */
	|				IF_STATEMENT
	|				ELSE_STATEMENT
	|				OUTPUT
	|				INPUT
OUTPUT :			t_OUTPUT_CMD t_STRING {$2=CopyStr(yytext);} EXPRESSION ';'						{GenerateColOut($2, $4);}
EXPRESSION:			COLLECTION '&' COLLECTION				{intersectionCol($1, $3);}
	|				COLLECTION '+' COLLECTION				{unionCol($1, $3);}
	|				COLLECTION '-' '(' EXPRESSION ')'		{subCol($1, $4);}
	|				COLLECTION
	|				/* empty */                    			{ $$ = NULL; }
INPUT: 				t_INPUT_CMD t_STRING VAR ';'			{ GenerateColInput($2, $3); }
IF_STATEMENT :     	t_IF '(' VAR t_OPERATOR VAR ')'			{printf("if (%s %s %s)\n", $3, $4, $5);}
	|				t_IF '(' VAR '&' VAR t_OPERATOR t_OPERATOR ')'	{printf("if (%s & %s %s %s)\n", $3, $5, $6, $7);}
	|				t_IF '(' VAR ')'						{printf("if (%s)\n", $3);}
ELSE_STATEMENT:   	t_ELSE									{printf("else\n");}
t_OPERATOR :    	t_EQ
	|  				t_CurlB
	|  				t_INTER
COLLECTION :		VAR										{$$=CopyStr($1);}
	|				'{' '}'									{$$ = "\"";}
	|				'{' STRING_LIST '}'						{$$ = $2;}
	|			    t_CurlB									{$$ = CopyStr(yytext);}
SET :				VAR										{$$= ($1);}
	|				'[' ']'									{$$ = "\"";}
	|				'[' INT_LIST ']'						{$$ = $2;}
INT :				VAR										{$$=CopyInt($1);}
 	|				INT_LIST								{$$ = $1;}
STR :				VAR										{$$=CopyInt($1);}
 	|				STRING_LIST								{$$ = $1;}
VAR :				t_ID									{$$ = CopyStr(yytext)}
VAR_LIST:			VAR_LIST ',' VAR                        { $$ = AddStrToList($1, $3); }
    | 				VAR                                     { $$ = CopyStr($1); }
INT_LIST :			INT_LIST ',' t_INT						{ $1 = AddIntToList($1, yytext); }
	|				t_INT									{$$ = CopyInt(yytext);}
STRING_LIST :		STRING_LIST ',' t_STRING				{$$ = AddStrToList($1, yytext);}
	|				t_STRING								{$$ = CopyStr(yytext);}
%%