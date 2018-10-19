#include "inc.h"
#include <minix/vm.h>

struct hook_entry {
	int key;
	void (*function)(void);
	char *name;
} hooks[] = {
	{ SF7,  pscount_dmp, "New: Count how many processes" }
};

