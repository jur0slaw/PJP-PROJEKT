#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <math.h>

#define screen_width 1920
#define screen_height 1080
#define step 0.2
#define dash 500
#define ratio 10000
#define radius 400

int flag;


double i = 0;
double j = 300;
 

double trigger = 0;
int ruch = 0;
double cooldown = 200;
double speed_x,speed_y;
double obecna_pozycja_x;
double obecna_pozycja_y;
double dystans = 0;
double max;
int przeciwnik = 0;
double pozx=0;
double pozy=0;
double kamien_speed_x;
double kamien_speed_y;
int ciecie=0;
double cut_start_x;
double cut_start_y;
double cut_end_x;
double cut_end_y;
double odleglosc;


void DoLogic(ALLEGRO_KEYBOARD_STATE stan, ALLEGRO_TIMER * timer , ALLEGRO_MOUSE_STATE mysz){


	speed_x = (i - obecna_pozycja_x) / ALLEGRO_BPS_TO_SECS(60);
	speed_y = (j - obecna_pozycja_y) / ALLEGRO_BPS_TO_SECS(60);

	obecna_pozycja_x = i;
	obecna_pozycja_y = j;
	


	if (ruch == 1) {
		switch (flag) {
		case 11: 
			i += 4*step;
			dystans += 4 * step;
			if (dystans >= max) {
				dystans = 0;
				ruch = 0;
				flag = 0;
				break;

			}
			break;
		case 12:
			i -= 4 * step;
			dystans += 4 * step;
			if (dystans >= max) {
				dystans = 0;
				ruch = 0;
				flag = 0;
				break;

			}
			break;
		}

	}

	

	//Ciecie

	if (mysz.buttons == 1  && ciecie == 0 && pow(mysz.x-i-50,2)+pow(mysz.y-j-50,2)<pow(radius,2)){
		ciecie =  1;
		cut_start_x = mysz.x;
		cut_start_y = mysz.y;
	}

	if ((!mysz.buttons == 1|| !(pow(mysz.x - i-50, 2) + pow(mysz.y - j - 50, 2)<pow(radius, 2))) && ciecie == 1) {
		ciecie = 0;
		cut_end_x = mysz.x;
		cut_end_y = mysz.y;
		if (((pozx > cut_start_x-60 && pozx < cut_end_x+60) || (pozx < cut_start_x+60 && pozx > cut_end_x-60)) && ((pozy > cut_start_y-60 && pozy < cut_end_y+60) || (pozy < cut_start_y+60 && pozy > cut_end_y-60))) {
			double A = cut_start_x - cut_end_x, B = cut_end_y - cut_start_y, 
			odleglosc = abs((A*(cut_start_x - pozx) + B*(pozy - cut_start_y)) / sqrt(A*A + B*B));
			if (odleglosc < 120) {
				przeciwnik = 0;
			}
		}
	}

	//KAMIEÑ
	if (przeciwnik == 1) {
		pozx += kamien_speed_x;
		pozy += kamien_speed_y;
		if (pozx < 0 || pozx > screen_width || pozy < 0 || pozy > screen_height) {
			przeciwnik = 0;
		}
	}

	if (al_key_down(&stan, ALLEGRO_KEY_SPACE) && przeciwnik==0){
		przeciwnik = 1;
		kamien_speed_x = (i - pozx)/ratio;
		kamien_speed_y = (j - pozy)/ratio;


	}


	

	//Flagi i Triggery dashowania i ruchu

	if (trigger > 0 && flag == 7 && ruch == 0 && al_key_down(&stan, ALLEGRO_KEY_D)) {
		flag += 4;
		ruch = 1;
	}

	if (trigger > 0 && flag == 8 && ruch == 0 && al_key_down(&stan, ALLEGRO_KEY_A)) {
		flag += 4;
		ruch = 1;
	}

	if (al_key_down(&stan, ALLEGRO_KEY_W) && j>=0 && ruch == 0) {
		j -= 1.5*step;
		//flag = 1;
		//trigger = 500.0;
	}


	if (al_key_down(&stan, ALLEGRO_KEY_S) && j <= screen_height-100 && ruch == 0) {
		j += 1.5*step;
		//flag = 2;
		//trigger = 500.0;
	}


	if (al_key_down(&stan, ALLEGRO_KEY_D) && i <= screen_width-100 && ruch == 0) {
		
		i += step ;
		flag = 3;
		trigger = 500.0;
	}


	if (al_key_down(&stan, ALLEGRO_KEY_A) && i >= 0 && ruch == 0) {
		i -= step ;
		flag = 4;
		trigger = 500.0;
	}

	//Obliczanie dasha

	if (trigger > 0 && trigger < 400 && flag == 3 && ruch == 0 && !al_key_down(&stan, ALLEGRO_KEY_D)) {
		flag += 4;
		if (((screen_width  - 100 ) - i) <= dash) {
			max = ((screen_width - 100) - i);
		}
		else {
			max = dash;
		}
	}

	if (trigger > 0 && trigger < 400 && flag == 4 && ruch == 0 && !al_key_down(&stan, ALLEGRO_KEY_A)) {
		flag += 4;
		if (i <= dash) {
			max = i;
		}
		else {
			max = dash;
		}
	}

	//Zerowanie flag i zmienjszanie triggera

	if (trigger > 0 && ruch == 0 ) {
		trigger -= 0.8;
		
	}
	if (trigger <= 0) {
		flag = 0;
	 }
}


