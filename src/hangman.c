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

#include <input.h>
#include <dictionary.h>

#include <hangman.h>

#include <ctype.h>

#define ESC 27

#define GAME_OVER 7 // max number of misses
#define PENALTY   2 // penalty for incorrect guess

// state of hangman
typedef	struct state_s* state_t;

struct word_s
{
	char * expression;
	char * definition;
};

struct state_s
{
	char * help;
	char * password;
	char * secret;
	   int length;
	char * misses;
	   int last;
	   int missed;
};

static void welcome( void )
{
	printf( "\n             **********************************          " );
	printf( "\n             *             HANGMAN            *          " );
	printf( "\n             **********************************          " );
	printf( "\n                                                            " );
	printf( "\n      ( press ESC to exit or any other key to continue ) " );
}

void draw_hangman( state_t game )
{
	switch ( game->missed )
	{
		case 0 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/           " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;
		
		case 1 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;
		
		case 2 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;
		
		case 3 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |      |     " );
			printf( "\n |      |     " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;
		
		case 4 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |   >--|     " );
			printf( "\n |      |     " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;
		
		case 5 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |   >--|--<  " );
			printf( "\n |      |     " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;
		
		case 6 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |   >--|--<  " );
			printf( "\n |      |     " );
			printf( "\n |     /      " );
			printf( "\n |   _/       " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;
		
		default :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |   >--|--<  " );
			printf( "\n |      |     " );
			printf( "\n |     / \\    " );
			printf( "\n |   _/   \\_  " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;
	}
}

void display( state_t game )
{
	draw_hangman( game );
	
	printf( "\n                          " );
	printf( "\n   help: %s", game->help   );
	printf( "\n   secret: %s", game->secret );
	printf( "\n   misses: %s", game->misses );
	printf( "\n   cant or misses: %d", game->missed );
	printf( "\n\n   Press 1 for to complet guess or ESC for exit: " );
	printf( "\n\n   Key: " );
}

// new game
state_t init( dictionary_t dictionary )
{
	state_t game = ( state_t ) malloc ( sizeof( struct state_s ) );
 
	word_t aux = ( word_t ) malloc ( sizeof( struct word_s ) );
	
	aux = random_word(dictionary);
	
	game->password = expression(aux);
	
	game->help = definition(aux);

	game->length = strlen(game->password);

	game->missed = 0;

	game->misses = (char *) malloc (sizeof(char));

	game->secret = (char *) malloc (sizeof(char)*game->length);

	memset(game->secret, '*', game->length-1);

	game->last = 0;

	return game;
}

void draw_winner(){

	printf(" \n                                                                                           " );
	printf( "\n        *************************************************************************          " );
	printf( "\n                                                                                           " );
	printf( "\n        *             (⌐■_■) CONGRATULATIONS! YOUR THE WINNER (⌐■_■)            *          " );
	printf( "\n                                                                                           " );
	printf( "\n        *************************************************************************          " );
	printf( "\n                                                                                           " );
	
}

int is_secret(state_t game){
	for(int i = 0; game->secret[i] != '\0'; i++){
		if (game->secret[i] == '*')
		{
			return 1;
		}
	}
	return 0;
}

int adivination(state_t game){

	char * aux = (char *) malloc (sizeof(char)*game->length);

	printf("\n\n   Your guess: ");

	aux = getstr( game->length - 1);

	for (int i = 0; i < game->length - 1; ++i)
	{
		if (game->password[i] != aux[i])
		{
			return 0;
		}
	}
	return 1;
}

void misses(state_t game, char guess)
{
	int cant = 0;
	for(int i= 0; game->misses[i] != '\0'; i++)
	{
		cant++;
	}
	game->misses[cant] = guess;
	game->misses[cant + 1] = ' ';
	game->misses[cant + 2] = '\0';
	game->missed = game->missed + 1;
}

void add_guess(state_t game, char guess){
	for(int i = 0; i < game->length; i++)
	{
		if(guess == game->password[i])
		{
			game->secret[i] = guess;
		}
	}
}

// update current game
void update( state_t game, char guess )
{	
	if(is_secret(game) == 0)
	{
		game->last = 1;
	}
	else
	{
		if(guess == '1')
		{
			if (adivination(game) == 1)
			{
				game->last = 1;
				
			}
			else
			{
				game->missed = GAME_OVER + 1;
			}
		}
		else
		{
			if(strchr(game->password, guess) == NULL)
			{
				misses(game, guess);
			}
			else
			{
				add_guess(game, guess);
			}
		}
	}
}

void draw_lose(state_t game){

	printf( "\n\n        .·´¯`(>▂<)´¯`·. YOU HAVE LOST! .·´¯`(>▂<)´¯`·.\n" );
	printf( "\n\n        The secret is: %s \n", game->password);
	printf( "\n            (ง •̀_•́)ง     Play again?       (ง •̀_•́)ง " );
	printf( "\n\n                Press S or any key for exit\n");

}

int play( dictionary_t dictionary )
{
	char guess;

	state_t game;

	game = init(dictionary);

	welcome( );

	guess = getch();

	if (guess == ESC){
		return -1;
	}

	while(game->missed <= GAME_OVER && guess != ESC && game->last != 1){
		
		system("clear");
		
		display(game);
		
		guess = getch();
		guess = tolower(guess);

		update(game, guess);
	}
	
	if (game->last == 1)
	{
		draw_winner();
	}
	else
	{
		draw_lose(game);
		
		guess = getch();
		
		if (guess == 'S' || guess == 's')
		{
			play(dictionary);
		}
	}
	
	return 0;
}
