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
