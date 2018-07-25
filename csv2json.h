#include <json/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void convertCsvtoJson(char *in, char *out) {
	/*Creating a json object*/
	json_object * jobj = json_object_new_object();
	
	/*Creating a json array*/
	json_object *jarray = json_object_new_array();
	
	json_object_object_add(jobj, "elements", jarray);

	int i = 0, j = 0, textcounter = 0;

	char* datas[20];
	char* texts[20];
	char *token, *token2;
	FILE *fpin = fopen(in,"r"); 
	char buf1[2000];
	char buf2[2000];

	if(fpin == NULL)
	{
		printf("ERROR!!");
	}
	else
	{
		fgets(buf1, 2000, fpin);		
			
		token = strtok(buf1, ",\n");
		while (token != NULL)
		{
	
			datas[i] = token;
			token = strtok(NULL, ",\n");
			i++;
		}	

		while (fgets(buf2, 2000, fpin))
		{
			i = 0;
			token2 = strtok(buf2, ",\n");
			while (token2 != NULL)
			{
	
				texts[i] = token2;
				token2 = strtok(NULL, ",\n");
				i++;
			}
			j = 0;
			json_object *jarray2 = json_object_new_array();
			json_object * jobj2 = json_object_new_object();

			for(j = 0; j < i; j++)
			{
				json_object *jstring1 = json_object_new_string(texts[j]);
				json_object_object_add(jobj2, datas[j], jstring1);
				
			}

			if(jobj2 != NULL)
				json_object_array_add(jarray, jobj2);
		}		
		
	}

	FILE *fpout = fopen(out,"w");	
	fprintf(fpout, "%s", json_object_to_json_string(jobj));
	fclose(fpout);	
	
}
