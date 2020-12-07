#include <stdio.h>
#include <windows.h>
#include <malloc.h>
#include <conio.h>
#include <time.h>

#define MAX_TODO_TITLE_LENGTH 300
#define MAX_TODO_INFO_LENGTH 1000

typedef struct todo{
	unsigned short int year;
	unsigned char month, day, hour, min, clear;
	char title[MAX_TODO_TITLE_LENGTH];
	char info[MAX_TODO_INFO_LENGTH];
	struct todo *next;
} TODO;

int counttd(TODO *t_head){
	TODO *t_temp = t_head;
	int count = 0;
	while(t_temp != NULL){
		count++;
		t_temp = t_temp->next;
	}
	return count;
}

int counttdop(TODO *t_head, int page, int unit){
	int count = counttd(t_head) - (page - 1) * unit;
	if(count >= unit)
	return unit;
	
	if(count <= 0)
	return 0;
	
	return count;
}

void loadtd(TODO **t_head, TODO **t_tail){
	FILE *file = fopen("TODO.txt", "rt");
	TODO **t_temp = t_head;
	int count;
	
	fscanf(file, "%d", &count);
	if(file != NULL){
		for(int i = 0; i < count; i++){
			TODO *t_new = (TODO *)malloc(sizeof(TODO));
			fscanf(file, "%d %d %d %d %d %d", &t_new->year, &t_new->month, &t_new->day, &t_new->hour, &t_new->min, &t_new->clear);
			fseek(file, 2, SEEK_CUR);
			fscanf(file, "%[^\n]", t_new->title);
			fseek(file, 2, SEEK_CUR);
			fscanf(file, "%[^\n]", t_new->info);
			t_new->next = NULL;
			*t_temp = t_new;
			*t_tail = *t_temp;
			t_temp = &(*t_temp)->next;
		}
	}
	fclose(file);
}

void savetd(TODO *t_head){
	FILE *file = fopen("TODO.txt", "wt");
	TODO *t_temp = t_head, *t_free;
	fprintf(file, "%d\n", counttd(t_head));
	while(t_temp != NULL){
		fprintf(file, "\n%d\n%d\n%d\n%d\n%d\n%d\n", t_temp->year, t_temp->month, t_temp->day, t_temp->hour, t_temp->min, t_temp->clear);
		fprintf(file, "%s\n%s", t_temp->title, t_temp->info);
		t_free = t_temp;
		t_temp = t_temp->next;
		free(t_free);
	}
	fclose(file);
}

void addtd(TODO **t_head, TODO **t_tail){
	TODO *t_new = (TODO *)malloc(sizeof(TODO));
	struct tm *now;
	time_t timer; //CHECK TIME
	timer = time(NULL); //GET CURRENT TIME IN SECONDS
	now = localtime(&timer);
	
	t_new->year = now->tm_year + 1900;
	t_new->month = now->tm_mon + 1;
	t_new->day = now->tm_mday;
	t_new->hour = now->tm_hour;
	t_new->min = now->tm_min;
	system("cls");
	printf("--- ADD TODO ---\n");
	printf("TITLE : ");
	rewind(stdin);
	scanf("%[^\n]", t_new->title);
	printf("\nINFO : ");
	rewind(stdin);
	scanf("%[^\n]", t_new->info);
	t_new->clear = 0;
	t_new->next = NULL;
	
	if(*t_head == NULL){
		*t_head = t_new;
		*t_tail = t_new;
		return;
	}
	
	(*t_tail)->next = t_new;
	*t_tail = t_new;
}

