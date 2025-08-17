typedef enum {Collection=1, Set, Int, Str} varType;

struct {
	char *name;
	varType typ;
} SymTable[100];
