#include "sqlite_rep.h"


static int update(sqlite3 *db, const char *sql,  const char *name, const char *new_value)
{
	sqlite3_stmt *stmt;

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	sqlite3_bind_text(stmt, 1, new_value, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);

	int rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	return rc;
	
}

void update_device_location(sqlite3 *db,  const char *name, const char *new_loc)
{
	const char *sql = "UDPATE devices SET location = ? WHERE name = ?;";
	update(db, sql, name, new_loc);
}

void update_device_condition(sqlite3 *db, const char *name, const char *new_cond)
{
	const char *sql = "UDPATE devices SET condition = ? WHERE name = ?;";
	update(db, sql, name, new_cond);
}
