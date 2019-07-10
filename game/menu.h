#ifndef MENU_H
#define MENU_H

typedef char* type;

//#include <cstddef>

struct Tree { // ХОРОШО БЫ ЗАПИЛИТЬ МЕНЮ СИНГЛТОНОМ... ВПРОЧЕМ, И ТАК СОЙДЕТ.	
	type content;
	int begin; // координата начала выбираемого контента
	Tree * branchs; // массив веток, идущих от этого корня
	Tree * parrent; // указатель на родительскую ветку (ака корень)
	size_t num; // количество веток, идущих от корня
};


#endif

