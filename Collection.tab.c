/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     t_STRING = 258,
     t_ID = 259,
     t_INT = 260,
     t_COLLECTION_CMD = 261,
     t_OUTPUT_CMD = 262,
     t_INPUT_CMD = 263,
     t_IF = 264,
     t_ELSE = 265,
     t_EQ = 266,
     t_CurlB = 267,
     t_INTER = 268,
     t_INT_CMD = 269,
     t_STR_CMD = 270,
     t_SET_CMD = 271
   };
#endif
/* Tokens.  */
#define t_STRING 258
#define t_ID 259
#define t_INT 260
#define t_COLLECTION_CMD 261
#define t_OUTPUT_CMD 262
#define t_INPUT_CMD 263
#define t_IF 264
#define t_ELSE 265
#define t_EQ 266
#define t_CurlB 267
#define t_INTER 268
#define t_INT_CMD 269
#define t_STR_CMD 270
#define t_SET_CMD 271




/* Copy the first part of user declarations.  */
#line 1 "Collection.y"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 772 "Collection.y"
{char *str; int num;}
/* Line 193 of yacc.c.  */
#line 901 "Collection.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 914 "Collection.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  28
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   162

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNRULES -- Number of states.  */
#define YYNSTATES  146

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   271

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    23,     2,
      24,    25,    21,    19,    30,    20,     2,    22,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    17,
       2,    18,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    28,     2,    29,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,     2,    27,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    12,    16,    20,    24,    28,
      32,    36,    40,    45,    50,    55,    60,    67,    74,    81,
      88,    95,   102,   109,   116,   123,   130,   137,   144,   151,
     158,   165,   172,   179,   186,   187,   195,   202,   204,   206,
     208,   210,   211,   217,   221,   225,   231,   233,   234,   239,
     246,   255,   260,   262,   264,   266,   268,   270,   273,   277,
     279,   281,   284,   288,   290,   292,   294,   296,   298,   302,
     304,   308,   310,   314
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      32,     0,    -1,    33,    -1,    32,    33,    -1,     6,    46,
      17,    -1,     6,    47,    17,    -1,    16,    46,    17,    -1,
      16,    47,    17,    -1,    14,    46,    17,    -1,    14,    47,
      17,    -1,    15,    46,    17,    -1,    15,    47,    17,    -1,
      46,    18,    42,    17,    -1,    46,    18,    44,    17,    -1,
      46,    18,    45,    17,    -1,    46,    18,    43,    17,    -1,
      46,    18,    44,    19,    44,    17,    -1,    46,    18,    46,
      19,    46,    17,    -1,    46,    18,    46,    19,    44,    17,
      -1,    46,    18,    44,    19,    46,    17,    -1,    46,    18,
      44,    20,    44,    17,    -1,    46,    18,    46,    20,    46,
      17,    -1,    46,    18,    46,    20,    44,    17,    -1,    46,
      18,    44,    20,    46,    17,    -1,    46,    18,    44,    21,
      44,    17,    -1,    46,    18,    46,    21,    46,    17,    -1,
      46,    18,    46,    21,    44,    17,    -1,    46,    18,    44,
      21,    46,    17,    -1,    46,    18,    44,    22,    44,    17,
      -1,    46,    18,    46,    22,    46,    17,    -1,    46,    18,
      46,    22,    44,    17,    -1,    46,    18,    44,    22,    46,
      17,    -1,    46,    18,    46,    19,    42,    17,    -1,    46,
      18,    42,    19,    46,    17,    -1,    -1,    46,    18,    46,
      19,     3,    34,    17,    -1,    46,    18,    46,    23,    42,
      17,    -1,    39,    -1,    40,    -1,    35,    -1,    38,    -1,
      -1,     7,     3,    36,    37,    17,    -1,    42,    23,    42,
      -1,    42,    19,    42,    -1,    42,    20,    24,    37,    25,
      -1,    42,    -1,    -1,     8,     3,    46,    17,    -1,     9,
      24,    46,    41,    46,    25,    -1,     9,    24,    46,    23,
      46,    41,    41,    25,    -1,     9,    24,    46,    25,    -1,
      10,    -1,    11,    -1,    12,    -1,    13,    -1,    46,    -1,
      26,    27,    -1,    26,    49,    27,    -1,    12,    -1,    46,
      -1,    28,    29,    -1,    28,    48,    29,    -1,    46,    -1,
      48,    -1,    46,    -1,    49,    -1,     4,    -1,    47,    30,
      46,    -1,    46,    -1,    48,    30,     5,    -1,     5,    -1,
      49,    30,     3,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   781,   781,   782,   783,   784,   785,   786,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   807,   808,
     809,   810,   811,   812,   813,   813,   815,   817,   818,   819,
     820,   821,   821,   822,   823,   824,   825,   826,   827,   828,
     829,   830,   831,   832,   833,   834,   835,   836,   837,   838,
     839,   840,   841,   842,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "t_STRING", "t_ID", "t_INT",
  "t_COLLECTION_CMD", "t_OUTPUT_CMD", "t_INPUT_CMD", "t_IF", "t_ELSE",
  "t_EQ", "t_CurlB", "t_INTER", "t_INT_CMD", "t_STR_CMD", "t_SET_CMD",
  "';'", "'='", "'+'", "'-'", "'*'", "'/'", "'&'", "'('", "')'", "'{'",
  "'}'", "'['", "']'", "','", "$accept", "Prog", "SENTENCE", "@1",
  "OUTPUT", "@2", "EXPRESSION", "INPUT", "IF_STATEMENT", "ELSE_STATEMENT",
  "t_OPERATOR", "COLLECTION", "SET", "INT", "STR", "VAR", "VAR_LIST",
  "INT_LIST", "STRING_LIST", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,    59,    61,    43,
      45,    42,    47,    38,    40,    41,   123,   125,    91,    93,
      44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    31,    32,    32,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    34,    33,    33,    33,    33,    33,
      33,    36,    35,    37,    37,    37,    37,    37,    38,    39,
      39,    39,    40,    41,    41,    41,    42,    42,    42,    42,
      43,    43,    43,    44,    44,    45,    45,    46,    47,    47,
      48,    48,    49,    49
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     4,     4,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     0,     7,     6,     1,     1,     1,
       1,     0,     5,     3,     3,     5,     1,     0,     4,     6,
       8,     4,     1,     1,     1,     1,     1,     2,     3,     1,
       1,     2,     3,     1,     1,     1,     1,     1,     3,     1,
       3,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    67,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     2,    39,    40,    37,    38,     0,    69,     0,    41,
       0,     0,    69,     0,    69,     0,    69,     0,     1,     3,
       0,     4,     5,     0,    47,     0,     0,     8,     9,    10,
      11,     6,     7,    73,    71,    59,     0,     0,     0,     0,
       0,     0,    56,    64,    66,    68,     0,    46,    56,    48,
      53,    54,    55,     0,    51,     0,    57,     0,    61,     0,
      12,     0,    15,    13,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
       0,     0,    58,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    70,    72,    44,    47,    43,     0,
      49,    33,    16,    19,    20,    23,    24,    27,    28,    31,
       0,    32,    18,    17,    22,    21,    26,    25,    30,    29,
      36,     0,     0,    35,    45,    50
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    10,    11,   130,    12,    34,    56,    13,    14,    15,
      65,    57,    49,    50,    51,    58,    18,    53,    54
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -88
static const yytype_int16 yypact[] =
{
      96,   -88,    20,    37,    46,    28,   -88,    20,    20,    20,
      83,   -88,   -88,   -88,   -88,   -88,    52,    55,    14,   -88,
      20,    20,    56,    17,    61,    21,    71,    29,   -88,   -88,
       1,   -88,   -88,    20,    13,    84,    30,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,   -88,   -88,     8,     7,     9,    90,
      47,    91,   103,    79,    97,   -88,   100,    -1,   -88,   -88,
     -88,   -88,   -88,    20,   -88,    20,   -88,    18,   -88,    27,
     -88,    20,   -88,   -88,    57,    57,    57,    57,   -88,    11,
      57,    57,    57,    13,   123,   126,   -88,    13,   106,    13,
      73,   107,   -88,   -88,   114,   116,   117,   118,   119,   120,
     121,   122,   124,   -88,   125,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   -88,   -88,   -88,    13,   -88,    73,
     -88,   -88,   -88,   -88,   -88,   -88,   -88,   -88,   -88,   -88,
     136,   -88,   -88,   -88,   -88,   -88,   -88,   -88,   -88,   -88,
     -88,   115,   137,   -88,   -88,   -88
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -88,   -88,   144,   -88,   -88,   -88,    26,   -88,   -88,   -88,
     -87,   -29,   -88,    39,   -88,     0,    87,   108,   110
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      16,    48,    17,   119,    43,     1,    44,    22,    24,    26,
      16,    43,    44,    45,   103,     1,    44,     1,    87,    88,
      35,    36,    89,    45,     1,    45,    70,    46,    71,    47,
      52,    32,   142,    55,    38,    66,    68,    46,    40,    46,
      19,    60,    61,    62,    33,    92,    42,    33,    85,    20,
     104,    33,    21,    63,   113,    64,    93,    84,   116,    33,
     118,     1,    44,    90,    73,    91,    74,    75,    76,    77,
      30,    94,    31,    37,    96,    98,   100,   102,    39,   106,
     108,   110,   112,    28,    60,    61,    62,     1,    41,     2,
       3,     4,     5,     6,    23,    25,    27,     7,     8,     9,
       1,    59,     2,     3,     4,     5,     6,    72,    78,    84,
       7,     8,     9,    95,    97,    99,   101,    86,   105,   107,
     109,   111,    79,    80,    81,    82,    83,    85,   114,   115,
     117,   121,   120,   122,   123,   124,   125,   126,   127,   128,
     144,   129,   131,   141,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   143,    29,    69,    67,     0,     0,     0,
       0,     0,   145
};

