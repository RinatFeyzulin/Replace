#include "handle_str.h"

/*Привязано к enum Action (handle.h)*/
const char* const menu_str =
	"\nВыберите действие:\n"
	"1 - Добавить устройство(реле)\n"
	"2 - Показать список устройств(реле)\n" /*TODO: Удалить после тестов*/
	"3 - Вывести информацию по конкретному устройству(реле)\n"
	"4 - Установить связи в цепи питания\n"
	"5 - Показать связи в цепи питания\n"
	"6 - Установить условия подтачивания\n"
	"Для выхода нажмите q\n"
	">";
/*Привязано к enum DevType (types_rep.h)*/
const char* const dev_type_str = 
	"Выберите тип устройства:\n"
					"1 -> НМШ1-1440\n"
					"2 -> НМШ1-400\n"
					"3 -> 1Н-1350\n"
					"4 -> 1НМ-950\n"
					"> ";
					
/*Строковое представление типов*/
const char* const str_minus = "Минус";
const char* const str_nmsh1_1440 = "НМШ1-1440";
const char* const str_nmsh1_400 = "НМШ1-400";
const char* const str_1n_1350 = "1Н-1350";		
const char* const str_1nm_950 = "1Н-1350";		
const char* const str_plus = "Плюс";
const char* const str_undefined = "Неопределено";
/*Оглавление  для обработчиков*/
const char* const head_set_links_str = "[INFO] Добавление идет от плюса. Перечисляем все контакты которые есть в цепи.\n"
	"Идем от Плюса(П) через нагрузку(обмотка реле) и на Минус(М).. \n";
const char* const show_devices_str = "Список устройств:\n";

/*Запросы данных*/
const char* const name_dev_str = "Введите название устройства: > ";

/*Привязано к enum CondEvent(types_rep.h), при возвращении
из функций нужно умножить на 10*/
const char* const cond_event_str =
		"Выберите условие для подтачивания реле:\n" 
		"1 -> Рельсовая цепь свободна\n"
		"2 -> Нажата кнопка\n"
		"3 -> Задан маршрут\n"
		"> ";
		
/*Доп инфа к cond_event*/
const char* const cond_ev_track_clear_str = "Введите номер рельсовой цепи: > ";
const char* const cond_ev_click_btn_str = "Введите номер кнопки: > "; /*TODO: исправить номер на название после тестов*/ 
const char* const cond_ev_route_str = "Введите номер маршрута: > ";

/*Запрос ввода формата*/
const char* const form_double_con_wind = "Введите контакты обмотки. Строка формата [1-2] >"; /*TODO: Удалить после пределки
функции find_connection*/

const char* const form_links_contacts = "Введите связь контактов в формате [1П:11-12=2П:11-12] > ";


/*ERROR MESSAGE*/
const char* const err_parse_format = "Некорректный ввод, свертесь с форматом\n";

const char* const err_nan_dev = "Данное устройство отсутствует в базе\n";
const char* const err_mem_add_dev = "Не удалось добавить устройство(возможно оно уже добавлено)\n";
const char* const err_db_add_dev = "Не удалось добавить устройство в базу(возможно оно уже добавлено)\n";

const char* const err_mem_add_links = "Не удалось добавить связи в память\n";
const char* const err_db_add_links = "Не удалось добавить связи в базу\n";

const char* const err_mem_add_condition = "Не удалось добавить условия подтачивания в память\n";
const char* const err_db_add_condition = "Не удалось добавить условия подтачивания в базу данных\n";



