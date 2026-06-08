#include "sqlite_rep.h"
/*TODO: возможно стоить место установки жестко разбить на Статив полка место чтобы проще было искать реле,
также скорее всего придется добавить поле релейная или поле, так как в поле место установки будет РЦ или Пикеты*/
/*TODO: также подумать над синхронизацией описания условий проверки, в случае изменения описания подтока реле*/

int create_table(sqlite3 *db) {

	int table_count = 0;
    char *err_msg = 0;
    int rc;

	// 0. Таблица состояния программы

    // 1. Таблица устройств 
    char *sql_devices = 
        "CREATE TABLE IF NOT EXISTS devices ("
        "name TEXT UNIQUE NOT NULL,"    // 1С, 1П
        "type INTEGER);";                   // НМШ1-1440
       // "location TEXT,"               // 11-2-12
       // "condition TEXT,"              // Условие подтока (для 1-4)
       /// "is_relay INTEGER DEFAULT 1);"; // 1 - реле, 0 - колодка
	table_count++;

/*
    // 2. Таблица лепестков (физические контакты)
    char *sql_contacts = 
        "CREATE TABLE IF NOT EXISTS contacts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "device_id INTEGER,"            // Чье это реле
        "pin_number INTEGER,"           // 11, 12, 1, 4...
        "pin_group INTEGER,"            // 1, 2... (0 для обмоток)
        "FOREIGN KEY(device_id) REFERENCES devices(id) ON DELETE CASCADE);";
*/

    // 3. Таблица ЛОГИКИ (Здесь будут храниться ребра для графа)
    char *sql_logic_links = 
        "CREATE TABLE IF NOT EXISTS logic_links ("
        "from_dev TEXT,"        // Реле, чей контакт используем (1П)
        "from_ax INTEGER,"            // 11
        "from_sec INTEGER,"           // 12 или 13
        "to_dev TEXT,"        // В цепи какого реле стоит (1С)
		"to_ax INTEGER,"
        "to_sec INTEGER);";             
	table_count++;
	
	char *sql_condition = 
	    "CREATE TABLE IF NOT EXISTS condition ("
	    "name TEXT UNIQUE NOT NULL,"       
	    "cond INTEGER,"          
	    "number INTEGER);";             
	table_count++;
/*
    // 4. Таблица МОНТАЖА (Физические провода и узлы)
    char *sql_mounting = 
        "CREATE TABLE IF NOT EXISTS mounting_nodes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "from_contact_id INTEGER,"           // Ссылка на конкретный лепесток
        "to_contact_id INTEGER,"              // ID электрического узла (шины)
        "wire_label TEXT,"              // Маркировка (П, М) монтаж или кабель
        "FOREIGN KEY(contact_id) REFERENCES contacts(id) ON DELETE CASCADE);";
*/
    // Выполняем создание таблиц по очереди
    char *queries[] = {sql_devices, sql_logic_links, sql_condition};

    
    for (int i = 0; i < table_count; i++) {
        rc = sqlite3_exec(db, queries[i], 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Ошибка SQL (%d): %s\n", i, err_msg);
            sqlite3_free(err_msg);
            return -1;
        }
    }

    // Включаем поддержку Foreign Keys (обязательно!)
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);

    return 0;
}


