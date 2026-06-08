#ifndef SQLITE_REP_H
#define SQLITE_REP_H

#include <sqlite3.h>
#include <stdio.h>

#include "../types_rep.h"
// exec
int create_table(sqlite3 *db);

// insert
int insert_dev(const char *name, int type, sqlite3 *db);
int add_condition_db(const char *name, int cond, int number, sqlite3 *db);
int add_logic_links_db(const char *from_dev, int, int, 
						const char *to_dev, int, int, sqlite3 *db);
//int add_relay_nmsh1(sqlite3 *db, const char *name, const char *type, const char *loc, const char *cond);
//int add_default_dev(sqlite3 *db, const char *name, const char *type, const char *loc, const char *cond, int dev_type);

// update
void update_device_location(sqlite3 *db, const char *name, const char *new_loc);
void update_device_condition(sqlite3 *db, const char *name, const char *new_cond);

// select
int get_device_id_by_name(sqlite3 *db, const char *searh_name);
int load_devices(AppState *app);
int load_links(AppState *app);
int load_condition(AppState *app);
//int get_contact_id(sqlite3 *db, int dev_id, int pin_number);

//int load_devices(AppState *app);

#endif
