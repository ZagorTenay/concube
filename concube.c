#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>
#include "xml2csv.h"
#include "json2xml.h"
#include "json2csv.h"
#include "xml2json.h"
#include "csv2xml.h"
#include "csv2json.h"

const char *get_extension(const char *fs);

int main(int argc, char **argv) {
	int i;
	int inflag = 0;
	int outflag = 0;
	int tflag = 0;
	int cmtrueflag = 0, cmfalseflag = 0;
	int compflag = 0;		/*compressed mode flag. if compflag = 0 , compress mode is off. if complag = 1 , compress mode is on*/
	char *input_fname = NULL;
	char *output_fname = NULL;
	char *t_fname = NULL;

	for(i = 1; i < argc; i++) {
		if(strcmp("-i", argv[i]) == 0) {
			if(i != argc - 1 && strcmp(argv[i + 1] , "-o") != 0 && strcmp(argv[i + 1] , "-t") != 0 && strcmp(argv[i + 1] , "-true") != 0 ){
				inflag = 1;
				input_fname = argv[i + 1];
			}
		} 
		else if(strcmp("-o", argv[i]) == 0) {
			if(i != argc - 1 && strcmp(argv[i + 1] , "-i") != 0 && strcmp(argv[i + 1] , "-t") != 0 && strcmp(argv[i + 1] , "-true") != 0){
				outflag = 1;
				output_fname = argv[i + 1];
			}
		}
		else if(strcmp("-t", argv[i]) == 0) {
			if(i != argc - 1 && strcmp(argv[i + 1] , "-o") != 0 && strcmp(argv[i + 1] , "-i") != 0 && strcmp(argv[i + 1] , "-true") != 0 ){
				tflag = 1;
				t_fname = argv[i + 1];
			}
		}
		else if(strcmp("-true", argv[i]) == 0) {
			cmtrueflag = 1;
			compflag = 1;
		}
		else if(strcmp("-false", argv[i]) == 0) {
			cmfalseflag = 1;
			compflag = 0;
		}
	}

	if(inflag == 0 || (tflag == 1 && outflag == 1) || (tflag == 0 && outflag == 0) 
	|| argc > 6 || (cmtrueflag == 1 && cmfalseflag == 1) || (cmtrueflag == 0 && cmfalseflag == 0)) {
		printf("Wrong command!!!\nPlease enter your command like this \"./concube -i inputfile -o outputfile -true\" or \"./concube -i inputfile -t format -false\".\nYou can enter -true or -false but not two of them.\nYou can change arguments places as well.\n");
	} else {
		//only format is given
		if(tflag == 1) {			
			if(strcmp(get_extension(input_fname),".xml") == 0 && strcmp(t_fname,"csv") == 0 && compflag == 0){
				char *out = "output.csv";
				convertXmltoCsv(input_fname, out);
			}
			else if(strcmp(get_extension(input_fname),".xml") == 0 && strcmp(t_fname,"json") == 0 && compflag == 0){
				char *out = "output.json";
				convertXmltoJson(input_fname, out);	
			}
			else if(strcmp(get_extension(input_fname),".json") == 0 && strcmp(t_fname,"csv") == 0 && compflag == 0){
				char *out = "output.csv";
				convertJsontoCsv(input_fname, out);		
			}
			else if(strcmp(get_extension(input_fname),".json") == 0 && strcmp(t_fname,"xml") == 0 ){
				char *out = "output.xml";
				convertJsontoXml(input_fname, out, compflag);
			} 
			else if(strcmp(get_extension(input_fname),".csv") == 0 && strcmp(t_fname,"xml") == 0 ){
				char *out = "output.xml";
				convertCsvtoXml(input_fname, out, compflag);
			}
			else if(strcmp(get_extension(input_fname),".csv") == 0 && strcmp(t_fname,"json") == 0 && compflag == 0){
				char *out = "output.json";
				convertCsvtoJson(input_fname, out);			
			}
			else{
				printf("Wrong command!!!\nPlease enter your command like this \"./concube -i inputfile -o outputfile -true\" or \"./concube -i inputfile -t format -false\".\nYou can enter -true or -false but not two of them.\nYou can change arguments places as well.\n");
			}
		}
			//only output is given
		else if(outflag == 1) {
			if(strcmp(get_extension(input_fname),".xml") == 0 && strcmp(get_extension(output_fname),".csv") == 0 && compflag == 0){
				convertXmltoCsv(input_fname, output_fname);
			}
			else if(strcmp(get_extension(input_fname),".xml") == 0 && strcmp(get_extension(output_fname),".json") == 0 && compflag == 0){
				convertXmltoJson(input_fname, output_fname);
			}
			else if(strcmp(get_extension(input_fname),".json") == 0 && strcmp(get_extension(output_fname),".csv") == 0 && compflag == 0){
				convertJsontoCsv(input_fname, output_fname);	
			}
			else if(strcmp(get_extension(input_fname),".json") == 0 && strcmp(get_extension(output_fname),".xml") == 0){
				convertJsontoXml(input_fname, output_fname, compflag);	
			} 
			else if(strcmp(get_extension(input_fname),".csv") == 0 && strcmp(get_extension(output_fname),".xml") == 0){
				convertCsvtoXml(input_fname, output_fname, compflag);
			}
			else if(strcmp(get_extension(input_fname),".csv") == 0 && strcmp(get_extension(output_fname),".json") == 0 && compflag == 0){
				convertCsvtoJson(input_fname, output_fname);	
			}
			else{
				printf("Wrong command!!!\nPlease enter your command like this \"./concube -i inputfile -o outputfile -true\" or \"./concube -i inputfile -t format -false\".\nYou can enter -true or -false but not two of them.\nYou can change arguments places as well.\n");
			}
		}
	}
	return 0;
}

const char *get_extension(const char *fs){
	const char *ext = strrchr(fs, '.');
	if(fs == NULL){
		ext = "";
	}
	return ext;
}
