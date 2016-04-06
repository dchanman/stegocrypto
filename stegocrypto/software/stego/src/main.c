#include <stdint.h>
#include <stdio.h>

#include "main_controller.h"

/* Comment this #define out when we're done testing */
#define TESTING

#ifdef TESTING
#include "minigames.h"
#include "bitmap_display.h"
#include "test.h"
#include "project1_demo.h"
#include "sdcard.h"
#include "rgb.h"
#include "geoencryption_engine.h"
#endif

int main() {
	printf("StegoCrypto\n");
	/* ASCII Art credit: http://www.asciiworld.com/-Dinosaurs-.html */
	printf(
			"                                .\n"
			"                        / `.   .' \\\n"
			"                .---.  <    > <    >  .---.\n"
			"                |    \\  \\ - ~ ~ - /  /    |\n"
			"                 ~-..-~             ~-..-~\n"
			"             \\~~~\\.'                    `./~~~/\n"
			"              \\__/                        \\__/\n"
			"               /                  .-    .  \\\n"
			"        _._ _.-    .-~ ~-.       /       }   \\/~~~/\n"
			"    _.-'q  }~     /       }     {        ;    \\__/\n"
			"   {'__,  /      (       /      {       /      `. ,~~|   .     .\n"
			"    `''''='~~-.__(      /_      |      /- _      `..-'   \\\\   //\n"
			"                / \\   =/  ~~--~~{    ./|    ~-.     `-..__\\\\_//_.-'\n"
			"               {   \\  +\\         \\  =\\ (        ~ - . _ _ _..---~\n"
			"               |  | {   }         \\   \\_\\\n"
			"              '---.o___,'       .o___,'\n"
	);

#ifdef TESTING
	/* Uncomment the feature you want to test */
	//rs232_test_multi_char();
	//rs232_test_single_char();
	//sdcard_test_open();
	//sdcard_test();
	//gps_test();

	//savefile_test();
	//gps_test();
	//minecraft_rpc_test_main();
	//minecraft_rpc_test_protocol_msg();
	//bluetooth_test();
	//touchscreen_test();
	//touchscreen_test_timeout();
	//sprint1_demo();
	//graphics_test();
	//int health = PLAYER_MAX_HEALTH;
	//minigame_creeper_encounter(&health);
	//minigame_digging();
	//timer_test();

	//project1_demo_initialize_savefile();
	//project1_demo_main();
	//journey_display_test();
	//stego_engine_test("blackbox.bmp", "output2.bmp");
	//xtea_engine_test();
	//rgb_test();

	//geoencryption_engine_test();

	//rgb_init();
	//sdcard_init();
	//bitmap_display();
	//stegocrypto_test_embed();
	//picture_transfer_test();
	//bluetooth_test();
	data_transfer_test();
#endif

	//main_controller_run();

	printf("Done\n");

	return 0;
}


