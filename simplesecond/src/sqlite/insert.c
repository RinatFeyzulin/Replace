#include "sqlite_rep.h"

int add_condition_db(const char *name, int cond, int number, sqlite3 *db)
{
	if(name == NULL) return -1;

	sqlite3_stmt *stmt;
	const char *sql = 
	"INSERT INTO condition (name, cond, number) VALUES(?,?,?);";	

	if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) 
		return -1;
 
	sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, cond);
	sqlite3_bind_int(stmt, 3, number);



	int rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(rc == SQLITE_DONE)
		return 0;
	return -1;
}

int add_logic_links_db(const char *from_dev, int from_ax, int from_sec, 
					const char *to_dev, int to_ax, int to_sec, sqlite3 *db)
{
	printf("[add_logic_links] %s:%d-%d=%s:%d-%d\n", 
		from_dev, from_ax, from_sec, to_dev, to_ax, to_sec);
	
	sqlite3_stmt *stmt;
	const char *sql = 
	"INSERT INTO logic_links (from_dev, from_ax, from_sec, to_dev, to_ax, to_sec) VALUES(?,?,?,?,?,?);";	

	if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) 
		return -1;
 
	sqlite3_bind_text(stmt, 1, from_dev, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, from_ax);
	sqlite3_bind_int(stmt, 3, from_sec);
	sqlite3_bind_text(stmt, 4, to_dev, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 5, to_ax);
	sqlite3_bind_int(stmt, 6, to_sec);


	int rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(rc == SQLITE_DONE)
		return 0;
	return -1;
}

int insert_dev(const char *name, int type, sqlite3 *db){
#ifdef DEBUG
	printf("[DEBUG] зашли в insert_dev, TYPE - %d\n", type);
#endif
	int rc;
	sqlite3_stmt *stmt;
	const char *sql = 
	"INSERT INTO devices (name, type) VALUES(?,?);";

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, type);

	rc = sqlite3_step(stmt);
	
	sqlite3_finalize(stmt);
	
	if(rc != SQLITE_DONE){
		return -1;
	} else {
		return 0;
	}
		
}

/*
int add_relay_nmsh1(sqlite3 *db, const char *name, const char *type, const char *loc, const char *cond)
{
	// открываем транзакцию
	sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
	
	int dev_id = add_default_dev(db,name, type, loc, cond, 1);
	if(dev_id <= 0) {
		sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
		return -1;
	}

	sqlite3_stmt *stmt;
	const char *sql = 
	"INSERT INTO contacts (device_id, pin_number, pin_group) VALUES(?,?,?);";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	
	for(int i = 1; i <= 4; i++){
		sqlite3_reset(stmt);
		sqlite3_bind_int(stmt, 1, dev_id);
		sqlite3_bind_int(stmt, 2, i);
		sqlite3_bind_int(stmt, 3, 0);
		sqlite3_step(stmt);
	}

	for(int i = 1; i <= 8; i++){
		sqlite3_reset(stmt);
		sqlite3_bind_int(stmt, 1, dev_id);
		sqlite3_bind_int(stmt, 2, i * 10 + 1);
		sqlite3_bind_int(stmt, 3, i);
		sqlite3_step(stmt);

		sqlite3_reset(stmt);
		sqlite3_bind_int(stmt, 1, dev_id);
		sqlite3_bind_int(stmt, 2, i * 10 + 2);
		sqlite3_bind_int(stmt, 3, i);
		sqlite3_step(stmt);

		sqlite3_reset(stmt);
		sqlite3_bind_int(stmt, 1, dev_id);
		sqlite3_bind_int(stmt, 2, i * 10 + 3);
		sqlite3_bind_int(stmt, 3, i);
		sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
	// закрываем транзакцию. все записи улетают в базу в этот момент
	if(sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK)
		return -1;
	return 0;
}

int add_default_dev(sqlite3 *db, const char *name, 
	const char *type, const char *loc, const char *cond, int dev_type)
{
	if(name == NULL || type == NULL || loc == NULL)
		return -1;
		
	int rc;
	sqlite3_stmt *stmt;
	
	const char *sql = 
	"INSERT INTO devices (name, type, location, condition, is_relay) VALUES(?,?,?,?,?);";

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, type, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, loc, -1, SQLITE_STATIC);
	if(cond == NULL)
		sqlite3_bind_text(stmt, 4, "EMPTY", -1, SQLITE_STATIC);
	else
		sqlite3_bind_text(stmt, 4, cond, -1, SQLITE_STATIC);
		
	sqlite3_bind_int(stmt, 5, dev_type);

	rc = sqlite3_step(stmt);

	sqlite3_finalize(stmt);
	if(rc != SQLITE_DONE)
		return -1;
	// возвращаем айди добавленного устройства
	return sqlite3_last_insert_rowid(db);
}


*/
