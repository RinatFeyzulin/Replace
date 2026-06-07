/*В каждом цикле сбора информации будет предусмотрена 
возможность выхода из функции путем передачи в буфер
символа q*/
#ifndef UI_H
#define UI_H

#include "../aux/aux.h"

#include <stdio.h>
#include <string.h>
//#include "../simplesecond.h"

/*Выводим сообщение на консоль, управляющие символы типа перевода строки передаются в сообщении*/
void print_message(const char *message);

/*Получает строку, в параметр message передаем требования для заполнения buffer*/
int get_string_stdin(char *buffer, int size, const char *message);

/*Получает строку, в параметр message передаем требования, число число возращается
в результате получения результата функции
@ добавлены параметры min max, чтобы можно было задавать диапазон запрашиваемых значений*/
int get_uint_stdin(const char *message, int min, int max);


#endif
