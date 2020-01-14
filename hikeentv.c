
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

static __attribute__((unused)) void release_json(cJSON **json)
{
	if(json == NULL) return;
	if(*json == NULL) return;
	cJSON_Delete(*json);
	*json = 0;
}

#define FAILED_OBJECT(obj) do{if(obj==NULL) printf("%s:%d parse object is null\n", __func__, __LINE__);}while(0)
static int parse_configs(const char *name, cJSON* json)
{
	__attribute__((unused)) int i, size;
    __attribute__((unused)) cJSON *main_object;
    __attribute__((unused)) cJSON *object;
    __attribute__((unused)) cJSON *item;
	if(strcmp(name, "keypad")==0)
	{
		size = cJSON_GetArraySize(json);
		for( i=0;i<size;i++ ) {
			item = cJSON_GetArrayItem(json, i);
			if(strcmp("type", item->string)==0)
				continue;
			if(item->type==cJSON_Number)
				printf("%s config %s > %d\n", name, item->string, item->valueint);
			else
				printf("%s config %s > %s\n", name, item->string, item->valuestring);
		}

	}
	return size;
}
static int dump_configs(const char *name, cJSON* json)
{
	int i, size;
    __attribute__((unused)) cJSON *main_object;
    __attribute__((unused)) cJSON *object;
    __attribute__((unused)) cJSON *item;
	main_object = cJSON_GetObjectItem(json, name);
	size = cJSON_GetArraySize(main_object);
	for( i=0;i<size;i++ ) {
		object = cJSON_GetArrayItem(main_object, i);   /* 因为这个对象是个数组获取，且只有一个元素所以写下标为0获取*/
		if(object == NULL) {
			FAILED_OBJECT(object);
			continue;
		}
		if( object->valuestring == NULL ) {
			item = cJSON_GetObjectItem(object,"path");
			if( item ) {
				printf("%s serach path %s\n", name, item->valuestring);
				continue;
			}
			item = cJSON_GetObjectItem(object,"type");
			if( item ) {
				printf("%s parse %s\n", name, item->valuestring);
				parse_configs(name, object);
				continue;
			}
			printf("%s try parse detail\n", name);
			parse_configs(name, object);
			continue;
		}
		printf("dump %s %s [%d] %s\n", json->string, name, i, object->valuestring);
	}

	return size;
}

int main(int argc, char **argv) {
    FILE            *fp = NULL;
    cJSON           *json;
    __attribute__((unused)) char            *out;
    char            *line;
    __attribute__((unused)) cJSON *main_object;
    __attribute__((unused)) cJSON *main_item;
    __attribute__((unused)) cJSON *sub_item;
    __attribute__((unused)) cJSON *object;
    size_t sz;
    __attribute__((unused)) int i=0, j, size, sub_size;

		if(NULL != (fp = fopen(argv[1], "r"))) {
			fseek(fp, 0, SEEK_END);
			sz = (size_t ) ftell(fp) + 1;
			fseek(fp, 0, SEEK_SET);
			line = (char *)malloc(sz);
			if(line==NULL) return -2;
			while (0 != fread(line, 1, sz, fp)) {
				json=cJSON_Parse(line); /* 获取整个大的句柄 */
				free(line);
				out=cJSON_Print(json);  /* 这个是可以输出的。为获取的整个json的值 */
				main_object = cJSON_GetObjectItem(json,"hikeentv");   /* 因为这个对象是个数组获取，且只有一个元素所以写下标为0获取*/

				size = cJSON_GetArraySize(main_object);
				for(i=0;i<size;i++) {
					main_item = cJSON_GetArrayItem(main_object, i);
					sub_size = cJSON_GetArraySize(main_item);
					for(j=0;j<sub_size;j++) {
						sub_item = cJSON_GetArrayItem(main_item, j);
						dump_configs(sub_item->string, main_item);
					}
				}
				/*
				cJSON_ReplaceItemViaPointer(main_object, cJSON_GetObjectItem(main_object,"support"), cJSON_Duplicate(cJSON_GetObjectItem(main_object,"config"), 1));
				cJSON_ReplaceItemInObject(main_object, "config", cJSON_CreateString("hello"));
				*/
				out=cJSON_Print(json);
				if(json==NULL) {
					return -1;
				}
				printf("\n%s\n", out);
				break;
			#if 0
				main_item = cJSON_GetObjectItem(main_object,"support");   /* 因为这个对象是个数组获取，且只有一个元素所以写下标为0获取*/
				main_item = cJSON_GetArrayItem(main_object, 0);
				dump_configs("panel", main_item);
				dump_configs("irmap", main_item);
				#if 0
				main_object = cJSON_GetObjectItem(main_item,"panel");
				size = cJSON_GetArraySize(main_object);
				for(i=0;i<size;i++) {
					object = cJSON_GetArrayItem(main_object, i);   /* 因为这个对象是个数组获取，且只有一个元素所以写下标为0获取*/
					if( object->valuestring == NULL ) continue;
					printf("hikeentv support panel [%d] %s\n", i, object->valuestring);
				}
				#endif
				main_item = cJSON_GetObjectItem(main_object,"config");   /* 因为这个对象是个数组获取，且只有一个元素所以写下标为0获取*/
				dump_configs("panel", main_item);
				dump_configs("irmap", main_item);
				break;
			#endif
			}
		}
		cJSON_Delete(json);
		fclose(fp);
		return 0;
}    
