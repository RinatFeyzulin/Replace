#include "handle.h"
#include "handle_str.h" // прописаны константы для сообщений
#include <limits.h>
/*в циклах предусмотрен выход путем вовращения с ui функции результата -1*/

int request_action(void){
	return get_uint_stdin(menu_str, MIN_ACTION, MAX_ACTION);
}

int handler_action(int c, AppState *app){
	switch(c){
		case ACTION_ADD_DEV: // Добавляем устройство
			if(add_dev_handle(app) == -1){
				print_message("Устройство не добавлено\n");
			}
			break;
		case ACTION_SHOW_DEVICES: // Показать список устройств
			show_devices_handle(app);
			break;
		case ACTION_INFO_DEV: // Инфа по конретному устройству
			show_info_dev_handle(app);
			break;				
		case ACTION_SET_LINKS: // Установить связи в цепи питания
			if(set_links_handle(app) == -1){
				print_message("Связи не установлены\n");	
			}
			break;
		case ACTION_FIND: // Показать связи в цепи питания
			find_handle(app);
			break;
		/*Будет предоставлен выбор из cond_event*/
		case ACTION_ADD_COND: // Добавить условия подтачивания
			if(cond_handle(app) == -1) 
				print_message("Условия подтачивания не добавлены\n");
			break;
	}

	
	return 0;	
}


/*Вспомогательные функции для find_handle лежат в handle_aux_func.c*/
// парсим строку формата [1-2]
int parse_double_contacts(char *str, int *ax, int *sec);
// Печатаем инфу инфу которую насобирали, если она есть
void print_find_itemstr(ItemStr *head);
// Обрабатываем результат прострела
void handle_find_result(FindCtx *find_ctx);

// Простреливаем цепь питания реле
int find_handle(AppState *app){

	const int buff_size = 64;
	
	char target[MAX_NAME_DEV]; // какое реле простреливаем
	if(get_string_stdin(target, MAX_NAME_DEV, name_dev_str) == -1)
		return -1;
	// строка формата 11-12
	char winding[buff_size];
	int ax;
	int sec;
	
	while(1){
		if(get_string_stdin(winding, buff_size, form_double_con_wind) == -1)
			return -1;
		if(parse_double_contacts(winding, &ax, &sec) == -1){
			print_message("Введена некорректная строка\n"); 
			continue;
		}
			
		else
			break;		
	}

	FindCtx find_ctx;
	find_connection(target, ax, sec, &find_ctx, app);
	handle_find_result(&find_ctx);
	
	return 0;
	
}

/*Вспомогательные функции для set_links_handle лежат в handle_aux_func.c*/
/*Установка терминатора вместо передаваемого символа 'c' в буфере, возвращает смещение
относительно буфера */
int terminator_set(char c, char *buffer);
/*Устанавливаем указатель на строку, и добавляем смещение*/
int set_ptr_offset(char **buff, char **ptr, char c);
/*parse string format is 
"1П:11-12=2П:11-12"*/
int parse_link(char *from, int *from_ax, int *from_sec, 
					char *to, int *to_ax, int *to_sec, char *buffer);

int set_links_handle(AppState *app){

	print_message(head_set_links_str);

	const int buff_size = 512;
	
	int from_ax;
	int from_sec;
	int to_ax;
	int to_sec;

	char from_name[MAX_NAME_DEV];
	char to_name[MAX_NAME_DEV];
	
	char buffer[buff_size];
	while(1){
		if(get_string_stdin(buffer, buff_size, form_links_contacts) == -1) 
			return -1;
		if(parse_link(from_name, &from_ax, &from_sec, to_name, &to_ax, &to_sec, buffer) == -1){
			print_message("Некорректный ввод\n");
			continue;
		}		
		add_links(from_name, from_ax, from_sec, to_name, to_ax, to_sec, app);
		add_logic_links(from_name, from_ax, from_sec, to_name, to_ax, to_sec, app->db);
	}
	
	return 0;
}

