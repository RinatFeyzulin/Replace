#ifndef SIMPLESECOND_H
#define SIMPLESECOND_H

#include "types_rep.h"
#include "./aux/aux.h"
/*Инициализируем основные компоненты программы*/
int init_app(AppState *app);


int add_dev(const char *name, DevType type, AppState *app);

DevScb *get_dev(const char *name, AppState *app);

Contact *get_contact(DevScb *dev, int ax, int sec);


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
