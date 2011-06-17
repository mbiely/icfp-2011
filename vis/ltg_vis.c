
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <alloca.h>

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>



#define	MIN(a,b)	(((a) < (b)) ? (a) : (b))
#define	MAX(a,b)	(((a) > (b)) ? (a) : (b))

#define	CLAMP(v,a,b)	MIN(b, MAX(a,v))


#define	VID_WIDTH	1100
#define	VID_HEIGHT	600

#define SLOT_WIDTH	32
#define	SLOT_HEIGHT	32

#define	CARD_HEIGHT	40

#define	PLAY_WIDTH	((SLOT_WIDTH * 16) + 1)
#define	PLAY_HEIGHT	((SLOT_HEIGHT * 16) + 1)

#define	FONT_HEIGHT	12


typedef	struct _slot {
	int64_t field;
	int64_t vitality;
	char *seq;
}	slot_t;

slot_t	player[2][256];


bool	is_slot_dead(slot_t *slot)
{
	if (slot->vitality == 0)
		return true;
	if (slot->vitality == -1)
		return true;
	return false;
}

bool	is_slot_integer(slot_t *slot)
{
	if (slot->field >= 0)
		return true;
	return false;
}

bool	is_slot_index(slot_t *slot)
{
	if ((slot->field >= 0) &&
	    (slot->field < 256))
		return true;
	return false;
}


typedef	struct _card {
	int id;
	char *name;
}	card_t;

card_t	cards[] = {
	{ .id =  -1, .name = "I" },
	{ .id =  -2, .name = "zero" },
	{ .id =  -3, .name = "succ" },
	{ .id =  -4, .name = "dbl" },
	{ .id =  -5, .name = "get" },
	{ .id =  -6, .name = "put" },
	{ .id =  -7, .name = "S" },
	{ .id =  -8, .name = "K" },
	{ .id =  -9, .name = "inc" },
	{ .id = -10, .name = "dec" },
	{ .id = -11, .name = "attack" },
	{ .id = -12, .name = "help" },
	{ .id = -13, .name = "copy" },
	{ .id = -14, .name = "revive" },
	{ .id = -15, .name = "zombie" },
	{ .id = 0 }};


char *	card_name_from_id(int id)
{
	for (int i = 0; i < sizeof(cards)/sizeof(card_t); i++)
		if (cards[i].id == id)
			return cards[i].name;
	return NULL;
}

int	card_id_for_name(char *name)
{
	for (int i = 0; i < sizeof(cards)/sizeof(card_t); i++)
		if (strcmp(cards[i].name, name) == 0)
			return cards[i].id;
	return 0;
}



void	vis_init_slots(slot_t slot[256])
{
	for (int i=0; i<256; i++) {
		slot[i].field = -1;
		slot[i].vitality = 10000;
	}
}


bool	vis_apply_card_to_slot(int play_id, int card_id, int slot_id)
{
	switch (card_id) {
	case -1:	/* I x */
		break;
	
	case -2:	/* zero */
		break;
	
	case -3:	/* succ n*/
		break;
	
	case -4:	/* dbl n */
		break;
	
	case -5:	/* get i */
		break;
	
	case -6:	/* put x y */
		break;
	case -7:	/* S f g x */
		break;
	case -8:	/* K x y */
		break;
	case -9:	/* inc i */
		break;
	case -10:	/* dec i */
		break;
	case -11:	/* attack i j n */
		break;
	case -12:	/* help i j n */
		break;
	case -13:	/* copy i */
		break;
	case -14:	/* revive i */
		break;
	case -15:	/* zombie i x */
		break;
	default:
		/* error case */
		goto out_err;
	}
	return true;

out_err:
	return false;
}

bool	vis_apply_slot_to_card(int play_id, int slot_id, int card_id)
{
	switch (card_id) {
	case -1:	/* I x */
		break;
	
	case -2:	/* zero */
		break;
	
	case -3:	/* succ n*/
		break;
	
	case -4:	/* dbl n */
		break;
	
	case -5:	/* get i */
		break;
	
	case -6:	/* put x y */
		break;
	case -7:	/* S f g x */
		break;
	case -8:	/* K x y */
		break;
	case -9:	/* inc i */
		break;
	case -10:	/* dec i */
		break;
	case -11:	/* attack i j n */
		break;
	case -12:	/* help i j n */
		break;
	case -13:	/* copy i */
		break;
	case -14:	/* revive i */
		break;
	case -15:	/* zombie i x */
		break;
	default:
		/* error case */
		goto out_err;
	}
	return true;

out_err:
	return false;
}




TTF_Font *fnt = NULL;



