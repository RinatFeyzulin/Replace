#ifndef HANDLE_STR_H
#define HANDLE_STR_H

/*Привязано к enum Action (handle.h)*/
extern const char* const menu_str;
/*Привязано к enum DevType (types_rep.h)*/
extern const char* const dev_type_str;
					
/*Строковое представление типов*/
extern const char* const str_minus;
extern const char* const str_nmsh1_1440;
extern const char* const str_nmsh1_400;
extern const char* const str_1n_1350;		
extern const char* const str_1nm_950;		
extern const char* const str_plus;
extern const char* const str_undefined;
/*Оглавление  для обработчиков*/
extern const char* const head_set_links_str;
extern const char* const show_devices_str;

/*Запросы данных*/
extern const char* const name_dev_str;

/*Привязано к enum CondEvent(types_rep.h), при возвращении
из функций нужно умножить на 10*/
extern const char* const cond_event_str;
		
/*Доп инфа к cond_event*/
extern const char* const cond_ev_track_clear_str;
extern const char* const cond_ev_click_btn_str; /*TODO: исправить (номер) на (название) после тестов*/ 
extern const char* const cond_ev_route_str;

/*Запрос ввода формата*/
extern const char* const form_double_con_wind; /*TODO: Удалить после пределки
функции find_connection*/

extern const char* const form_links_contacts;


/*ERROR MESSAGE*/
extern const char* const err_parse_format;

extern const char* const err_nan_dev;
extern const char* const err_mem_add_dev;
extern const char* const err_db_add_dev;

extern const char* const err_mem_add_links;
extern const char* const err_db_add_links;

extern const char* const err_mem_add_condition;
extern const char* const err_db_add_condition;



#endif
