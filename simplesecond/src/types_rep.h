#ifndef TYPES_REP_H
#define TYPES_REP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
//#include "./sqlite/sqlite_rep.h"


#define MAX_DEVICES 1000
#define MAX_CONTACT 3000
#define MAX_NAME_DEV 256
//#define COUNT_COND 3
#define DATA_BASE "scb_base.db"

typedef enum {
	FIND_OK, // замечаний нет
	FIND_INCORRECT_DEV_NAME, // некорректное имя 
	FIND_NAN_PLUS, // не нашли плюс
	FIND_NAN_MINUS, // не нашли минус
	FIND_NAN_CONTACT,
	FIND_NAN_CONTACT_PLUS,
	FIND_NAN_CONTACT_MINUS,
	FIND_NAN_COND_OR_DONT_COND,
	FIND_NAN_LINKS_P,  // не дошли до плюса
	FIND_NAN_LINKS_M // не дошли до минуса 
}FindEvent;
#define MAX_FINDEVENT 10 /*Нужны для функций которые будут принимать диапазон значений*/
#define MIN_FINDEVENT 0
/*Может хранить в app и сделать методы для очистки, веделения памяти
*/
typedef struct {
	char name[MAX_NAME_DEV];
	struct ItemStr *next;
} ItemStr;


typedef struct {
	int event;
	char name[MAX_NAME_DEV];
	ItemStr *head;
}FindCtx;

typedef enum {
	MINUS,
	NMSH_1_1440,
	NMSH_1_400,
	_1N_1350,
	_1NM_950,
	PLUS
} DevType;

#define MAX_DEVTYPE PLUS
#define MIN_DEVTYPE MINUS

/*Условия подтока и безтока должны идти строго по порядку
подточено-обесточено какого то действия, так как на этом завязана логика добавления 
условий подтачивания-обестачивания*/
typedef enum {
	TRACK_CLEAR = 10, 
	TRACK_OCUPP = 11,
	CLICK_BTN = 20,
	DONT_CLICK_BTN = 21,
	ROUTE = 30,
	DONT_ROUTE = 31
} CondEvent;
#define MAX_CONDEVENT 3 /*Нужны для функций которые будут принимать диапазон значений*/
#define MIN_CONDEVENT 1 
 
typedef struct {
	CondEvent event;
	int number;
	char prefix[4];
} Condition;

typedef struct {
	Condition *condition;
	struct CondNode *next;
} CondNode;

typedef struct {
	CondNode *cond_node;
	int idx_source_dev; // индекс устройства в app->devices
	struct CondList *next_list;
}CondList;

typedef struct {
	int idx_graph;
	int idx_source_dev; // индекс самого устройства в app->devices
	int idx_con_dev; // индекс контакта в dev->contacts
	
	int axial;
	int second;
	int dev_state;
//	CondNode *cond_node[COUNT_COND];
 	CondList *cond_list;
	//struct DevScb *source_dev;
	
	//struct Relay *target_dev;
	//struct Contact *next;
} Contact;

typedef struct {
	Contact *contact;
	struct ContactNode *next;
} ContactNode;

typedef struct {
	ContactNode *contact_node;
	struct ContactList *next_list;
} ContactList;

typedef struct {
	//Contact *to_contact;
	int to_idx_device;
	int to_idx_contact;
	struct Edge *next;
} Edge;

typedef struct {
	//Contact *from_contact;
	int from_idx_device; // app->devices
	int from_idx_contact; // dev->contacts
	Edge *head;
} Vertex;


typedef struct{
	int idx;
	
	char name[MAX_NAME_DEV];
	//char plus[MAX_NAME_DEV]; // привязать плюсы к реле чтобы по графу во время прострела не скакать
	
	DevType type;
	
	Contact *contacts;
	int count_contacts;
	
	CondList *cond_list;
	CondList *dont_cond_list;
	
} DevScb;

typedef struct {
	DevScb dev;
	int next_bucket;
} NodeDevScb;


/*Доступ и добавление объектов осуществяется за счет бакетов, пропускаем через хэш функцию 
имя устройства и берем отстаток от деления на количество выделенной памяти для устройств, это будет
индекс бакета в котором будет храниться индекс(смежение) от указателя devices, который берется от
переменной count_devices, после удачного добавления count_devices инкрементируется и следующий объект
который будет добавляться как раз пойдет дальше по памяти, для исключения коллизий в добавляемом 
объекте будет храниться индекс следующего бакета, получается что то типа односвязного списка, 
для этого при инициализации программы заполняем массив бакетов значением -1, получив индекс бакета
по значению которое в нем хранится переходим в массив devices, сравниваем имена если 
сопадает возвщаем объект(get_dev) или -1(add_dev) в зависимости от задачи, 
если совпадений нет, смотрим на next_bucket если там -1 то дальше идти некуда, 
иначе берем это значение и дальше прыгаем в массив devices.



*/
typedef struct {
	NodeDevScb *devices;
//	int buckets[MAX_DEVICES];
	int *buckets;
	int capacity_devices;
	int count_devices;

	sqlite3 *db;
	
//	Vertex graph[MAX_CONTACT];
	Vertex *graph;
	int count_contacts;
	int capacity_contacts;
	//int visited[MAX_CONTACT];
	int *visited;

	int running;
} AppState;

#endif
