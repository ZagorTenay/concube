#include <json/json.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h> 
#include <libxml/tree.h> 



void lastOp(json_object *jobj, char* key, xmlNodePtr root, int compressMode){
  	enum json_type type;
  	type = json_object_get_type(jobj); /*Getting the type of the json object*/
  	switch (type) {
    		case json_type_boolean:
                        break;
    		case json_type_double:
			if(compressMode == 1)
			{
				xmlNewProp(root, BAD_CAST key, BAD_CAST json_object_get_string(jobj));
			}
			else
			{
				xmlNodePtr node1 = xmlNewChild(root, NULL, key, BAD_CAST json_object_get_string(jobj));
			}
                        break;
    		case json_type_int:

			if(compressMode == 1)
			{
				xmlNewProp(root, BAD_CAST key, BAD_CAST json_object_get_string(jobj));
			}
			else
			{
				xmlNodePtr node1 = xmlNewChild(root, NULL, key, BAD_CAST json_object_get_string(jobj));
			}
                        break;
    		case json_type_string:
			if(compressMode == 1)
			{
				xmlNewProp(root, BAD_CAST key, BAD_CAST json_object_get_string(jobj));
			}
			else
			{
				xmlNodePtr node1 = xmlNewChild(root, NULL, key, BAD_CAST json_object_get_string(jobj));
			}
                        break;
  	}

}

void parse_array(json_object *jobj, char *key, xmlNodePtr root, int compressMode) {

 	void jsonParse(json_object * jobj, xmlNodePtr root, int compressMode);
  	enum json_type type;

  	json_object *jarray = jobj; /*Simply get the array*/
  	if(key) {
    		jarray = json_object_object_get(jobj, key); /*Getting the array if it is a key value pair*/
  	}
  	int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/

  	int i;
  	json_object * jvalue;
  	for (i=0; i< arraylen; i++){

    		jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/

		xmlNodePtr node = xmlNewChild(root, NULL, key, BAD_CAST NULL);
    		type = json_object_get_type(jvalue);

    		if (type == json_type_array) {
      			parse_array(jvalue, NULL, node, compressMode);
    		}
    		else if (type != json_type_object) {
      			lastOp(jvalue, key, node, compressMode);
    		}
    		else {
      			jsonParse(jvalue, node, compressMode);
    		}
  	}
}

/*Parsing the json object*/
void jsonParse(json_object * jobj, xmlNodePtr root, int compressMode) {
  	enum json_type type;
  	json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
    	type = json_object_get_type(val);
	xmlNodePtr node;
    	switch (type) {
      		case json_type_boolean:
			break;
      		case json_type_double:
			break;
      		case json_type_int:
                        lastOp(val, key, root, compressMode);  
			break;
      		case json_type_string: 
			lastOp(val, key, root, compressMode);                   
			break;
      		case json_type_object: 
			node = xmlNewChild(root, NULL, key, BAD_CAST NULL);
                        jobj = json_object_object_get(jobj, key);
                        jsonParse(jobj, node, compressMode);
                        break;
      		case json_type_array:
                        parse_array(jobj, key, root, compressMode);
                        break;
    		}
  	}
}


static void convertJsontoXml(char *in, char *out, int compressMode) {

    	xmlDocPtr doc = NULL;       /* document pointer */ 
    	xmlNodePtr root_node = NULL;/* node pointers */ 
    	xmlDtdPtr dtd = NULL;       /* DTD pointer */ 
    	char buff[2000]; 

    	/* 
    	 * Creates a new document, a node and set it as a root node 
     	*/ 
   	doc = xmlNewDoc(BAD_CAST "1.0"); 
    	root_node = xmlNewNode(NULL, BAD_CAST "root"); 
    	xmlDocSetRootElement(doc, root_node); 



	char* data[2000];
	char* token;
	char buffer[2000];

 	FILE *fp = fopen(in,"r");
	int i=0;

	while (fgets(buffer, 2000, fp))
	{
		token = buffer;			
		data[i] = malloc(strlen(token)); 
	        strcpy(data[i],token); 
		json_object * jobj = json_tokener_parse(data[i]);
		jsonParse(jobj, root_node, compressMode);
		i++;

	}	
	

	xmlSaveFormatFileEnc(out, doc, "UTF-8", 1); 

    	/*free the document */ 
    	xmlFreeDoc(doc); 

    	/* 
     	*Free the global variables that may 
     	*have been allocated by the parser. 
     	*/ 
    	xmlCleanupParser(); 

    	xmlMemoryDump();

}
