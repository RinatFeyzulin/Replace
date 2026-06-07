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
			set_links_handle(app);
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

int cond_handle(AppState *app){

	char dev_name[MAX_NAME_DEV];
	CondEvent cond;
	int number;

	if(get_string_stdin(dev_name, MAX_NAME_DEV, name_dev_str) == -1) return -1;

	cond = get_uint_stdin(cond_event_str, MIN_CONDEVENT, MAX_CONDEVENT);
	if(cond <= 0) return -1;
	cond *= 10;

	/*В зависимости от типа события запрашиваем доп инфу*/
	switch(cond){
		case TRACK_CLEAR:
			number = get_uint_stdin(cond_ev_track_clear_str, 0, INT_MAX);
			break;
	}
	if(number == -1) return -1;

	if(add_condition(dev_name, cond, number, app) == -1) return -1;
	if(add_condition_db(dev_name, cond, number, app->db) == -1) return -1;
	
	return 0;
}

int parse_double_contacts(char *str, int *ax, int *sec){
	char *ptr = str;
	while(*ptr == ' ') ptr++;
	while(*ptr != '-') ptr++;
	*ptr++ = '\0';
	*ax = ConvertToUInt(str);
	*sec = ConvertToUInt(ptr);
	if(!*ax || !*sec) return -1;
	return 0;
}

static void print_find_itemstr(ItemStr *head){
	ItemStr *tmp = head;
	while(tmp){
		print_message(tmp->name);
		print_message("\n");
		tmp = (ItemStr *)tmp->next;
	}
}
//sy 2c
int find_handle(AppState *app){
	char target[MAX_NAME_DEV];
	if(get_name_dev_stdin(target, MAX_NAME_DEV) == -1)
		return -1;
	// строка формата 11-12
	char winding[512];
	int ax;
	int sec;
	
	while(1){
		const char *message = "Введите строку формата [1-2] >";
		if(get_link_stdin(winding, sizeof(winding), message) == -1)
			return -1;
		if(parse_double_contacts(winding, &ax, &sec) == -1) 
			continue;
		else
			break;		
	}

	FindCtx find_ctx;
	find_connection(target, ax, sec, &find_ctx, app);
	switch(find_ctx.event){
		case FIND_OK:
			print_message("[SYSTEM]Операция прошла успешно, условия подтачивания внесены\n");
			break;
		case FIND_INCORRECT_DEV_NAME:
			print_message("[SYSTEM] Некорректный ввод, данное устройство отсутствует в базе\n");
			break;
		case FIND_NAN_PLUS:
			print_message("[SYSTEM] НЕ НАШЛИ ПЛЮС\n");
			break;
		case FIND_NAN_MINUS:
			print_message("[SYSTEM] НЕ НАШЛИ МИНУС\n");
			break;
		case FIND_NAN_CONTACT:
			print_message("[SYSTEM] Данные контакты отсутствуют\n");
			break;
		case FIND_NAN_CONTACT_PLUS:
			print_message("[SYSTEM] НЕ НАШЛИ Контакты ПЛЮСа\n");
			break;
		case FIND_NAN_CONTACT_MINUS:
			print_message("[SYSTEM] НЕ НАШЛИ Контакты Минуса\n");
			break;
		case FIND_NAN_COND_OR_DONT_COND:
			print_message("[SYSTEM] Недобавлены условия подтачивая реле:\n");
			print_find_itemstr(find_ctx.head);
			break;
		case FIND_NAN_LINKS_P:
			print_message("[SYSTEM] Не дошли до плюса\n");
			break;
		case FIND_NAN_LINKS_M:
			print_message("[SYSTEM] Не дошли до минуса\n");
			break;
		default:
			print_message("[SYSTEM] Что то пошло не так\n");
		
	}
	
	return 0;
	
}

