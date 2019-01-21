#include <stdlib.h>
#include <fsystem.h>
#include <stdio.h>



int main(int argc, char* argv[]){ //pierwszy argument nazwa dysku wirtualnego , drugi argument co ma sie zrobić (cyfra) , 3 argument (ewentualnie nazwa pliku)
	
	int result;
	char* name;
	name=argv[1];
	//printf("%s \n", name);
	int command;
	command=atoi(argv[2]);
	/*
	1 stworz dysk
	2 usun dysk
	3 wyswietl mape zajetosci
	4 wyswietl katalog
	5 kopiuj z linuxa na virtual disc
	6 kopiuj z virtual disc na linuxa
	7 usun plik z virtual

	*/
	switch (command){
		case 1 :
			result=create_system(name);
			break;
		case 2 :
			result=delete_system(name);
			break;
		case 3:
			result=display_space_map(name);
			break;
		case 4:
			result=display_directory(name);
			break;
		case 5:
			result=add_file(name, argv[3]);
			break;
		case 6:
			result=copy_file_outside(name, argv[3]);
			break;
		case 7:
			result=delete_file(name, argv[3]);
			break;
		default:
			break;

	}
	return result;

}
