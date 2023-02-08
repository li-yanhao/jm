#include "xmltracefile.h"

#include <string.h>

#define STACK_SIZE 1024

char bClosed = '0';
char filename[255];
char file_frameorder[255];
FILE* fh = NULL;
FILE* fo = NULL;

char tags[STACK_SIZE];
int stackcount = 0;
char bPrevIsElement = '0';
int displaynr = -1;
int loglevel = 4;

int write_to_file(char* text);

void xml_set_trace_filename(char* location)
{
	if(strlen(location) > 0 && (strstr(location, ".xml") != NULL))
	{
		strcpy(filename, location);
		strncpy(file_frameorder, location, strlen(location)-4);
		strcat(file_frameorder, "_displayorder.xml");
	}
	else
	{
		strcpy(filename, "\0");
		strcpy(file_frameorder, "\0");
	}
}

char* xml_get_trace_filename()
{
	return filename;
}

void xml_set_log_level(int level)
{
	loglevel = level;
}

int xml_get_log_level()
{
	return loglevel;
}


int xml_open_trace_file()
{
	if(filename == NULL) return -2;

	if(fh != NULL) xml_close_trace_file();

	fh = fopen(filename, "w");

	if(fh == NULL) return -1;
	//Write the XML declaration
	fputs("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n", fh);

	bClosed = '1';

	if(filename == NULL) return -4;
	fo = fopen(file_frameorder, "w");
	if(fo == NULL) return -3;

	//Write the XML declaration
	fputs("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n", fo);
	//Write parent node
	fputs("<AVCTraceDisplayOrder tracefile=\"", fo);
	fputs(filename, fo);
	fputs("\">\n", fo);

	return 0;
}

void xml_close_trace_file()
{
	if(fh != NULL) fclose(fh);
	if(fo != NULL)
	{
		//Write closing tag
		fputs("</AVCTraceDisplayOrder>\n", fo);
		fclose(fo);
	}
}

int xml_gen_trace_file()
{
	if(strcmp(xml_get_trace_filename(),"\0") != 0)
		return 1;
	return 0;
}

void xml_close_tag()
{
	if(bClosed == '0')
	{
		write_to_file(">");
		bClosed = '1';
	}
}

void xml_write_start_element(char* name)
{
	char str[STACK_SIZE];
	int i;

	xml_close_tag();

	if(bPrevIsElement == '1') write_to_file("\n");

	for(i = 0; i < stackcount; i++)
		write_to_file("\t");

	write_to_file("<");
	write_to_file(name);
	bClosed = '0';

	//Add element to stack
	strcpy(str, tags);
	strcpy(tags, name);
	strcat(tags, ";");
	strcat(tags, str);
	stackcount++;

	bPrevIsElement = '1';
}

void xml_write_end_element()
{
	char* pch;
	char first = '0';
	char str[STACK_SIZE];
	int i;

	xml_close_tag();

	if(bPrevIsElement == '1')
	{
		write_to_file("\n");
		for(i = 1; i < stackcount; i++)
			write_to_file("\t");
	}
	write_to_file("</");
	//Get the last tag from the stack
	pch = strtok(tags, ";");
	if(pch != NULL) write_to_file(pch);
	//Write rest of the stack content
	first = '1';
	pch = strtok(NULL, ";");
	while(pch != NULL)
	{
		if(first == '1')
		{
			strcpy(str, pch);
			first = '0';
		}
		else
			strcat(str, pch);
		strcat(str, ";");
		pch = strtok(NULL, ";");
	}
	strcpy(tags, str);
	stackcount--;
	write_to_file(">");
	bPrevIsElement = '1';
}

void xml_write_string_attribute(char* name, char* value)
{
	write_to_file(" ");
	write_to_file(name);
	write_to_file("=\"");
	write_to_file(value);
	write_to_file("\"");
}

void xml_write_int_attribute(char* name, int value)
{
	char buffer[255];

	sprintf(buffer, "%i", value);

	xml_write_string_attribute(name, buffer);
}

void xml_write_text(char* value)
{
	xml_close_tag();
	write_to_file(value);

	bPrevIsElement = '0';
}

void xml_write_int(int value)
{
	char buffer[255];

	xml_close_tag();
	sprintf(buffer, "%i", value);
	write_to_file(buffer);

	bPrevIsElement = '0';
}

void xml_write_comment(char* comment)
{
	xml_close_tag();
	xml_write_text("<!-- ");
	xml_write_text(comment);
	xml_write_text(" -->");
}

int xml_check_and_write_end_element(char* name)
{
	int len = strlen(name);
	if (strncmp(tags, name, len) == 0)
	{
		xml_write_end_element();	//Close the tag
		return 1;
	}
	return 0;
}

int xml_output_frame_order(int id)
{
	int concealed = 0;
	char buffer[255];

	if(id == -1) concealed = 1;
	
	displaynr = displaynr + 1;
	
	sprintf(buffer, "\t<Picture displaynr=\"%i\" id=\"%i\" concealed=\"%i\"/>\n", displaynr, id, concealed);

	if(fo != NULL) return fputs(buffer, fo);

	return -1;
}

int write_to_file(char* text)
{
  if(fh != NULL) return fputs(text, fh);

	return -1;
}
