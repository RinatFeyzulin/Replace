#include "ui.h"

/*Вспомогательная функция для очистки буфера стандартного ввода*/
static void cleanup_buff_stdin(){
	int c;
	while((c = fgetc(stdin)) != '\n' && c != EOF);
}

void print_message(const char *message){
	printf("%s", message);
}

int get_string_stdin(char *buffer, int size, const char *message){
	while(1){
			printf("%s:\n> ", message);
			if(fgets(buffer, size, stdin) == NULL){
				printf("Ошибка чтения\n\n");
				return -1;
			}
			buffer[strcspn(buffer, "\n")] = '\0';

			if(strncmp(buffer, "q", size) == 0){
				return -1;
			} else {
				break;
			}
				
		}
	return 0;	
}

int get_uint_stdin(const char *message, int min, int max){
	char buff[64];
	int number;
	
	printf("%s\n", message);
	while(1){
		printf("> ");
		if(fgets(buff, sizeof(buff), stdin) == NULL){
			printf("Ошибка записи в буфер\n");
			return -1;
		}
		buff[strcspn(buff, "\n")] = '\0';
		if(strncmp(buff,  "q", sizeof(buff)) == 0){
			return -1;
		}			
		number = ConvertToUInt(buff);
		if(number == -1){
			printf("Введено некоретное число, повторите попытку\n");
			continue;
		}
		if(number < min || number > max){
			printf("Не уложился в диапазон, повтори попытку\n");
			continue;
		}
		return number;
	}
	
}


static char *nmsh1_1440 = "НМШ1-1440";
static char *nmsh1_400 = "НМШ1-400";
static char *_1n_1350 = "1Н-1350";
static char *_1nm_950 = "1НМ-950";
static char *undefined = "UNDEFINED";


void print_dev(DevScb *dev, int id){

	char *type_str;
	switch(dev->type){
		case 1:
			type_str = nmsh1_1440;
			break;
		case 2:
			type_str = nmsh1_400;
			break;
		case 3:
			type_str = _1n_1350;
			break;
		case 4:
			type_str = _1nm_950;
			break;
		default:
			type_str = undefined;
		
	}

	printf("[%d] NAME - %s, TYPE - %s\n", id, dev->name, type_str);
}


void print_contacts(DevScb *dev){
	for(int i = 0; i < dev->count_contacts; i++){
		printf("[%d-%d]\n", dev->contacts[i].axial, dev->contacts[i].second);
	}
}




int get_event_cond_stdin(CondEvent *event, int *number){
	int count_event = 3;
	while(1){
		printf("Выберите условие для подтачивания реле:\n"
		"1 -> Рельсовая цепь свободна\n"
		"2 -> Нажата кнопка\n"
		"3 -> Задан маршрут\n"
		"> "
		);

		int ev  = fgetc(stdin);
		cleanup_buff_stdin();
			
		if(ev == 'q'){
			return -1;
		}

		ev = ev - '0';

		if(ev < 1 || ev > count_event){
			printf("Введено некорректное число, повторите попытку\n\n");
			continue;
		} 

		ev *= 10;
		int num = -1;

		const char *track_clear = "Введите номер рельсовой цепи";
		const char *click_btn = "Нажата кнопка";
		const char *route = "Задан маршрут";
		
		switch(ev){
			case TRACK_CLEAR:
				num = get_int(track_clear);
				break;
			case CLICK_BTN:
				num = get_int(click_btn);
				break;
			case ROUTE:
				num = get_int(route);
				break;
		} 
		
		if(num == -1) {
			printf("Ошибка которой не должно быть\n");
			return -1;
		}
		
	}
}

int get_info_dev_stdin(char *name, int len, int *type){

	if(get_name_dev_stdin(name, len) == -1)
		return -1;
		
	while(1){
		printf("Выберите тип устройства:\n"
				"1 -> НМШ1-1440\n"
				"2 -> НМШ1-400\n"
				"3 -> 1Н-1350\n"
				"4 -> 1НМ-950\n"
				"> "
				);
				
		int c  = fgetc(stdin);
		cleanup_buff_stdin();
		
		if(c == 'q'){
			return -1;
		} 
			
		c = c - '0';
		printf("%d\n", c);
		
		if(c < 1 || c > 4){
			printf("Введено некорректное число, повторите попытку\n\n");
			continue;
		} else {
			*type = c;
			break;
		}
	}
	
	return 0;
}
