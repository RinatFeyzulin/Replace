#include "simplesecond.h"


int add_edge(Contact *from, Contact *to, AppState *app){
 
	int from_idx = from->idx_graph;

	Edge *new_edge = malloc(sizeof(Edge));
	new_edge->to_idx_device = to->idx_source_dev;
	new_edge->to_idx_contact = to->idx_dev;
	
	new_edge->next = (struct Edge *)app->graph[from_idx].head;

	app->graph[from_idx].from_idx_device = from->idx_source_dev;
	app->graph[from_idx].from_idx_contact = from->idx_dev;
	app->graph[from_idx].head = new_edge;
	
	return 0;
}


int add_links(const char *from, int from_ax, int from_sec, 
								const char *to, int to_ax, int to_sec, AppState *app){
	DevScb *src = get_dev(from, app);
	DevScb *dest = get_dev(to, app);

	if(src == NULL || dest == NULL) return -1;

	Contact *src_con = get_contact(src, from_ax, from_sec);
	if(src_con == NULL) return -1;

	Contact *dest_con = get_contact(dest, to_ax, to_sec);
	if(dest_con == NULL) return -1;

	add_edge(src_con, dest_con, app);
	
	return 0;
	
}