int main(void) {
	ALLEGRO_DISPLAY *okno = NULL;





	if (!al_init()) {
		al_show_native_message_box(NULL, NULL, NULL, "ALLEGRO INIC FAIL", NULL, NULL);
	}


	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();

	if (!al_install_mouse()) {
		al_show_native_message_box(NULL, NULL, NULL, "ALLEGRO MOUSE FAIL", NULL, NULL);
	}

	okno = al_create_display(screen_width, screen_height);
	if (!okno) {
		al_show_native_message_box(NULL, NULL, NULL, "ALLEGRO INIC FAIL", NULL, NULL);

	}
	

	ALLEGRO_FONT *comic = al_load_ttf_font("comic.ttf", 40, NULL);
	ALLEGRO_BITMAP *kappa = al_load_bitmap("kappa.jpg");
	ALLEGRO_BITMAP *kamien = al_load_bitmap("rock.png");
	ALLEGRO_KEYBOARD_STATE stan;
	ALLEGRO_MOUSE_STATE mysz;
	ALLEGRO_TIMER *timer = al_create_timer(ALLEGRO_BPS_TO_SECS(60));
	al_convert_mask_to_alpha(kappa, al_map_rgb(255, 255, 255));
	al_convert_mask_to_alpha(kamien, al_map_rgb(255, 255, 255));





	

	al_flip_display();
 
	
	

	al_get_keyboard_state(&stan);
	al_start_timer(timer);
	al_get_mouse_state(&mysz);

	double lag = 0;
	double previous = al_get_timer_count(timer);

	
	while (!al_key_down(&stan, ALLEGRO_KEY_ESCAPE)) {
		double start = al_get_timer_count(timer);
		double elapsed = start - previous;
		previous = start;
		lag += elapsed;

		al_get_keyboard_state(&stan);
		al_get_mouse_state(&mysz);
	
		while(lag >= ALLEGRO_BPS_TO_SECS(60)){
			DoLogic(stan,timer,mysz);
			lag -= ALLEGRO_BPS_TO_SECS(60);
		}


		al_draw_textf(comic, al_map_rgb(255, 0, 0), 0, 0, ALLEGRO_ALIGN_LEFT, "Timer %i", al_get_timer_count(timer));
		al_draw_textf(comic, al_map_rgb(255, 0, 0), 0, 1000, ALLEGRO_ALIGN_LEFT, "Trigger %f",trigger);
		al_draw_textf(comic, al_map_rgb(255, 0, 0), 500, 1000, ALLEGRO_ALIGN_LEFT, "Speed X %f", speed_x);
		al_draw_textf(comic, al_map_rgb(255, 0, 0), 900, 1000, ALLEGRO_ALIGN_LEFT, "Speed Y %f ", speed_y);
		al_draw_textf(comic, al_map_rgb(255, 0, 0), 1500, 1000, ALLEGRO_ALIGN_LEFT, "odleglosc %f", odleglosc);

		al_draw_filled_rectangle(0, screen_height/10, screen_width, screen_height - screen_height / 10, al_map_rgb(255, 255, 255));
		
		al_draw_bitmap(kappa, i , j, NULL);
		al_draw_circle(i + 50, j + 50, radius, al_map_rgb(100, 100, 100), 5);

		if (ciecie == 1) {
			al_draw_line(cut_start_x, cut_start_y, mysz.x, mysz.y, al_map_rgb((al_get_timer_count(timer)+100)%255, al_get_timer_count(timer) % 255, al_get_timer_count(timer) % 255),10);
		}

		if (przeciwnik == 1) {
			al_draw_scaled_rotated_bitmap(kamien, 200, 200, pozx, pozy, 0.25, 0.25, pozx / 60 , NULL);
		}
		al_flip_display();

		al_clear_to_color(al_map_rgb((int)(i*j/(j+i)*0.1) % 255, (int)(j*0.05) % 255, (int)(i *0.05) %255));



	}

	al_uninstall_keyboard;
	al_destroy_display(okno);

	al_destroy_font(comic);



	return 0;
}

