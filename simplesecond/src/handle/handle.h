#ifndef HANDLE_H
#define HANDLE_H

#include "../simplesecond.h"
#include "../ui/ui.h"
#include "../sqlite/sqlite_rep.h"
#include "handle_str.h"
/*ПРОВЕРЯТЬ СООТВЕТСВИЕ С menu_str(handle_str.h)*/
typedef enum {
	ACTION_ADD_DEV = 1,
	ACTION_SHOW_DEVICES, /*TODO: Удалить после окончания тестирования*/
	ACTION_INFO_DEV,
	ACTION_SET_LINKS,
	ACTION_FIND,
	ACTION_ADD_COND
} Action;
#define MAX_ACTION 6
#define MIN_ACTION 1 


/*Запрашиваем действие (Добавить, Показать и т.д), возращаем код операции, перечисление enum Action
в виде int*/
int request_action(void);

/*Обрабатываем запрос, по сути кейсы на другие методы*/
int handler_action(int c, AppState *app);


/*Дальше идет функциональность*/ 


/*Добавляем устройство в память и базу 
Запрашиваемая иформация:
	Имя устройства
	Тип
*/
int add_dev_handle(AppState *app);

/*Показать список устройсв*/
void show_devices_handle(AppState *app);
/*Показать инфу по конкретному устройству как в планах замены*/
void show_info_dev_handle(AppState *app);

/*Устанавливаем связи между контактами, Информация добавляется в базу и память
Запрашиваемая информация:(формат запроса 1П:11-12=2П11-12), парсить помагают доп функции 
	Имя реле откуда
	Номера контактов откуда
	Имя реле куда
	Номера контактов куда
*/
int set_links_handle(AppState *app);

/*Простреливаем цепь питания, и собираем CondList, если у какого то реле нет,
то программа выведет его через print_find_itemstr
Запрашиваемая информация:
	Имя устройтсва
	Какую обмотку простреливать(формат 1-2)*/
int find_handle(AppState *app); /*вспомогательные функции для find_handle лежат в handle_aux_func.c*/


/*Добавляем условия подтачивания для реле, данные заносятся в память и базу sql
Собираемая инфрмация : 
	CondEvent cond, // Условие для подтока реле 
	int number
*/
int cond_handle(AppState *app);
#endif
