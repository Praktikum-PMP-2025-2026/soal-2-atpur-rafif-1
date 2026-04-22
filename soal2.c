#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100

#define MIN (8 * 60)
#define MAX (18 * 60)

struct Item{
	char name[MAX_WORD];
	int accepted;
	int rejected;
	char flag;
	struct Item* next;
};

void insertItem(struct Item** items, struct Item* newItem){
	if(*items == NULL){
		*items = newItem;
		return;
	}

	struct Item* current = *items;
	while(current->next != NULL)
		current = current->next;

	current->next = newItem;
}

struct Item* getItem(struct Item* items, char* name){
	struct Item* current = items;
	while(current != NULL){
		if(strcmp(current->name, name) == 0) return current;
		current = current->next;
	}

	return NULL;
}

struct Item* getMin(struct Item* items){
	struct Item* current = items;
	struct Item* min = NULL;
	while(current != NULL){
		char considered = current->flag;
		char isLess = min == NULL || (strcmp(current->name, min->name) < 0);
		if(considered && isLess)
			min = current;
		current = current->next;
	}

	return min;
}

struct Item** sort(struct Item* items, int length){
	struct Item* current = items;
	while(current != NULL){
		current->flag = 1;
		current = current->next;
	}

	struct Item** sorted = malloc(sizeof(struct Item*) * length);
	for(int i = 0; i < length; ++i){
		struct Item* min = getMin(items);
		min->flag = 0;
		sorted[i] = min;
	}

	return sorted;
}

int main(){
	char filename[MAX_WORD];
	scanf("%s", filename);

	FILE* file = fopen(filename, "r");

	char date[MAX_WORD];
	fscanf(file, "%s", (char*)&date);

	struct Item* items = NULL;

	int hour, minute, count;
	char name[MAX_WORD];
	while(fscanf(file, "%d:%d %s %d", &hour, &minute, (char*)&name, &count) == 4){
		struct Item* item = getItem(items, name);
		char tmp = item == NULL;
		if(item == NULL) {
			item = malloc(sizeof(struct Item));
			insertItem(&items, item);

			strcpy(item->name, name);
			item->accepted = 0;
			item->rejected = 0;
			item->next = NULL;
		}

		int totalMinute = hour * 60 + minute;
		if(MIN <= totalMinute && totalMinute <= MAX){
			item->accepted += count;
		} else {
			item->rejected += count;
		}
	}

	int length = 0;
	struct Item* current = items;
	while(current != NULL){
		length += 1;
		current = current->next;
	}

	struct Item** sorted = sort(items, length);

	printf("DITERIMA\n");
	for(int i = 0; i < length; ++i){
		struct Item* current = sorted[i];
		if(current->accepted != 0)
			printf("%s %d\n", current->name, current->accepted);
	}

	printf("DITOLAK\n");
	for(int i = 0; i < length; ++i){
		struct Item* current = sorted[i];
		if(current->rejected != 0)
			printf("%s %d\n", current->name, current->rejected);
	}
}
