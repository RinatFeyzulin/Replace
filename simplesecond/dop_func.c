#include "simplesecond.h"


void print_tree(int v, AppState *app, int depth) {
    Contact *con = app->graph[v].from_contact;
    Relay *rel = (Relay *)con->source_dev;

    // Рисуем отступы для визуализации дерева
    for (int i = 0; i < depth; i++) printf("  │ ");
    
    // Печатаем текущий контакт
    printf("──> [%s: %d-%d] (ID:%d)\n", rel->name, con->axial, con->second, v);

    // Чтобы не войти в бесконечный цикл (если есть кольца в схеме)
    // используем локальныйvisited для текущей ветки
    app->visited[v] = 1;

    Edge *curr = app->graph[v].head;
    while (curr != NULL) {
        int neighbor = curr->to_contact->id;
        
        if (!app->visited[neighbor]) {
            print_tree(neighbor, app, depth + 1);
        } else {
            // Если мы тут уже были в этой ветке — это кольцо
            for (int i = 0; i <= depth; i++) printf("  │ ");
            printf("  (цикл на ID:%d)\n", neighbor);
        }
        curr = (Edge *)curr->next;
    }

    // ВАЖНО: снимаем отметку при выходе из рекурсии, 
    // чтобы этот контакт был доступен для других путей
    app->visited[v] = 0;
}


void dfs_go(const char *name, int ax, int sec, AppState *app){
	Relay *rel = get_relay(name, app);
	Contact *con = get_contact(rel, ax, sec);
	int v = con->id;
	print_tree(v, app, 0);	
}

void dfs(int v, Vertex *graph, int *visited){

	visited[v] = 1;
	Contact *con = graph[v].from_contact;

	Relay *rel = (Relay *)con->source_dev;
	
	printf("Проходим через контакты: %d - %d, реле %s\n", con->axial, con->second, 
		rel->name);

	Edge *curr = graph[v].head;

	while(curr != NULL){
		int neighbor = curr->to_contact->id;

		if(!visited[neighbor]){
			dfs(neighbor, graph, visited);
		}

		curr = (Edge *)curr->next;
	}
}