void	vis_draw_grid(SDL_Surface *dst, unsigned slotw, unsigned sloth)
{
	for (int x = 0; x < 17; x++)
		vlineColor(dst, x * SLOT_WIDTH, 0, PLAY_HEIGHT, 0x8F8F8FFF);
	for (int y = 0; y < 17; y++)
		hlineColor(dst, 0, PLAY_WIDTH, y * SLOT_HEIGHT, 0x8F8F8FFF);
}

void	vis_draw_card_grid(SDL_Surface *dst, unsigned slotw, unsigned sloth)
{
	for (int x = 0; x < 17; x++)
		vlineColor(dst, x * SLOT_WIDTH, 0, CARD_HEIGHT, 0x8F8F8FFF);
	for (int y = 0; y < 2; y++)
		hlineColor(dst, 0, PLAY_WIDTH, y * CARD_HEIGHT, 0x8F8F8FFF);
}


void	vis_draw_string(SDL_Surface *dst, unsigned xp, unsigned yp, char *str, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect dstRect = {xp, yp, 0, 0};
	SDL_Color fg = { .r = r, .g = g, .b = b };
#if 0
	SDL_Color bg = { .r = 0, .g = 0, .b = 0 };
	
	SDL_Surface *sText = TTF_RenderText_Shaded(fnt, str, fg, bg);
#else
	SDL_Surface *sText = TTF_RenderText_Solid(fnt, str, fg);
#endif
	SDL_BlitSurface(sText, NULL, dst, &dstRect);
	SDL_FreeSurface(sText);
}


void	vis_draw_slot(SDL_Surface *dst, unsigned x, unsigned y, slot_t slot)
{
	unsigned xp = x * SLOT_WIDTH + 1;
	unsigned yp = y * SLOT_HEIGHT + 1;

	unsigned r = (slot.field > 0) ? slot.field / 256 : 0;
	unsigned g = (slot.vitality > 0) ? slot.vitality * 255 / 10000 : 0;
	unsigned b = (slot.field < 0) ? -slot.field * 16 : 0;

	boxRGBA(dst, xp, yp, xp + SLOT_WIDTH - 2, yp + SLOT_HEIGHT - 2,
		r, g, b, 255);

	char line[2][64] =  { "", "" };

	if (slot.field >= 0)
		sprintf(line[0], "%5ld", slot.field);
	if (slot.field < 0)
		sprintf(line[0], "%s", card_name_from_id(slot.field));

	sprintf(line[1], "%5ld", slot.vitality);

	for (int i=0; i<2; i++)
		vis_draw_string(dst, xp, yp + i * FONT_HEIGHT, line[i], 255, 255, 255);
}

void	vis_draw_slots(SDL_Surface *dst, slot_t slot[256])
{
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			vis_draw_slot(dst, x, y, slot[x + y*16]);
		}
	}
}

void	vis_draw_card(SDL_Surface *dst, unsigned x, card_t card)
{
	unsigned xp = x * SLOT_WIDTH + 1;
	unsigned yp = 1;

	unsigned r = 64;
	unsigned g = x * 8;
	unsigned b = 0;
	boxRGBA(dst, xp, yp, xp + SLOT_WIDTH - 2, yp + CARD_HEIGHT - 2,
		r, g, b, 255);

	char line[2][64] =  { "", "" };

	if (card.name)
		sprintf(line[0], "%s", card.name);

	for (int i=0; i<2; i++)
		vis_draw_string(dst, xp, yp + i * FONT_HEIGHT, line[i], 255, 255, 255);
}

void	vis_draw_cards(SDL_Surface *dst)
{
	for (int i = 0; i < sizeof(cards)/sizeof(card_t); i++) {
		vis_draw_card(dst, i, cards[i]);
	}
}



uint64_t turn = 0;
int play_id = 0;
int card_id = 0;
int slot_id = 0;
int vitality = 0;




void	parse_turn_info(char *line)
{
	sscanf(line, "###### turn %ld", &turn);
	printf("turn %ld\n", turn);  
}

void	parse_player_info(char *line)
{
	char card_name[64];
	int n;

	n = sscanf(line, "player %d applied card %s to slot %d",
		&play_id, card_name, &slot_id);
	if (n == 3) {
		printf("%d: %s -> %d [%d]\n", play_id, card_name, slot_id, n);  
		return;
	}

	n = sscanf(line, "player %d applied slot %d to card %s",
		&play_id, &slot_id, card_name);
	if (n == 3) {
		printf("%d: %d -> %s [%d]\n", play_id, slot_id, card_name, n);  
		return;
	}
}

void	parse_player_turn(char *line)
{
	sscanf(line, "*** player %d'", &play_id);
	printf("player %d\n", play_id);  
}

void	parse_slot(char *line)
{
	int n = sscanf(line, "%d={%d,", &slot_id, &vitality);
	if (n == 2) {
		printf("slot %d: %d\n", slot_id, vitality);  
	}
	return;	
}

