#include "simplesecond.h"
#include "./sqlite/sqlite_rep.h"
//static int load_dev(AppState *app);

int init_app(AppState *app)
{
	app->devices = malloc(sizeof(NodeDevScb) * MAX_DEVICES);
	if(app->devices == NULL) return -1;
	app->buckets = malloc(sizeof(int) * MAX_DEVICES);
	if(app->buckets == NULL) return -1;

	app->graph = malloc(sizeof(Vertex) * MAX_CONTACT);
	if(app->graph == NULL) return -1;
	app->visited = malloc(sizeof(int) * MAX_CONTACT);
	if(app->visited == NULL) return -1;
	
		
	for(int i = 0; i < MAX_DEVICES; i++){
		app->buckets[i] = -1;
		app->devices[i].next_bucket = -1;
		app->visited[i] = 0;
	}

	app->capacity_devices = MAX_DEVICES;
	app->capacity_contacts = MAX_CONTACT;
	app->count_devices = 0;
	app->count_contacts = 0;
	//открываем базу данных, если нет то создатся
	if(sqlite3_open(DATA_BASE, &app->db) != SQLITE_OK) return -1;

	create_table(app->db);
	load_devices(app);
	
	if(app->count_devices == 0){
		add_dev("П", PLUS, app);
		add_dev("М", PLUS, app);		
		add_dev("П", PLUS, app);		
		add_dev("М", PLUS, app);		
	} else {
		load_links(app);
		load_condition(app);		
	}

	app->running = 1;
	
	return 0;
	
}
