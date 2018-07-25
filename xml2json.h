#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

json_object * jobjs[200];
json_object *jarray;
int arrayFlag;

static void createJsonFromXml(xmlNode * a_node, json_object * jobj);

static void convertXmltoJson(char *in, char *out)
{	
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	const char *Filename = in;
	doc = xmlReadFile(in, NULL, XML_PARSE_NOBLANKS);

	if (doc == NULL)
	{
		printf("error: could not parse file %s\n", Filename);
	}
	else
	{
		/*Creating a json object*/
		json_object * jobj = json_object_new_object();
		jarray = json_object_new_array();
		arrayFlag = 0;
	
		/* Get the root element node*/
		root_element = xmlDocGetRootElement(doc);

		createJsonFromXml(root_element, jobj);
		//printf ("The json object created: %s\n",json_object_to_json_string(jobj));
		FILE *fpout = fopen(out,"w");	
		fprintf(fpout, "%s", json_object_to_json_string(jobj));
		fclose(fpout);	
		/*free the document*/
		xmlFreeDoc(doc);
	}
	/*Free the global variables that may have been allocated by the parser.*/
	xmlCleanupParser();

}
/* Recursive function that prints the XML structure */



static void createJsonFromXml(xmlNode * a_node, json_object * jobj)
{

	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
	{	
		
		if (cur_node->type == XML_ELEMENT_NODE && cur_node->children->content != NULL) 
		{
			json_object *jstring1 = json_object_new_string(cur_node->children->content);
			json_object_object_add(jobj, cur_node->name, jstring1);
		}
		else if(cur_node->type == XML_ELEMENT_NODE && cur_node->children->content == NULL)
		{
			int i = 0;
			int flag = 0;
			while(jobjs[i])
			{	
				i++;			
			}
			
			jobjs[i] = json_object_new_object();

			if(cur_node->type == XML_ELEMENT_NODE && cur_node->children->children->content != NULL)
			{
				flag = 1;
				if(arrayFlag == 0)
				{
					arrayFlag = 1;
					json_object_object_add(jobj, cur_node->name, jarray);
				}
			}

			createJsonFromXml(cur_node->children, jobjs[i]);

			
			if(flag == 1)
			{
				json_object_array_add(jarray, jobjs[i]);
			}
			else
				json_object_object_add(jobj, cur_node->name, jobjs[i]);
		

			i=0;
		}
		
		
	}	

}
