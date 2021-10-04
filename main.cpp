#include "system.h"
#include <stdio.h>

//int syncPrintf(const char *format, ...);


extern int userMain(int argc,char* argv[]);


int main(int argc,char* argv[]){
	//syncPrintf("Usao u moj main\n");
	printf("usao sam u main\n");
	System::initialize();
	printf("zavrsio sam init\n");
	int ret = userMain(argc, argv);
	printf("zavrsio sam userM\n");
	System::finish();
	printf("zavrsio sam finish\n");
	return 3;
}

