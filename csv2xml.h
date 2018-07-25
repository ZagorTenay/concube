#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h> 
#include <libxml/tree.h> 


static void convertCsvtoXml(char *in, char *out, int compressMode)
{
	
	int compMode = compressMode; // 0'da compress mode açık 1'de değil
	xmlDocPtr doc = NULL;       /* document pointer */ 
    	xmlNodePtr root_node = NULL, node = NULL;
    	xmlDtdPtr dtd = NULL;       /* DTD pointer */ 

	doc = xmlNewDoc(BAD_CAST "1.0"); 
    	root_node = xmlNewNode(NULL, BAD_CAST "root"); 
    	xmlDocSetRootElement(doc, root_node); 

	int i = 0, j = 0, textcounter = 0;

	char* datas[1000];
	char* texts[1000];
	char *token, *token2;
	FILE *fp = fopen(in,"r"); 
	char buf1[2000];
	char buf2[2000];

	if(fp == NULL)
	{
		printf("ERROR!!");
	}
	else
	{
		fgets(buf1, 2000, fp);		
			
		token = strtok(buf1, ",\n");
		while (token != NULL)
		{
	
			datas[i] = token;
			token = strtok(NULL, ",\n");
			i++;
		}	

		while (fgets(buf2, 2000, fp))
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
			node = xmlNewChild(root_node, NULL, "element", BAD_CAST NULL);
			for(j = 0; j < i; j++)
			{
				if(compMode == 0)
					xmlNewChild(node, NULL, datas[j], BAD_CAST texts[j]);
				else 
					xmlNewProp(node, BAD_CAST datas[j], BAD_CAST texts[j]);		
			}
			
		}		
		
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
	fclose(fp);
	
}
