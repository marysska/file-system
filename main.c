#include <stdlib.h>
#include <fsystem.h>
#include <stdio.h>



int main(int argc, char* argv[]){
	
	int result;
	char* name;
	name=argv[1];
	printf("%s \n", name);
	result=create_system(name);
	result=display_directory(name);
	result=add_file(name, "file11.txt");
	result=add_file(name, "file12.txt");
	result=display_directory(name);
	result=add_file(name, "file13.txt");
	result=delete_file(name, "file12.txt");
	result=add_file(name, "file14.txt");

	//sleep(20);
	//result=display_directory(name);

	result=display_directory(name);
	sleep(60);
	copy_file_outside(name, "file14.txt");
	//result=delete_system(name);
	return 0;


}
