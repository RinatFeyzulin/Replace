#include "handle.h"
#include "handle_str.h"
/*Вспомогательные функии для show_devices_handle*/

const char *get_str_type(DevType type){
	switch(type){
		case MINUS:
			return str_minus;
		case NMSH_1_1440:
			return str_nmsh1_1440;
		case NMSH_1_400:
			return str_nmsh1_400;
		case _1N_1350:
			return str_1n_1350;
		case _1NM_950:
			return str_1nm_950;
		case PLUS:
			return str_plus;
	}
	return str_undefined;
}

/*Вспомогательные функции для find_handle*/
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

void print_find_itemstr(ItemStr *head){
	ItemStr *tmp = head;
	while(tmp){
		print_message(tmp->name);
		print_message("\n");
		tmp = (ItemStr *)tmp->next;
	}
}

void handle_find_result(FindCtx *find_ctx){
	switch(find_ctx->event){
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
			print_find_itemstr(find_ctx->head);
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
}

/*Вспомагательные функции для  set_links_handle*/

int terminator_set(char c, char *buffer){
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

int set_ptr_offset(char **buff, char **ptr, char c){
	*buff = *ptr;
	int offset = terminator_set(c, *ptr);
	if(offset == -1) return -1;
	*ptr += offset;
	return 0;	
}

/*parse string format is 
"1П:11-12=2П:11-12"*/
int parse_link(char *from, int *from_ax, int *from_sec, char *to, int *to_ax, int *to_sec, char *buffer){
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



