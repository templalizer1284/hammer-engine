#include "hammer.h"

int
main(void){
	
	printf("Hammer Engine %d.%d Battlecruiser operational...\n", VERSION_MAJOR, VERSION_MINOR);

	h_MainLoop();
	
	return 0;
}