int add_dev_handle(AppState *app){
	char name[MAX_NAME_DEV];
	int type;
	if(get_string_stdin(name, MAX_NAME_DEV, name_dev_str) == -1)
		return -1;
 	if((type = get_uint_stdin(dev_type_str, MIN_DEVTYPE, PLUS - 1)) == -1) /*TODO: ПЕРЕДЕЛАТЬ
 	(PLUS - 1) НА (PLUS) В СЛУЧАЕ ЕСЛИ БУДУТ ПРИВЯЗЫВАТЬСЯ ПЛЮСЫ К КОНТАКТАМ*/ 
 		return -1;
	
	if(add_dev(name, type, app) == -1){
		return -1;
	}

	if(insert_dev(name, type, app->db) == -1){
		return -1;
	}
	return 0;
} 

/*Отдает DevType возвращаем строковое представление из handle_str.h*/
const char *get_str_type(DevType type);

void show_devices_handle(AppState *app){
	print_message(show_devices_str);
	int buff_size = 512;
	char buffer[buff_size];
	
	for(int i = 0; i < app->count_devices; i++){
		DevScb *dev = &app->devices[i].dev;
		snprintf(buffer, buff_size, "[%d] Name: %s; Type: %s\n",
		 					i, dev->name, get_str_type(dev->type));
		print_message(buffer);	
	}		
}

void show_info_dev_handle(AppState *app){

	const int buff_size = 512; 

	char name[MAX_NAME_DEV];
	if(get_string_stdin(name, MAX_NAME_DEV, name_dev_str) == -1) 
		return;

	DevScb *dev = get_dev(name, app);
	if(dev == NULL) 
		print_message("Данного устройства нет в базе\n");

	char buffer[buff_size];
	snprintf(buffer, buff_size, "Name: %s; Type: %s\n",
			 					dev->name, get_str_type(dev->type));
	// Достаем контакты
	print_message("| К-т   | Назначение | Проверка");
	for(int i = 0; i < dev->count_contacts; i++){

		Contact *con = &dev->contacts[i];
		snprintf(buffer, buff_size, "|%d-%d|", con->axial, con->second);
		print_message(buffer);

		print_message("В цепи питания: ");
		CondList *curr_list = con->cond_list;
		while(curr_list){
			print_message(((DevScb *)curr_list->target_dev)->name);
			print_message("; ");
			curr_list = (CondList *)curr_list->next_list;
		}
		print_message("|");
	}
}

int cond_handle(AppState *app){

	char dev_name[MAX_NAME_DEV];
	CondEvent cond;
	int number = -1; /* сразу зададим значение чтобы если вдруг случайно (что мало вероятно) 
	в switch case мы не попадем можно было выйти из функции*/

	if(get_string_stdin(dev_name, MAX_NAME_DEV, name_dev_str) == -1) return -1;

	cond = get_uint_stdin(cond_event_str, MIN_CONDEVENT, MAX_CONDEVENT);
	if(cond <= 0) return -1;
	cond *= 10;

	/*В зависимости от типа события запрашиваем доп инфу*/
	switch(cond){
		case TRACK_CLEAR:
		case TRACK_OCUPP: // сбор информации осуществляется для условий подтачивания, обестачивания добавил чтобы компилятор не ругался
			number = get_uint_stdin(cond_ev_track_clear_str, 0, INT_MAX);
		case CLICK_BTN:
		case DONT_CLICK_BTN:
			 number = get_uint_stdin(cond_ev_click_btn_str, 0, INT_MAX);
		case ROUTE:
		case DONT_ROUTE:
			 number = get_uint_stdin(cond_ev_route_str, 0, INT_MAX); 
			break;
	}
	
	if(number == -1) return -1;

	if(add_condition(dev_name, cond, number, app) == -1) return -1;
	if(add_condition_db(dev_name, cond, number, app->db) == -1) return -1;
	
	return 0;
}
