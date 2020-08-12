/* 
 * Copyright (c) 2018 Valentin Cassano.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <llist.h>

#include <dictionary.h>

#include <ctype.h>

struct word_s
{
	char * expression;
	char * definition;
};

struct dictionary_s
{
	 char * name;
	llist_t content;
	    int entries;
};

// assumption: line = "\" ... \", \" ... \"\n"
word_t line2word( char * line )
{
	word_t word = ( word_t ) malloc ( sizeof( struct word_s ) );

	word->expression = strtok(line, ",");
	word->definition = strtok(NULL, "\n");
	
	return word;
}

// parse the buffer line by line converting the lines
// read into the words to be inserted in the dictionary
llist_t parse( char * buffer )
{
	llist_t content = make( );
	
	char * c = buffer;
	while(c){
		char * nextLine = strchr(c, '\n');
		if (nextLine) *nextLine = '\0';
		content = add(content, line2word(c));
 		c = nextLine ? (nextLine+1) : NULL;
	}
			
	return content;
}

dictionary_t load( char * file_name )
{
	dictionary_t dictionary = (dictionary_t) malloc (sizeof(struct dictionary_s));
	dictionary->name = file_name ;

	long int longitud = 0;
	FILE* f;
	f = fopen(file_name, "r");

	if(f == NULL){
		printf("Error al abrir el archivo");
	}
	else{
		fseek(f, 0, SEEK_END);
		longitud = ftell(f);
		rewind(f);
		char * buffer = (char *) malloc (sizeof(char)*longitud);

		if(buffer){
			fread (buffer, 1, longitud, f);
		}
		fclose(f);
		dictionary->content = parse(buffer);
		
		dictionary->entries = length(dictionary->content);
		
	}

	return dictionary;

}

word_t random_word( dictionary_t dictionary )
{
    srand(time(NULL));
    return at( dictionary->content, rand( ) % dictionary->entries );
}

char * expression( word_t word )
{
	int j = 0;

	char * result = (char *) malloc (sizeof(char)*strlen(word->expression));
	
	strcpy(result, word->expression);

	for (int i = 0; result[i] != '\0'; ++i)
	{
		result[i] = result [i+1];
		j = i;
	}

	result[j-1] = ' ';

	char * tilde = (char *) malloc (sizeof(char));
	char * sin = (char *) malloc (sizeof(char));

	strcpy(tilde, "áéíóú");
	strcpy(sin,   "aeiou");

	for (int j = 0; result[j] != '\0' ; j++)
	{
		for (int i = 0; tilde[i] != '\0'; i++)
		{
			if (result[j] == tilde[i])
			{
				result[j] = sin[i];
			}
		}
	}

	for (int i = 0; result[i]!= '\0'; i++){
		{
			result[i] = tolower(result[i]);
		}
	}


	
	return result;
}

char * definition( word_t word )
{
	char * result = (char *) malloc (sizeof(char)*strlen(word->definition));
	
	strcpy(result, word->definition);
	
	result[0] = ' ';

	result[strlen(word->definition)-1] = ' ';
	
	return result;
}

void print_word( word_t word )
{
	printf( "  %s:", word->expression );
	printf( "\n  %s ", word->definition );
}
