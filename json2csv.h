#include <json/json.h>
#include <stdio.h>
#include <string.h>

char* keys[2560];
const char* values[2560];

static void takeKeys(char* key);
static void takeValues(const char* value);
static void jsonToCsv(char *out);

void print_json_value(json_object *jobj,char* key){
  	enum json_type type;
  	//printf("type: ",type);
  	type = json_object_get_type(jobj); /*Getting the type of the json object*/
  	switch (type) {
    		case json_type_boolean:
                       // printf("json_type_boolean\n");
                       // printf("value: %s\n", json_object_get_boolean(jobj)? "true": "false");
                        break;
    		case json_type_double:
                       // printf("json_type_double\n");
                       // printf("value: %lf\n", json_object_get_double(jobj));
                        break;
    		case json_type_int:
                        //printf("json_type_int\n");
                        //printf("value: %d\n", json_object_get_int(jobj));
			takeKeys(key);
			takeValues(json_object_get_string(jobj));
                        break;
    		case json_type_string:
                        //printf("json_type_string\n");
                       // printf("value: %s\n", json_object_get_string(jobj));
			takeKeys(key);
			takeValues(json_object_get_string(jobj));
                        break;
  	}

}

void json_parse_array(json_object *jobj, char *key) {
  	void json_parse(json_object * jobj);
  	enum json_type type;

  	json_object *jarray = jobj; /*Simply get the array*/
  	if(key) {
    		jarray = json_object_object_get(jobj, key); /*Getting the array if it is a key value pair*/
  	}

  	int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/
  	//printf("Array Length: %d key: %s\n",arraylen,key);
  	int i;
  	json_object * jvalue;

  	for (i=0; i< arraylen; i++){
    		jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/
    		type = json_object_get_type(jvalue);
    		if (type == json_type_array) {
      			json_parse_array(jvalue, NULL);
    		}
    		else if (type != json_type_object) {
      			//printf("value[%d]: ",i);
      			print_json_value(jvalue, key);
    		}
    		else {
      			json_parse(jvalue);
    		}
  	}
}

/*Parsing the json object*/
void json_parse(json_object * jobj) {
  	enum json_type type;
  	json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
    	//printf("type: ",type);
    	type = json_object_get_type(val);
    	switch (type) {
      		case json_type_boolean:
			break;
      		case json_type_double:
			break;
      		case json_type_int:
			print_json_value(val, key);
			break;
      		case json_type_string: 
			print_json_value(val, key);                   
			break;
      		case json_type_object:
			//printf("json_type_object\n");
                        jobj = json_object_object_get(jobj, key);
                        json_parse(jobj);
                        break;
      		case json_type_array:
                        //printf("type: json_type_array, ");
                        json_parse_array(jobj, key);
                        break;
    		}
  	}
}

static void takeValues(const char* value)
{
	int i = 0;
	
	while(values[i])
	{
		i++;
	}

	values[i] = value;	
	i = 0;	
	
}

static void takeKeys(char* key)
{
	int i = 0, flag = 0;
	
	while(keys[i])
	{
		if(strcmp(keys[i], key) == 0)
		{
			flag=1;
		}
		i++;
	}
	
	if(flag == 0)
	{
		keys[i] = key;	
	}
	i = 0;	
	
}

static void jsonToCsv(char *out)
{

	FILE *fp = fopen(out,"w");	
	
	int i = 0, j = 0;	
	
	while(keys[i])
	{
		if(keys[i + 1] == NULL)
		{				
			fprintf(fp, "%s\n", keys[i]);
		}
		else
		{
			fprintf(fp, "%s,", keys[i]);
		}		
		i++;		
	}	

	while(values[j])
	{	
		j++;
		
		if(j % i == 0)
		{
			fprintf(fp, "%s\n", values[j-1]);
		}
		else
		{
			fprintf(fp, "%s,", values[j-1]);	
		}				
	}
	fclose(fp);	
}

static void convertJsontoCsv(char *in, char *out) {
	char* data[2000];
	char* token;
	char buffer[2000];

 	FILE *fp = fopen(in,"r");
	int i=0;

	while (fgets(buffer,2000, fp))
	{
		token = buffer;				
		data[i] = malloc(strlen(token)); 
	        strcpy(data[i],token); 
		//printf("test : %s\n",data[i]);
		json_object * jobj = json_tokener_parse(data[i]);
		json_parse(jobj);
		i++;

	}	
	jsonToCsv(out);
	

}