void	parse_expr(char *line)
{
	/* (slots ... ) and (1) ignored for now */
	return;	
}


enum	_state {
	ST_INIT	= 0,
	ST_SCAN	= 1,
	ST_TURN = 2,
};


void	parse_input(char *line)
{
	static char state = ST_INIT;
	
	switch (line[0]) {
	case 'L':	/* Lambda */
		state = ST_SCAN;
	
	case '#':	/* turn info */
		parse_turn_info(line);
		break;
		
	case 'p':	/* player info */
		parse_player_info(line);
		break;

	case '*':	/* players turn */
		parse_player_turn(line);
		state = ST_TURN;
		break;

	case '(':	/* refine */
		parse_expr(line);
		state = ST_SCAN;
		break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		parse_slot(line);
		break;

	case 's':	/* slot ignored */
	case 'c':	/* card ignored */
	default:	/* ignored */
		break;

	}
}


int	main(int argc, char *argv[])
{

	SDL_Surface *screen;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr,
			"Unable to initialize SDL: %s\n",
			SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);


	screen = SDL_SetVideoMode(VID_WIDTH, VID_HEIGHT, 16,
		SDL_DOUBLEBUF | SDL_SWSURFACE);

	SDL_Surface *play0, *play1;
	play0 = SDL_CreateRGBSurface(SDL_SWSURFACE,
		PLAY_WIDTH, PLAY_HEIGHT, 32, 0, 0, 0, 0);
	play1 = SDL_CreateRGBSurface(SDL_SWSURFACE,
		PLAY_WIDTH, PLAY_HEIGHT, 32, 0, 0, 0, 0);

	SDL_Surface *card0, *card1;
	card0 = SDL_CreateRGBSurface(SDL_SWSURFACE,
		PLAY_WIDTH, CARD_HEIGHT + 1, 32, 0, 0, 0, 0);
	card1 = SDL_CreateRGBSurface(SDL_SWSURFACE,
		PLAY_WIDTH, CARD_HEIGHT + 1, 32, 0, 0, 0, 0);

	SDL_WM_SetCaption("LTG Sim", "LTG Sim");

	if (TTF_Init() != 0) {
		fprintf(stderr,
			"Unable to initialize TTF: %s\n",
			SDL_GetError());
		exit(2);
	}

	fnt = TTF_OpenFont("/usr/share/fonts/TTF/Vera.ttf", 8);
	if (!fnt) {
		fprintf(stderr,
			"Unable to open font: %s\n",
			SDL_GetError());
		exit(3);
	}


	vis_init_slots(player[0]);
	vis_init_slots(player[1]);

	vis_draw_grid(play0, SLOT_WIDTH, SLOT_HEIGHT);
	vis_draw_grid(play1, SLOT_WIDTH, SLOT_HEIGHT);

	vis_draw_slots(play0, player[0]);
	vis_draw_slots(play1, player[1]);

	vis_draw_card_grid(card0, SLOT_WIDTH, CARD_HEIGHT);
	vis_draw_card_grid(card1, SLOT_WIDTH, CARD_HEIGHT);

	vis_draw_cards(card0);
	vis_draw_cards(card1);


	unsigned frame = 0;
	char line[4096];	/* fixme */

	while (1) {
		SDL_Rect psrcRect = {0, 0, PLAY_WIDTH, PLAY_HEIGHT };
		SDL_Rect play0Rect = {10, 10, PLAY_WIDTH, PLAY_HEIGHT };
		SDL_Rect play1Rect = {VID_WIDTH - 10 - PLAY_WIDTH, 10, PLAY_WIDTH, PLAY_HEIGHT };

		SDL_BlitSurface(play0, &psrcRect, screen, &play0Rect);
		SDL_BlitSurface(play1, &psrcRect, screen, &play1Rect);

		SDL_Rect csrcRect = {0, 0, PLAY_WIDTH, CARD_HEIGHT + 1 };
		SDL_Rect card0Rect = {10, 20 + PLAY_HEIGHT, PLAY_WIDTH, CARD_HEIGHT + 1 };
		SDL_Rect card1Rect = {VID_WIDTH - 10 - PLAY_WIDTH, 20 + PLAY_HEIGHT, PLAY_WIDTH, CARD_HEIGHT + 1 };

		SDL_BlitSurface(card0, &csrcRect, screen, &card0Rect);
		SDL_BlitSurface(card1, &csrcRect, screen, &card1Rect);

		SDL_Flip(screen); //Refresh the screen

		fgets(line, sizeof(line), stdin);
		parse_input(line);


		SDL_Event event; /* Event structure */
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				break;
		}

		frame++;
	}

	exit(0);
}
