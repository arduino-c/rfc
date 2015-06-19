

#include "rfc.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <uart.h>

struct rfc_element {
	char name[OPC_NAMEMAXLEN];
	void *keeper;
};

int rfc_cnt;
struct rfc_element records[OPC_MAXELEMENTS];


static struct rfc_element *get_label_reckord(char *name) {
	int i;
	i = 0;
	while(i < rfc_cnt) {
		if(strcmp(&records[i].name, name) == 0) {
			return &records[i];
		}
		++i;
	}
	return NULL;
}


int rfc_bind(char *name, void *keeper)
{
	struct rfc_element *e;

	if(rfc_cnt >= (OPC_MAXELEMENTS - 1))
		return -1;

	e = &records[rfc_cnt];
	++rfc_cnt;

	memcpy(&e->name, name, sizeof e->name);
	e->keeper = keeper;
	return 0;
}


static void rfc_reply(int value) {
	printf("%d\n", value);
}

static void rfc_error(int errno) {
	if(errno == OPC_ERROR_OK)
		return;
	printf("ERROR %d\n", errno);
}

static int rfc_set(char *s) {
	char name[OPC_NAMEMAXLEN];
	int value;
	value = 0;
	sscanf(s, "%s %d", name, &value);
	/*fflush(stdin);*/

	struct rfc_element *e;
	if((e = get_label_reckord(name)) == NULL) {
		return OPC_ERROR_BADNAME;
	}

	((int(*)(int, char))e->keeper)(value, 'w');
	return OPC_ERROR_OK;
}

static int rfc_get(char *s) {
	char name[OPC_NAMEMAXLEN];
	sscanf(s, "%s", name);

	struct rfc_element *e;
	if((e = get_label_reckord(name)) == NULL) {
		return OPC_ERROR_BADNAME;
	}

	rfc_reply(((int(*)(int, char))e->keeper)(0, 'r'));
	return OPC_ERROR_OK;
}

static int rfc_call(char *s) {
	char name[OPC_NAMEMAXLEN];
	int value;
	value = 0;
	sscanf(s, "%s %d", name, &value);

	struct rfc_element *e;
	if((e = get_label_reckord(name)) == NULL) {
		return OPC_ERROR_BADNAME;
	}

	rfc_reply(((int(*)(int))e->keeper)(value));
	
	return OPC_ERROR_OK;
}

int rfc_serve(char *s) {
	int retval;
	retval = OPC_ERROR_BADCMD;
	if(strncmp(s, "set", 3) == 0) {
		retval = rfc_set((s + 4));
	} else if(strncmp(s, "get", 3) == 0) {
		retval = rfc_get((s + 4));
	} else if(strncmp(s, "call", 4) == 0) {
		retval = rfc_call((s + 5));
	}

	rfc_error(retval);
	return retval;
}


void rfc_init() {
	
}

