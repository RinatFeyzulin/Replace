#include "sqlite_rep.h"
#include "../simplesecond.h"

int load_devices(AppState *app){
	sqlite3_stmt *stmt;

	const char *sql = "SELECT name, type FROM devices";
	sqlite3_prepare_v2(app->db, sql, -1, &stmt, NULL);

	int count = 0;
	
	while(sqlite3_step(stmt) == SQLITE_ROW){
	
		const char *name = (char *)sqlite3_column_text(stmt, 0);
		int type = sqlite3_column_int(stmt, 1);
		 
		add_dev(name, type, app);

		count++;
	}
	
	sqlite3_finalize(stmt);
	//sqlite3_close(app->db);

	printf("Загружено объектов dev: %d\n", count);

	return 0;

}

int load_links(AppState *app){
	sqlite3_stmt *stmt;

	const char *sql = "SELECT from_dev,from_ax,from_sec,to_dev,to_ax,to_sec FROM logic_links";
	sqlite3_prepare_v2(app->db, sql, -1, &stmt, NULL);

	int count = 0;
	
	while(sqlite3_step(stmt) == SQLITE_ROW){
	
		const char *from_dev = (char *)sqlite3_column_text(stmt, 0);
		int from_ax = sqlite3_column_int(stmt, 1);
		int from_sec = sqlite3_column_int(stmt, 2);
		const char *to_dev = (char *)sqlite3_column_text(stmt, 3);
		int to_ax = sqlite3_column_int(stmt, 4);
		int to_sec = sqlite3_column_int(stmt, 5);

		//printf("[SQLITE LOAD] %s:%d-%d=%s:%d-%d\n", from_dev, from_ax, from_sec, to_dev, to_ax, to_sec);
		add_links(from_dev, from_ax, from_sec, to_dev, to_ax, to_sec, app);

		count++;
	}
	
	sqlite3_finalize(stmt);
	//sqlite3_close(app->db);

	printf("Загружено объектов links:%d\n", count);

	return 0;
}

int load_condition(AppState *app){
	sqlite3_stmt *stmt;

	const char *sql = "SELECT name,cond,number FROM condition";
	sqlite3_prepare_v2(app->db, sql, -1, &stmt, NULL);

	int count = 0;
	
	while(sqlite3_step(stmt) == SQLITE_ROW){
	
		const char *name = (char *)sqlite3_column_text(stmt, 0);
		int cond = sqlite3_column_int(stmt, 1);
		int number = sqlite3_column_int(stmt, 2);


		add_condition(name, cond, number, app);

		count++;
	}
	
	sqlite3_finalize(stmt);
	//sqlite3_close(app->db);

	printf("Загружено объектов condition:%d\n", count);

	return 0;
}

int get_device_id_by_name(sqlite3 *db, const char *search_name)
{
	sqlite3_stmt *stmt;
	int found_id = -1;
	const char *sql = "SELECT id FROM devices WHERE name = ?;";

	if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;

	sqlite3_bind_text(stmt, 1, search_name, -1, SQLITE_STATIC);

	if(sqlite3_step(stmt) == SQLITE_ROW){
		found_id = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return found_id;
}

/*
int get_contact_id(sqlite3 *db, int dev_id, int pin_number)
{
	sqlite3_stmt *stmt;
	int contact_id = -1;
	const char *sql = "SELECT id FROM contacts WHERE device id = ? AND pin_number = ?;";

	if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;

	sqlite3_bind_int(stmt, 1, dev_id);
	sqlite3_bind_int(stmt, 2, pin_number);

	if(sqlite3_step(stmt) == SQLITE_ROW){
		contact_id = sqlite3_column_int(stmt, 0);
	}

	sqlite3_finalize(stmt);
	return contact_id;
}


*/
