#include "simplesecond.h"

/*всегда возвращает -1*/
static int get_find_event(const char *name, int event, FindCtx *find_ctx){
	snprintf(find_ctx->name, MAX_NAME_DEV, name);
	find_ctx->event = event;
	return -1;
}
static void malloc_cond(DevScb *target){
	CondList *new_list = malloc(sizeof(CondList)); 
	new_list->next_list = (struct CondList *)target->cond_list;
	new_list->cond_node = NULL;
	target->cond_list = new_list;	
} 
static void set_event_cond(DevScb *target, DevScb *src, Contact *con){

	CondList *curr_list;

	if(con->dev_state)
		curr_list = src->cond_list;
	else
		curr_list = src->dont_cond_list;
						
	if(curr_list == NULL){
		/*TODO: значит у нас не описано реле, нужно будет высылать пользователя для заполнения
		условия подтачивания этого реле*/
		printf("Не добавлены условия подтачивания реле %s\n", src->name);
		return;
	} 
	
	while(curr_list != NULL){
		CondNode *curr_node = curr_list->cond_node;
		while(curr_node != NULL){
			CondNode *new_node = malloc(sizeof(CondNode));
			new_node->next = (struct CondNode *)target->cond_list->cond_node;
			target->cond_list->cond_node = new_node;
			
			new_node->condition = curr_node->condition;
			
			curr_node = (CondNode *)curr_node->next;	
		}	
		curr_list = (CondList *)curr_list->next_list;
	}
}
static void check_condition(DevScb *dev, FindCtx *find_ctx){
	if(!dev->cond_list || !dev->dont_cond_list){
		find_ctx->event = FIND_NAN_COND_OR_DONT_COND;
		ItemStr *item = malloc(sizeof(ItemStr));
		snprintf(item->name, MAX_NAME_DEV, dev->name);
		item->next = (struct ItemStr *)find_ctx->head;
		find_ctx->head = item;
	}
}

static void cleanup_find_item_str(FindCtx *find_ctx){
 	ItemStr *tmp;
	while(find_ctx->head){
		tmp = find_ctx->head;
		find_ctx->head = (ItemStr *)find_ctx->head->next;
		free(tmp);
	}
}
// Найти путь от current_id до 'target_id'
int find_path(int current_id, int target_id, AppState *app, 
			int depth, DevScb *target, int *idx, FindCtx *find_ctx) {
    // Если мы дошли до цели
    if (current_id == target_id) {
        for (int i = 0; i < depth; i++) printf("  │ ");
      	*idx += 1;
		if(target)
			malloc_cond(target);
			   
        printf("──> [ЦЕЛЬ ДОСТИГНУТА в %d раз! ID:%d]\n", *idx, target_id);
        return 1; 
    }

    app->visited[current_id] = 1;

 	int idx_dev = app->graph[current_id].from_idx_device;
 	int idx_con = app->graph[current_id].from_idx_contact;
 	
	DevScb *dev = &app->devices[idx_dev].dev;
 	Contact *con = &dev->contacts[idx_con];

    // Визуализация текущего шага
    for (int i = 0; i < depth; i++) printf("  │ ");
    printf("──> Проверка %s (%d-%d)\n", dev->name, con->axial, con->second);

    int found = 0;
    Edge *curr = app->graph[current_id].head;
    
    while (curr != NULL) {

        int neighbor = app->devices[curr->to_idx_device].dev.contacts[curr->to_idx_contact].idx_graph;
        
        if (!app->visited[neighbor]) {
            if (find_path(neighbor, target_id, app, depth + 1, target, idx, find_ctx)) {

				if(target)
					set_event_cond(target, dev, con);
				else
					check_condition(dev, find_ctx);
						
                found = 1;
            }
        }
        curr = (Edge *)curr->next;
    }

    // Очищаем для других веток
    app->visited[current_id] = 0;
    return found;
}

