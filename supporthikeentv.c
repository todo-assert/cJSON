
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#include<sys/types.h>
#define __USE_MISC
#include<dirent.h>

#ifndef DT_DIR
#define DT_DIR 4
#endif /* DT_DIR */
#ifndef DT_ISDIR
#define DT_ISDIR(mode) (mode == DT_DIR)
#endif /* DT_ISDIR */
#ifndef DT_ISREG
#define DT_ISREG(mode) (mode == DT_REG)
#endif /* DT_ISREG */


static int add_config(char *search_path, cJSON *item)
{
	int size = 0;
	DIR *dir;
	struct dirent *dirent;
	dir = opendir(search_path);
	if( dir == NULL ) {
		return size;
	}
	while((dirent = readdir(dir))!=NULL) {
		if(DT_ISREG(dirent->d_type)) {
			/* printf("file name: %s \n", dirent->d_name); */
			cJSON_AddItemToArray(item, cJSON_CreateString(dirent->d_name));
			size ++;
		}
	}
	closedir(dir);

	return size;
}


int main(int argc, char **argv) 
{
	int ret = -1;

	DIR *dir;
	struct dirent *dirent;
	char path[1024] ;
	__attribute__((unused)) char *search_path = argv[1];
	__attribute__((unused)) char *genfile = argv[2];
	__attribute__((unused)) char *out;
	__attribute__((unused)) cJSON *root, *support, *item;
	if( argc < 3 ) {
		return ret;
	}
	dir = opendir(search_path);
	if( dir == NULL ) {
		return ret;
	}
	root = 	cJSON_CreateObject();
	support = 	cJSON_CreateObject();
	while((dirent = readdir(dir))!=NULL) {
		if( strcmp(".", dirent->d_name) == 0 || strcmp("..", dirent->d_name) == 0 ) continue ;
		if(DT_ISDIR(dirent->d_type)) {
			/* printf("dir name: %s \n", dirent->d_name); */
			item = 	cJSON_CreateArray();
			sprintf(path, "%s/%s", search_path, dirent->d_name);
			add_config(path, item);
			cJSON_AddItemToObject(support, dirent->d_name, item);
		}
	}

	cJSON_AddItemToObject(root, "support", support);
	out=cJSON_Print(root);
	printf("\n%s\n", out);

	return ret;
}