void showtd(TODO *t_head, int unit, int page){
	TODO *t_temp = t_head;
	for(int i = 0; i < ((page - 1) * unit); i++){
		if(t_temp != NULL)
		t_temp = t_temp->next;
	}
	
	system("cls");
	printf("--- (%d of %d) PAGE ---\n\n", page, (counttd(t_head) / unit + 1));
	for(int i = 0; i < unit; i++){
		if(t_temp != NULL){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			printf(">%d ", i + 1);
			if(t_temp->clear){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				printf("%s\n\n", t_temp->title);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			else{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				printf("%s\n", t_temp->title);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf("   %s\n", t_temp->info);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				printf("   %d YEAR, %d MONTH, %d DAY, %d HOUR, %d MINUTE\n\n", t_temp->year, t_temp->month, t_temp->day, t_temp->hour, t_temp->min);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			t_temp = t_temp->next;
		}
	}
}

void findtail(TODO *t_head, TODO **t_tail){
	TODO *t_temp = t_head;
	if(t_head == NULL){
		*t_tail = NULL;
		return;
	}
	while(t_temp->next != NULL){
		t_temp = t_temp->next;
	}
	*t_tail = t_temp;
}

void deletetd(TODO **t_head, int num){
	TODO **t_temp = t_head, *t_free;
	if(counttd(*t_head) == 1 && num == 0){
		t_free = *t_temp;
		*t_head = NULL;
		free(t_free);
		return;
	}
	for(int i = 0; i < num; i++){
		if(t_temp != NULL)
		t_temp = &(*t_temp)->next;
	}
	if((*t_temp)->next != NULL){
		t_free = (*t_temp)->next;
		(*t_temp)->year = ((*t_temp)->next)->year;
		(*t_temp)->month = ((*t_temp)->next)->month;
		(*t_temp)->day = ((*t_temp)->next)->day;
		(*t_temp)->hour = ((*t_temp)->next)->hour;
		(*t_temp)->min = ((*t_temp)->next)->min;
		(*t_temp)->clear = ((*t_temp)->next)->clear;
		strcpy((*t_temp)->title, ((*t_temp)->next)->title);
		strcpy((*t_temp)->info, ((*t_temp)->next)->info);
		(*t_temp)->next = ((*t_temp)->next)->next;
		free(t_free);
	}
	else{
		t_free = *t_temp;
		*t_temp = NULL;
		free(t_free);
	}
}

void toggletd(TODO *t_head, int num){
	TODO *t_temp = t_head;
	for(int i = 0; i < num; i++){
		if(t_temp != NULL)
		t_temp = t_temp->next;
	}
	
	if(t_temp->clear)
	t_temp->clear = 0;
	else t_temp->clear = 1;
}

void debugtd(TODO *t_head, TODO *t_tail){
	system("cls");
	printf("--- DEBUG TODO ---\n\n");
	printf("HEAD TODO : %s\n\n", t_head->title);
	printf("TAIL TODO : %s", t_tail->title);
	Sleep(1000);
}

int main(void){
	TODO *t_head = NULL, *t_tail = NULL;
	int unit, page = 1, key;
	unsigned char deletemode = 0;
	loadtd(&t_head, &t_tail);
	while(1){
		system("cls");
		printf("MAX UNIT OF ONE PAGE : ");
		unit = getch() - 48;
		if(unit < 10 && unit > 0)
		break;
		
		system("cls");
		printf("MAX UNIT OF ONE PAGE IS MUST OVER 1, UNDER 10");
		Sleep(1000);
	}
	while(1){
		if(deletemode)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
		
		showtd(t_head, unit, page);
		key = getch();
		if(key - 48 <= counttdop(t_head, page, unit) && key - 48 > 0){
			if(deletemode){
				deletetd(&t_head, ((page - 1) * unit) + key - 49);
				deletemode = 0;
				findtail(t_head, &t_tail);	
			}
			else toggletd(t_head, ((page - 1) * unit) + key - 49);
		}
		switch(key){
			case 224 :
				switch(getch()){
					case 75 : //LEFT
						if(page > 1)
						page--;
						break;
					
					case 77 : //RIGHT
						if((counttd(t_head) / unit + 1) > page)
						page++;
						break;
				}
				break;
				
			case 0 :
				switch(getch()){
					case 59 : //F1
						addtd(&t_head, &t_tail);
						break;
					
					case 60 : //F2
						if(deletemode)
						deletemode = 0;
						else deletemode = 1;
						break;
					
					case 61 : //F3
						savetd(t_head);
						system("cls");
						return counttd(t_head);
					case 62 : //F4
						debugtd(t_head, t_tail);
						break;
				}
				break;
		}
	}
}