/*внутри кода будем идти от цели к плюсу, и от цели к минусу. Так как у нас
общий плюс и минус для всех, это позволит избежать лишних прыжков по графу
Поэтому при вызове find_path мы передаем current_id реле которое нужно прострелить,
получается с него мы должны начать поиск,а target_id это конечная цель тоесть Плюс.
Второй момент так как не все реле контакты которых участвуют в цепи могут иметь описание,
нужно передать информацию о этих реле, поэтому обход будет происходить в два этапа

Первый и второй этап будут отличаться передачей параметра DevScb *target, для первого прохода
передаем NULL, для второго реле цель. 
1) Первый этап делаем обход от реле до плюса, если цель достигнута, проверяем cond или dont_cond на null,
если там null заполняем FindCtx и так по каждому реле в цепи, если везде прописаны условия то стуктуру
не трогаем,(для этого изначально нужно инициализировать поле event контантой FIND_OK), иначе
указываем причину что пошло не так. Далее проходим от реле до минуса.
2) Второй этап будет зависить от FindCtx если все нормально то проходим еще раз и уже заполняем 
CondList. 
*/
int find_connection(const char *target_name, int t_ax, int t_sec,
				FindCtx *find_ctx, AppState *app) {
    
    DevScb *r_start = get_dev(target_name, app);
    if(!r_start){
    	return get_find_event(r_start->name, FIND_INCORRECT_DEV_NAME, find_ctx);
    }

	DevScb *r_end = get_dev("П", app);
	if(!r_end){
		return get_find_event("П", FIND_NAN_PLUS, find_ctx);
	}	

	DevScb *r_end_M = get_dev("М", app);
	if(!r_end_M){
		return get_find_event("М", FIND_NAN_MINUS, find_ctx);
	}
 
	
    Contact *c_start = get_contact(r_start, t_ax, t_sec);
    if(!c_start){
    	return get_find_event(r_start->name, FIND_NAN_CONTACT, find_ctx);
    }
    Contact *c_end = r_end->contacts;
	if(!c_end){
		return get_find_event(r_end->name, FIND_NAN_CONTACT_PLUS, find_ctx);
	}
	Contact *c_end_M = r_end_M->contacts;
	if(!c_end_M){
		return get_find_event(r_end->name, FIND_NAN_CONTACT_MINUS, find_ctx);
	}
	
    printf("\nИщем цепь питания %s\n", target_name);
    
    // Сбрасываем visited перед поиском
    for(int i = 0; i < 1000; i++) app->visited[i] = 0;

	/* Делаем первый обход от Цели до Плюса, проверяем достигнет ли цели и 
	добавлены ли условия подтачивания для реле контакты которых участвуют
*/
	int count_res = 0;
	find_ctx->event = FIND_OK;
	find_ctx->head = NULL;
	cleanup_find_item_str(find_ctx);
	
    if (!find_path(c_start->idx_graph, c_end->idx_graph, 
    			app, 0, NULL, &count_res, find_ctx)) {
        printf("Цепь не может быть собрана (путь не найден).\n");
        return get_find_event(target_name, FIND_NAN_LINKS_P, find_ctx);
	}
	// Здесь будем делать проход от Цели до Минуса

    if (!find_path(c_start->idx_graph, c_end_M->idx_graph, 
    		app, 0, NULL, &count_res, find_ctx)) {
        printf("Цепь не может быть собрана (путь не найден).\n");
        return get_find_event(target_name, FIND_NAN_LINKS_M, find_ctx);
    }

    	
// Второй этап проходов
	if (!find_path(c_start->idx_graph, c_end->idx_graph, 
				app, 0, r_start, &count_res, find_ctx)) {
	    printf("Цепь не может быть собрана (путь не найден).\n");
	    return get_find_event(target_name, FIND_NAN_LINKS_P, find_ctx);
	}

	if (!find_path(c_start->idx_graph, c_end_M->idx_graph, 
			app, 0, r_start, &count_res, find_ctx)) {
	    printf("Цепь не может быть собрана (путь не найден).\n");
	    return get_find_event(target_name, FIND_NAN_LINKS_M, find_ctx);
	}
    	
 // Проверка того что получилось
    CondList *curr = r_start->cond_list;
    int count = 1;
    while(curr != NULL){
		CondNode *curr_node = curr->cond_node;
		while(curr_node != NULL){
			printf("%d лист: event %d, number %d\n", count, 
				curr_node->condition->event, curr_node->condition->number);
			curr_node = (CondNode *)curr_node->next ;
		}
		count++;
    	curr = (CondList *)curr->next_list;
    }

	return 0;  
}

