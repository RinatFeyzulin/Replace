#include "simplesecond.h"

/*Выделяем паиять для CondList(условия подтачивания устройств)*/
static CondList *create_cond_list(void){
	CondList *cond_list = malloc(sizeof(CondList));
	if(cond_list == NULL) return NULL;
	cond_list->next_list = NULL;
	
	cond_list->cond_node  = malloc(sizeof(CondNode));
	if(cond_list->cond_node == NULL) return NULL;
	cond_list->cond_node->next = NULL;
		
	cond_list->cond_node->condition = malloc(sizeof(Condition));
	if(cond_list->cond_node->condition == NULL) return NULL;

	return cond_list;	
}

/*Сюда передаем только условия подтока, условия безтока автоматически определяются*/
int add_condition(const char *name, CondEvent event, int number, AppState *app){

	DevScb *dev = get_dev(name, app);
	if(!dev)
		return -1;
		
	CondList *cond_list = create_cond_list();
	if(!cond_list) 
		return -1;

	CondList *dont_cond_list = create_cond_list();
	if(!dont_cond_list)
		return -1;

	cond_list->next_list = (struct CondList *)dev->cond_list;	
	dont_cond_list->next_list = (struct CondList *)dev->dont_cond_list;

	/*В зависимости от подтока выбирается противопроложное действия для безтока*/
	cond_list->cond_node->condition->event = event;
	if(event % 2 == 0){
		dont_cond_list->cond_node->condition->event = event + 1;		
	} else {
		dont_cond_list->cond_node->condition->event = event - 1;
	}

	cond_list->cond_node->condition->number = number;
	dont_cond_list->cond_node->condition->number = number;

	cond_list->idx_source_dev = dev->idx;
	dont_cond_list->idx_source_dev = dev->idx;

	dev->cond_list = cond_list;
	dev->dont_cond_list = dont_cond_list;

	
	return 0;
}


Contact *get_contact(DevScb *dev, int ax, int sec){
	Contact *contacts = dev->contacts;
	for(int i = 0; i < dev->count_contacts; i++){
		if(contacts[i].axial == ax && contacts[i].second == sec){
			return &contacts[i];
		}
	}
	return NULL;
}

static int resize_graph(AppState *app){
	int capacity = app->capacity_contacts * 2;
	Vertex *tmp_graph = realloc(app->graph, capacity *sizeof(Vertex));
	int *tmp_visited = realloc(app->visited, capacity *sizeof(int));
	if(tmp_graph == NULL || tmp_visited == NULL) return -1;

	app->visited = tmp_visited;
	app->graph = tmp_graph;
	app->capacity_contacts = capacity;

	
	
	return 0;
}

static int add_contact(DevScb *dev, Contact *con, int axial, int second, int dev_state, AppState *app){

	if(dev == NULL || con == NULL) return -1;

	if(app->count_contacts >= app->capacity_contacts){
		if(resize_graph(app) == -1) return -1;
	}
	
	con->axial = axial;
	con->second = second;
	con->dev_state = dev_state;
	con->cond_list = NULL;

	con->idx_graph = app->count_contacts;
	con->idx_con_dev = dev->count_contacts;
	con->idx_source_dev = dev->idx;
		
	//con->source_dev = (struct DevScb *)dev;
	
	//app->graph[app->count_contacts].from_contact = con;
	app->graph[app->count_contacts].from_idx_device = dev->idx;
	app->graph[app->count_contacts].from_idx_contact = dev->count_contacts;
	app->graph[app->count_contacts].head = NULL;

	app->visited[app->count_contacts] = 0;

	dev->count_contacts++;
	app->count_contacts++;
	

	return 0;
}

