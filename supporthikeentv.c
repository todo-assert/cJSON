
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#include<sys/types.h>
#ifndef __USE_MISC
#define __USE_MISC
#endif
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


static __attribute__((unused)) int add_config(char *search_path, cJSON *item)
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

extern int print_usage(char *target);
int print_usage(char *target)
{
	printf(
		"%s common_dirent config_dirent\n\n", target
		);
	return 0;
}

int main(int argc, char **argv) 
{
	int ret = -1;

	DIR *dir;
	struct dirent *dirent;
	char path[1024] ;
	size_t size;
	int itemsize, i;
	size_t len;
	char *pbuf;
	__attribute__((unused)) char *search_path = argv[1];
	__attribute__((unused)) char *inputfile = argv[2];
	__attribute__((unused)) char *input_path = argv[2];
	__attribute__((unused)) char *genfile = argv[3];
	__attribute__((unused)) char *out;
	FILE *fp;
	__attribute__((unused)) cJSON *top, *root, *support, *item, *input, *object, *config, *childobj;
	if( argc < 2 ) {
		print_usage(argv[0]);
		return ret;
	}
	dir = opendir(search_path);
	if( dir == NULL ) {
		return ret;
	}
	top = 	cJSON_CreateObject();
	root = 	cJSON_CreateObject();
	config = 	cJSON_CreateObject();
#if 0
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
	closedir(dir);
#endif

	cJSON_AddItemReferenceToObject(top, "company", cJSON_CreateString("SHENZHEN HIKEEN TECHNOLOGY CO.,LTD"));
	cJSON_AddItemReferenceToObject(top, "version", cJSON_CreateString("CJSON " LIBVERSION));
	cJSON_AddItemReferenceToObject(top, "tools version", cJSON_CreateString(GIT_VERSION));
	cJSON_AddItemReferenceToObject(top, "author", cJSON_CreateString("longqi"));
	cJSON_AddItemReferenceToObject(top, "email", cJSON_CreateString("1218715400@qq.com"));
	cJSON_AddItemToObject(top, "hikeentv", root);
	// cJSON_AddItemToObject(root, "support", support);
	cJSON_AddItemToObject(root, "config", config);

// add all json
	dir = opendir(input_path);
	if( dir == NULL ) {
		return ret;
	}
	while((dirent = readdir(dir))!=NULL) {
		if( strcmp(".", dirent->d_name) == 0 || strcmp("..", dirent->d_name) == 0 ) continue ;
		if(DT_ISDIR(dirent->d_type)) continue ;
		len = strlen(dirent->d_name);
		if( len > 5 && strcasecmp(".json", &dirent->d_name[len-5]) == 0 ) {
			printf("open %s\n", dirent->d_name);
			sprintf(path, "%s/%s", input_path, dirent->d_name);
			if(NULL != (fp = fopen(path, "r"))) {
				fseek(fp, 0, SEEK_END);
				size = (size_t ) ftell(fp) + 1;
				fseek(fp, 0, SEEK_SET);
				pbuf = (char *)malloc(size);
				if(pbuf != NULL && (0 != fread(pbuf, 1, size, fp))) {
					input=cJSON_Parse(pbuf);
					if(input) {
						object = cJSON_GetObjectItem(input,"config");
						itemsize = cJSON_GetArraySize(object);
						for(i=0;i<itemsize;i++) {
							childobj = cJSON_GetArrayItem(object, i);
							if(childobj)
								cJSON_AddItemToArray(config, cJSON_Duplicate(childobj, 1));
						}
						cJSON_Delete(input);
					}
					free(pbuf);
				}
				fclose(fp);
			}
		}
	}
	closedir(dir);

/*
	if(NULL != (fp = fopen(inputfile, "r"))) {
		fseek(fp, 0, SEEK_END);
		size = (size_t ) ftell(fp) + 1;
		fseek(fp, 0, SEEK_SET);
		pbuf = (char *)malloc(size);
		if(pbuf != NULL && (0 != fread(pbuf, 1, size, fp))) {
			input=cJSON_Parse(pbuf);
			if(input) {
				object = cJSON_GetObjectItem(input,"config");
				cJSON_AddItemToArray(root, object);
			}
			free(pbuf);
		}
		fclose(fp);
	}
*/
	out=cJSON_Print(top);
	if( genfile == NULL )
		printf("\n%s\n", out);
	else {
		fp =fopen(genfile, "w");
		fwrite(out, sizeof(char), strlen(out), fp);
		fclose(fp);
	}
	ret = 0;

	return ret;
}
