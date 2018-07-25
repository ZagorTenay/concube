#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

void parse_element_names(xmlNode * a_node);
void xmlToCsv(char *out);
void taketags(xmlNode * node);
void takecontents(xmlNode * node);
xmlNode* tags[256];
xmlNode* data[256];

static void convertXmltoCsv(char *in, char *out)
{	
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	doc = xmlReadFile(in, NULL, XML_PARSE_NOBLANKS);

	if (doc == NULL)
	{
		printf("error: could not parse file %s\n", in);
	}
	else
	{
		/* Get the root element node*/
		root_element = xmlDocGetRootElement(doc);

		parse_element_names(root_element);
		xmlToCsv(out);
		
		/*free the document*/
		xmlFreeDoc(doc);
	}
	/*Free the global variables that may have been allocated by the parser.*/
	xmlCleanupParser();

}
/* Recursive function that prints the XML structure */

void taketags(xmlNode * node)
{
	int i = 0, flag = 0;
	
	while(tags[i])
	{	
		if(strcmp(tags[i]->name, node->name) == 0)
		{
			flag=1;
		}
		i++;
	}
	
	if(flag == 0)
	{
		tags[i] = node;	
	}

	i = 0;
}

void takecontents(xmlNode * node)
{
	int i = 0;

	while(data[i])
	{	
		i++;
	}

	data[i] = node;

	i = 0;
}

void xmlToCsv(char *out)
{

	FILE *fp = fopen(out,"w");	
	
	int i = 0, j = 0;	
	
	while(tags[i])
	{
		if(tags[i + 1] == NULL)
		{				
			fprintf(fp, "%s\n", tags[i]->name);
		}
		else
		{
			fprintf(fp, "%s,", tags[i]->name);
		}		
		i++;		
	}	

	while(data[j])
	{	
		j++;
		
		if(j % i == 0)
		{
			fprintf(fp, "%s\n", data[j-1]->children->content);
		}
		else
		{
			fprintf(fp, "%s,", data[j-1]->children->content);	
		}				
	}

	fclose(fp);	
}

void parse_element_names(xmlNode * a_node)
{

	xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		
		if (cur_node->type == XML_ELEMENT_NODE && cur_node->children->content != NULL) {
			taketags(cur_node);
			takecontents(cur_node);
		}
		parse_element_names(cur_node->children);
	}	

}