static int create_contacts_nmsh1(DevScb *dev, AppState *app){
	/*18пар*/
	dev->contacts = malloc(sizeof(Contact) * 18);
	if(dev->contacts == NULL) return -1;
	
	add_contact(dev, dev->contacts, 1, 2, 1, app);
	add_contact(dev, dev->contacts + 1, 3, 4, 1, app);

	int ax = 0;
	
	for(int i = 1, j = 2; i <= 8; i++, j+=2){
		ax = i * 10 + 1;
		add_contact(dev, dev->contacts + j, ax, ax + 1, 1, app);
		add_contact(dev, dev->contacts + j + 1, ax, ax + 2, 0, app);
		
	}		
	return 0;
} 

static int create_contacts_plus(DevScb *dev, AppState *app){
	dev->contacts = malloc(sizeof(Contact));
	if(dev->contacts == NULL) return -1;
	
	return add_contact(dev, dev->contacts, 1, 2, 1, app);
	 	
}

static int create_contacts(DevScb *dev, AppState *app){
	switch(dev->type){
		case NMSH_1_1440:
		case NMSH_1_400:
		case _1N_1350:
		case _1NM_950:
			return create_contacts_nmsh1(dev, app);
		case PLUS:
			return create_contacts_plus(dev, app);
		default:
			return -1;
	}	
	return 0;
}

static int resize_devices(AppState *app){

	int capacity = app->capacity_devices * 2;
	NodeDevScb *tmp = realloc(app->devices, capacity * sizeof(NodeDevScb));
	if(tmp == NULL) return -1;
	app->devices = tmp;
	app->capacity_devices = capacity;

	int *tmp_buckets = malloc(capacity * sizeof(int));
	if(tmp_buckets == NULL) return -1;
	
	
	for(int i = 0; i < capacity; i++){
		tmp_buckets[i] = -1;
	}
	
	for(int i = 0; i < app->count_devices; i++){
		int idx_bucket = hash_djb2(app->devices[i].dev.name) % capacity;
		app->devices[i].next_bucket = tmp_buckets[idx_bucket];
		tmp_buckets[idx_bucket] = i;
	}

	free(app->buckets);
	
	app->buckets = tmp_buckets;

	return 0;
}

static NodeDevScb *get_new_node_dev(const char *name, AppState *app){

	int hash = hash_djb2(name);
	int idx_bucket = hash % app->capacity_devices;
	int idx = app->buckets[idx_bucket];
	
	while(idx != -1){
		if(strncmp(name, app->devices[idx].dev.name, sizeof(MAX_NAME_DEV)) == 0)
			return NULL;
		idx = app->devices[idx].next_bucket;
		
	}

	if(app->count_devices >= app->capacity_devices){
		if(resize_devices(app) == -1) 
			return NULL;
		idx_bucket = hash % app->capacity_devices;
	}
	
	app->devices[app->count_devices].next_bucket = app->buckets[idx_bucket];
	app->buckets[idx_bucket] = app->count_devices;
	app->devices[app->count_devices].dev.idx = app->count_devices;
	
	return &app->devices[app->count_devices];
	
}

int add_dev(const char *name, DevType type, AppState *app){

	
	NodeDevScb *node_dev = get_new_node_dev(name, app);

	if(node_dev == NULL){
		printf("Попытка повторного добавления\n");
		return -1; // устройство с таким именем  уже есть	
	} 


	snprintf(node_dev->dev.name, MAX_NAME_DEV, "%s", name);
	node_dev->dev.type = type;
	node_dev->dev.count_contacts = 0;
	node_dev->dev.idx = app->count_devices;
		
	if(create_contacts(&node_dev->dev, app) == -1)
		return -1;		
	
	app->count_devices++;

	return 0;
}

DevScb *get_dev(const char *name, AppState *app){

	int idx_bucket = hash_djb2(name) % MAX_DEVICES;
	int idx = app->buckets[idx_bucket];

	while(idx != -1){
		if(strncmp(name, app->devices[idx].dev.name, sizeof(MAX_NAME_DEV)) == 0)
			return &app->devices[idx].dev;
		idx = app->devices[idx].next_bucket;
	}
	return NULL;
}

