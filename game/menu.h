#ifndef MENU_H
#define MENU_H

typedef char* type;

//#include <cstddef>

struct Tree { // ������ �� �������� ���� ����������... �������, � ��� ������.	
	type content;
	int begin; // ���������� ������ ����������� ��������
	Tree * branchs; // ������ �����, ������ �� ����� �����
	Tree * parrent; // ��������� �� ������������ ����� (��� ������)
	size_t num; // ���������� �����, ������ �� �����
};


#endif