static const yytype_int8 yycheck[] =
{
       0,    30,     2,    90,     3,     4,     5,     7,     8,     9,
      10,     3,     5,    12,     3,     4,     5,     4,    19,    20,
      20,    21,    23,    12,     4,    12,    17,    26,    19,    28,
      30,    17,   119,    33,    17,    27,    29,    26,    17,    26,
       3,    11,    12,    13,    30,    27,    17,    30,    30,     3,
      79,    30,    24,    23,    83,    25,    29,    30,    87,    30,
      89,     4,     5,    63,    17,    65,    19,    20,    21,    22,
      18,    71,    17,    17,    74,    75,    76,    77,    17,    79,
      80,    81,    82,     0,    11,    12,    13,     4,    17,     6,
       7,     8,     9,    10,     7,     8,     9,    14,    15,    16,
       4,    17,     6,     7,     8,     9,    10,    17,    17,    30,
      14,    15,    16,    74,    75,    76,    77,    17,    79,    80,
      81,    82,    19,    20,    21,    22,    23,    30,     5,     3,
      24,    17,    25,    17,    17,    17,    17,    17,    17,    17,
      25,    17,    17,   117,    17,    17,    17,    17,    17,    17,
      17,    17,    17,    17,    10,    47,    46,    -1,    -1,    -1,
      -1,    -1,    25
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     6,     7,     8,     9,    10,    14,    15,    16,
      32,    33,    35,    38,    39,    40,    46,    46,    47,     3,
       3,    24,    46,    47,    46,    47,    46,    47,     0,    33,
      18,    17,    17,    30,    36,    46,    46,    17,    17,    17,
      17,    17,    17,     3,     5,    12,    26,    28,    42,    43,
      44,    45,    46,    48,    49,    46,    37,    42,    46,    17,
      11,    12,    13,    23,    25,    41,    27,    49,    29,    48,
      17,    19,    17,    17,    19,    20,    21,    22,    17,    19,
      20,    21,    22,    23,    30,    30,    17,    19,    20,    23,
      46,    46,    27,    29,    46,    44,    46,    44,    46,    44,
      46,    44,    46,     3,    42,    44,    46,    44,    46,    44,
      46,    44,    46,    42,     5,     3,    42,    24,    42,    41,
      25,    17,    17,    17,    17,    17,    17,    17,    17,    17,
      34,    17,    17,    17,    17,    17,    17,    17,    17,    17,
      17,    37,    41,    17,    25,    25
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 783 "Collection.y"
    {GenerateColDef((yyvsp[(2) - (3)].str));;}
    break;

  case 5:
#line 784 "Collection.y"
    {GenerateColListDef((yyvsp[(2) - (3)].str));;}
    break;

  case 6:
#line 785 "Collection.y"
    {GenerateSetDef((yyvsp[(2) - (3)].str));;}
    break;

  case 7:
#line 786 "Collection.y"
    {GenerateSetListDef((yyvsp[(2) - (3)].str));;}
    break;

  case 8:
#line 787 "Collection.y"
    {GenerateIntDef((yyvsp[(2) - (3)].str));;}
    break;

  case 9:
#line 788 "Collection.y"
    {GenerateIntListDef((yyvsp[(2) - (3)].str));;}
    break;

  case 10:
#line 789 "Collection.y"
    {GenerateStringDef((yyvsp[(2) - (3)].str));;}
    break;

  case 11:
#line 790 "Collection.y"
    {GenerateStrListDef((yyvsp[(2) - (3)].str));;}
    break;

  case 12:
#line 791 "Collection.y"
    {GenerateColAssign((yyvsp[(1) - (4)].str),(yyvsp[(3) - (4)].str));;}
    break;

  case 13:
#line 792 "Collection.y"
    {GenerateIntAssign((yyvsp[(1) - (4)].str),(yyvsp[(3) - (4)].str));;}
    break;

  case 14:
#line 793 "Collection.y"
    {GenerateStrAssign((yyvsp[(1) - (4)].str),(yyvsp[(3) - (4)].str));;}
    break;

  case 15:
#line 794 "Collection.y"
    {GenerateSetAssign((yyvsp[(1) - (4)].str),(yyvsp[(3) - (4)].str));;}
    break;

  case 16:
#line 795 "Collection.y"
    {GenerateIntAdd((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 17:
#line 796 "Collection.y"
    {GenerateIntAdd((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 18:
#line 797 "Collection.y"
    {GenerateIntAdd((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 19:
#line 798 "Collection.y"
    {GenerateIntAdd((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 20:
#line 799 "Collection.y"
    {GenerateIntSub((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 21:
#line 800 "Collection.y"
    {GenerateIntSub((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 22:
#line 801 "Collection.y"
    {GenerateIntSub((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 23:
#line 802 "Collection.y"
    {GenerateIntSub((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 24:
#line 803 "Collection.y"
    {GenerateIntMul((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 25:
#line 804 "Collection.y"
    {GenerateIntMul((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 26:
#line 805 "Collection.y"
    {GenerateIntMul((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 27:
#line 806 "Collection.y"
    {GenerateIntMul((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 28:
#line 807 "Collection.y"
    {GenerateIntDiv((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 29:
#line 808 "Collection.y"
    {GenerateIntDiv((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 30:
#line 809 "Collection.y"
    {GenerateIntDiv((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 31:
#line 810 "Collection.y"
    {GenerateIntDiv((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 32:
#line 811 "Collection.y"
    {GenerateColUnify((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 33:
#line 812 "Collection.y"
    {GenerateColUnify((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 34:
#line 813 "Collection.y"
    {(yyvsp[(5) - (5)].str)=CopyStr(yytext);;}
    break;

  case 35:
#line 813 "Collection.y"
    {GenerateColUnify((yyvsp[(1) - (7)].str), (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].str));;}
    break;

  case 36:
#line 815 "Collection.y"
    { GenerateColIntersection((yyvsp[(1) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].str)); ;}
    break;

  case 41:
#line 821 "Collection.y"
    {(yyvsp[(2) - (2)].str)=CopyStr(yytext);;}
    break;

  case 42:
#line 821 "Collection.y"
    {GenerateColOut((yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].str));;}
    break;

  case 43:
#line 822 "Collection.y"
    {intersectionCol((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str));;}
    break;

  case 44:
#line 823 "Collection.y"
    {unionCol((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str));;}
    break;

  case 45:
#line 824 "Collection.y"
    {subCol((yyvsp[(1) - (5)].str), (yyvsp[(4) - (5)].str));;}
    break;

  case 47:
#line 826 "Collection.y"
    { (yyval.str) = NULL; ;}
    break;

  case 48:
#line 827 "Collection.y"
    { GenerateColInput((yyvsp[(2) - (4)].str), (yyvsp[(3) - (4)].str)); ;}
    break;

  case 49:
#line 828 "Collection.y"
    {printf("if (%s %s %s)\n", (yyvsp[(3) - (6)].str), (yyvsp[(4) - (6)].str), (yyvsp[(5) - (6)].str));;}
    break;

  case 50:
#line 829 "Collection.y"
    {printf("if (%s & %s %s %s)\n", (yyvsp[(3) - (8)].str), (yyvsp[(5) - (8)].str), (yyvsp[(6) - (8)].str), (yyvsp[(7) - (8)].str));;}
    break;

  case 51:
#line 830 "Collection.y"
    {printf("if (%s)\n", (yyvsp[(3) - (4)].str));;}
    break;

  case 52:
#line 831 "Collection.y"
    {printf("else\n");;}
    break;

  case 56:
#line 835 "Collection.y"
    {(yyval.str)=CopyStr((yyvsp[(1) - (1)].str));;}
    break;

  case 57:
#line 836 "Collection.y"
    {(yyval.str) = "\"";;}
    break;

  case 58:
#line 837 "Collection.y"
    {(yyval.str) = (yyvsp[(2) - (3)].str);;}
    break;

  case 59:
#line 838 "Collection.y"
    {(yyval.str) = CopyStr(yytext);;}
    break;

  case 60:
#line 839 "Collection.y"
    {(yyval.str)= ((yyvsp[(1) - (1)].str));;}
    break;

  case 61:
#line 840 "Collection.y"
    {(yyval.str) = "\"";;}
    break;

  case 62:
#line 841 "Collection.y"
    {(yyval.str) = (yyvsp[(2) - (3)].str);;}
    break;

  case 63:
#line 842 "Collection.y"
    {(yyval.str)=CopyInt((yyvsp[(1) - (1)].str));;}
    break;

  case 64:
#line 843 "Collection.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str);;}
    break;

  case 65:
#line 844 "Collection.y"
    {(yyval.str)=CopyInt((yyvsp[(1) - (1)].str));;}
    break;

  case 66:
#line 845 "Collection.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str);;}
    break;

  case 67:
#line 846 "Collection.y"
    {(yyval.str) = CopyStr(yytext);}
    break;

  case 68:
#line 847 "Collection.y"
    { (yyval.str) = AddStrToList((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str)); ;}
    break;

  case 69:
#line 848 "Collection.y"
    { (yyval.str) = CopyStr((yyvsp[(1) - (1)].str)); ;}
    break;

  case 70:
#line 849 "Collection.y"
    { (yyvsp[(1) - (3)].str) = AddIntToList((yyvsp[(1) - (3)].str), yytext); ;}
    break;

  case 71:
#line 850 "Collection.y"
    {(yyval.str) = CopyInt(yytext);;}
    break;

  case 72:
#line 851 "Collection.y"
    {(yyval.str) = AddStrToList((yyvsp[(1) - (3)].str), yytext);;}
    break;

  case 73:
#line 852 "Collection.y"
    {(yyval.str) = CopyStr(yytext);;}
    break;


/* Line 1267 of yacc.c.  */
#line 2549 "Collection.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 853 "Collection.y"

