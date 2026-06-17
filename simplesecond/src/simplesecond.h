#ifndef SIMPLESECOND_H
#define SIMPLESECOND_H

#include "types_rep.h"
#include "./aux/aux.h"
/*Инициализируем основные компоненты программы, загружаем устройства с базы
данных если они есть, загужаем с базы связи и условия подтачивания для базовых реле

typedef struct {

	NodeDevScb *devices;   // выделяем память, заполняем dev->next_bucket -1
	int *buckets;          // выделяем память, заполняем -1
	int capacity_devices;  // изначально MAX_DEVICES
	int count_devices; (0) //изначально 0,
							 инкрементируется по мере загрузки устройств с базы 

	sqlite3 *db;            // создаем если отсутствует, 
	                           если есть то открываем существующую
	
	Vertex *graph;          // выделяем память
	int count_contacts;     //изначально 0, инкрементируется 
	                          по мере создания устройств
	int capacity_contacts; // изначально MAX_CONTACTS
	int *visited;          // выделяем память и зануляем

	int running;            // для главного цикла main
} AppState;

*/
int init_app(AppState *app);

/*Добавляем устройство в массив app->devices, при заполнении массива, расширяется
динамически, также контакты устройства создаются автоматически в зависимости
от типа устройства передаваемого параметром type*/
int add_dev(const char *name, DevType type, AppState *app);

/*Возвращаем объект DevScb, если по имени не удалось найти вернет NULL*/
DevScb *get_dev(const char *name, AppState *app);

/*Возвращает объект Contact, если таких контактов нет, то вернет NULL*/
Contact *get_contact(DevScb *dev, int ax, int sec);

/*Условия подтачивания для базовых реле, передается условие подтока согласно
перечислению CondEvent,(types_rep.h), 
условия обестачивания определяются автоматически, в зависимости от подтока выбирается
противоположное действия для безтока*/
int add_condition(const char *name, CondEvent event, int number, AppState *app);




int add_edge(Contact *from, Contact *to, AppState *app);
int add_links(const char *from, int , int, const char *to, int, int, AppState *app);

// rep_find
/*Добавлена сущность findctx нужна она для получения информации если прострелить цепь питания не удалось,
может быть несколько фактров почему так могло произойти:
1) Это если указано реле которого нет в базе
2) */
int find_connection(const char *, int, int , FindCtx *, AppState *app);
int find_path(int current_id, int target_id, AppState *app, int depth, DevScb *, int *, FindCtx *find_ctx);

// функции для теста
int add_links_dev(DevScb *dev, AppState *app);
#endif
