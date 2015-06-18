
#include "opc.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <uart.h>

struct opc_element {
	char name[OPC_NAMEMAXLEN];
	void *keeper;
};

int opc_cnt;
struct opc_element records[OPC_MAXELEMENTS];


static struct opc_element *get_label_reckord(char *name) {
	int i;
	i = 0;
	while(i < opc_cnt) {
		if(strcmp(&records[i].name, name) == 0) {
			return &records[i];
		}
		++i;
	}
	return NULL;
}


int opc_bind(char *name, void *keeper)
{
	struct opc_element *e;

	if(opc_cnt >= (OPC_MAXELEMENTS - 1))
		return -1;

	e = &records[opc_cnt];
	++opc_cnt;

	memcpy(&e->name, name, sizeof e->name);
	e->keeper = keeper;
	return 0;
}


static void opc_reply(int value) {
	printf("%d\n", value);
}

static void opc_error(int errno) {
	if(errno == OPC_ERROR_OK)
		return;
	printf("ERROR %d\n", errno);
}

static int opc_set(char *s) {
	char name[OPC_NAMEMAXLEN];
	int value;
	value = 0;
	sscanf(s, "%s %d", name, &value);
	/*fflush(stdin);*/

	struct opc_element *e;
	if((e = get_label_reckord(name)) == NULL) {
		return OPC_ERROR_BADNAME;
	}

	((int(*)(int, char))e->keeper)(value, 'w');
	return OPC_ERROR_OK;
}

static int opc_get(char *s) {
	char name[OPC_NAMEMAXLEN];
	sscanf(s, "%s", name);

	struct opc_element *e;
	if((e = get_label_reckord(name)) == NULL) {
		return OPC_ERROR_BADNAME;
	}

	opc_reply(((int(*)(int, char))e->keeper)(0, 'r'));
	return OPC_ERROR_OK;
}

static int opc_call(char *s) {
	char name[OPC_NAMEMAXLEN];
	int value;
	value = 0;
	sscanf(s, "%s %d", name, &value);

	struct opc_element *e;
	if((e = get_label_reckord(name)) == NULL) {
		return OPC_ERROR_BADNAME;
	}

	opc_reply(((int(*)(int))e->keeper)(value));
	
	return OPC_ERROR_OK;
}

int opc_serve(char *s) {
	int retval;
	retval = OPC_ERROR_BADCMD;
	if(strncmp(s, "set", 3) == 0) {
		retval = opc_set((s + 4));
	} else if(strncmp(s, "get", 3) == 0) {
		retval = opc_get((s + 4));
	} else if(strncmp(s, "call", 4) == 0) {
		retval = opc_call((s + 5));
	}

	opc_error(retval);
	return retval;
}




