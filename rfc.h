
#ifndef __OPC__
#define __OPC__


#define OPC_ERROR_OK 0 /*все в порядке, ошибок нет*/
#define OPC_ERROR_LINK 1 /*нет связи с устройством*/
#define OPC_ERROR_BADCMD 2 /*неверная команда*/
#define OPC_ERROR_BADNAME 3 /*нет такого элемента*/
#define OPC_ERROR_ACCESS 4 /*нет прав чтения/записи для этого элемента*/


#define OPC_MAXELEMENTS	32
#define OPC_NAMEMAXLEN	16

int opc_bind(char *name, void *keeper);
int opc_serve(char *s);

#endif /* __OPC__ */
