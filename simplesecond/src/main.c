#include "simplesecond.h"
//#include "./ui/ui.h"
#include "./sqlite/sqlite_rep.h"
#include "./handle/handle.h"
void create_relays(AppState *app);
void create_links(AppState *app);
void create_condition(AppState *app);
/*TODO: наладить сохранение кода на гитхаб*/

/*TODO: навести порядок в handle, описать каждую функцию в handle.h*/

/*TODO: Добавить возможность добавлять условия подтачивания на выбор из enum CondEvent, 
и что если условия подтачивания делать просто из строки, тогда можно будет формировать
нормальный текст, который будет добавляться уже в другие цепи питания,
но ошибки в описании будут на совести механиков + трудно будет анализировать такие
массивы строк на поторяющиеся действия.
1) написать ui для получения инфы для условий подтачивания, для отдельного реле
2) написать sql для занесения в базу инфу которую собрал
3) написать init для загрузки из базы инфу которая внесена
4) написать функцию cleanup_condition (simplesecond.c)
	для освобождения CondList сделать функцию универсальной,
	то есть чтобы можно было и вложенные листы тоже очистить 
*/

/*TODO: разработать еще один обработчик для формирования готового текста,
который потом можно будет уже собирать в файл*/

/*TODO: Что если сделать вершину не пару контактов,а контакт по отдельности, 
тогда можно будет потом анализировать монтажную и принципиальную части, оставить
логику добавления 1П:11-12=2П:11-12, а внутри связывать уже 1П:11 и 1П:12, 1П:12 и 2П:11,
только нужно будет предупреждать если тройник развернут в таком виде его и добавлять*/
/*TODO: привязывать реле и контакты которые в цепи участвуют к конкретному плюсу,
это позволит предотврать повторное добавление пары контактов одного и того же реле
в разные цепи питания*/
/*TODO: нужно продумать, что если уже один раз был прострел, тогда нужно будет освобождать память
от прошлого прохода, дальше каким образом это все хранить в базе данных? Единственное что приходит в голову
это собирать все в строку и отправлять в базу, либо вообще не отправлять так как в базе уже есть связи
а алгоритм будет просто по ним проходить и формировать уже условия, в базе только хранить те условия 
подтачивания реле, в цепи которых нет контактов других реле*/
/*TODO: все таки стоит хранить также списки контактов которые участвуют в цепи, это поможет посмотреть
какие связи уже добавлены, вдруг что то упустили*/
/*TODO: Удалить после окончания тестирования, ACTION_SHOW_DEVICES(handle.h), */

int main(void)
{
	AppState app;
	memset(&app, 0, sizeof(app));

	init_app(&app); 
	
//	create_relays(&app);
//	create_links(&app);	
	create_condition(&app);
	
	int c;
	while(app.running){
		c = request_action();

		if(c == -1) 
			break;

		handler_action(c, &app);
	}
	
	printf("Aufwidersein\n");
	return 0;

	
}

void create_links(AppState *app){
	add_logic_links("sy", 11, 12, "3П", 11, 12, app->db);
	add_logic_links("3П", 11, 12, "sy", 11, 12, app->db);

	add_logic_links("sy", 11, 12, "1П", 11, 12, app->db);
	add_logic_links("1П", 11, 12, "sy", 11, 12, app->db);

	add_logic_links("sy", 11, 12, "1П", 11, 13, app->db);
	add_logic_links("1П", 11, 13, "sy", 11, 12, app->db);
		
	add_logic_links("3П", 11, 12, "1c", 1, 2, app->db);
	add_logic_links("1c", 1, 2, "3П", 11, 12, app->db);

	add_logic_links("1П", 11, 12, "1c", 1, 2, app->db);
	add_logic_links("1c", 1, 2, "1П", 11, 12, app->db);
 
	add_logic_links("1П", 11, 13, "2П", 11, 12, app->db);
	add_logic_links( "2П", 11, 12,"1П", 11, 13, app->db);

	add_logic_links("2П", 11, 12, "2c", 1, 2, app->db);
	add_logic_links("2c", 1, 2,"2П", 11, 12, app->db);

	add_logic_links("2kn", 21, 22, "2c", 3,4, app->db);	
	add_logic_links("2c", 3,4,"2kn", 21, 22, app->db);
}

void create_relays(AppState *app){

	insert_dev("1П",_1N_1350, app->db);
	add_dev("1П",_1N_1350, app);

	add_dev("2П", _1N_1350, app);
	insert_dev("2П", _1N_1350, app->db);


	add_dev("3П", _1N_1350, app);
	insert_dev("3П", _1N_1350, app->db);

	add_dev("91k", _1N_1350, app);
	insert_dev("91k", _1N_1350, app->db);

	add_dev("92k", _1N_1350, app);
	insert_dev("92k", _1N_1350, app->db);

	add_dev("1kn", _1N_1350, app);
	insert_dev("1kn", _1N_1350, app->db);

	add_dev("2kn", _1N_1350, app);
	insert_dev("2kn", _1N_1350, app->db);

	add_dev("sy", _1N_1350, app);
	insert_dev("sy", _1N_1350, app->db);
	
	add_dev("1c", _1N_1350, app);
	insert_dev("1c", _1N_1350, app->db);

	add_dev("2c", _1N_1350, app);
	insert_dev("2c", _1N_1350, app->db);

}

void create_condition(AppState *app){
	add_condition("1П", TRACK_CLEAR, 1, app);
	add_condition("1П", TRACK_OCUPP, 1, app);

	add_condition("2П", TRACK_CLEAR, 2, app);
	add_condition("2П", TRACK_OCUPP, 2, app);

	add_condition("91k", CLICK_BTN, 91, app);
	add_condition("91k", DONT_CLICK,91, app);

	add_condition("92k", CLICK_BTN, 92, app);
	add_condition("92k", DONT_CLICK, 92, app);
	
}