static int terminator_set(char c, char *buffer){
	char *ptr = buffer;
	int count = 0;
	
	while(*ptr){
		if(*ptr == c){
			*ptr = '\0'; 
			return count + 1;			
		} 
		ptr++;
		count++;
	}
	return -1;
}

static int set_ptr_offset(char **buff, char **ptr, char c){
	*buff = *ptr;
	int offset = terminator_set(c, *ptr);
	if(offset == -1) return -1;
	*ptr += offset;
	return 0;	
}

/*parse string format is 
"1П:11-12=2П:11-12"*/
static int parse_link(char *from, int *from_ax, int *from_sec, char *to, int *to_ax, int *to_sec, char *buffer){
	char *ptr = buffer;
	
	char *from_ax_str = NULL;
	char *from_sec_str = NULL;
	char *to_ax_str = NULL;
	char *to_sec_str = NULL;
	
	while(*ptr == ' '){ // убираем пробелы в начале
		ptr++;
	}

	if(set_ptr_offset(&from, &ptr, ':') == -1) return -1;
	if(set_ptr_offset(&from_ax_str, &ptr, '-') == -1) return -1;
	if(set_ptr_offset(&from_sec_str, &ptr, '=') == -1) return -1;
	if(set_ptr_offset(&to, &ptr, ':') == -1) return -1;
	if(set_ptr_offset(&to_ax_str, &ptr, '-') == -1) return -1;
	to_sec_str = ptr;

	*from_ax = ConvertToUInt(from_ax_str);
	if(*from_ax == -1) return -1;

	*from_sec = ConvertToUInt(from_sec_str);
	if(*from_sec == -1) return -1;

	*to_ax = ConvertToUInt(to_ax_str);
	if(*to_ax == -1) return -1;

	*to_sec = ConvertToUInt(to_sec_str);
	if(*to_sec == -1) return -1;

	return 0;

/*		
	from = ptr;
	offset = terminator_set(':', ptr);
	if(offset == -1) return -1;
	ptr += offset;
	
	from_ax_str = ptr;
	offset = terminator_set('-', ptr);
	if(offset == -1) return -1;
	ptr += offset;

	from_sec_str = ptr;
	offset = terminator_set('=', ptr);
	if(offset == -1) return -1;
	ptr += offset;

	to = ptr;
	offset = terminator_set(':', ptr);
	if(offset == -1) return -1;
	ptr += offset;
*/
	

}

int set_links_handle(AppState *app){
	print_message("[INFO] Добавление идет от плюса. Перечисляем все контакты которые есть в цепи.\n"
	"Идем от Плюса(П) через нагрузку(обмотка реле) и на Минус(М).. \n");
	
	
	int from_ax;
	int from_sec;
	int to_ax;
	int to_sec;

	char from_name[MAX_NAME_DEV];
	char to_name[MAX_NAME_DEV];
	const char *message = "Введите связь контактов в формате [1П:11-12=2П:11-12] > ";
	char buffer[512];
	while(1){
		if(get_link_stdin(buffer, sizeof(buffer), message) == -1) return -1;
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
	char buff[MAX_NAME_DEV];
	int type;
	if(get_info_dev_stdin(buff, MAX_NAME_DEV, &type) == -1){
		return -1;
	}
		
	if(add_dev(buff, type, app) == -1){
		return -1;
	}

	if(insert_dev(buff, type, app->db) == -1){
		return -1;
	}

	return 0;
} 


void show_devices_handle(AppState *app){
	for(int i = 0; i < app->count_devices; i++){
		DevScb *dev = &app->devices[i].dev;
		print_dev(dev, i);
		print_contacts(dev);
	}		
}

int show_info_dev_handle(AppState *app){
	char name[MAX_NAME_DEV];
	if(get_name_dev_stdin(name, MAX_NAME_DEV) == -1) return -1;

	DevScb *dev = get_dev(name, app);
	if(dev == NULL) return -1;
	print_dev(dev, 0);
	print_contacts(dev);

	return 0;	
}


