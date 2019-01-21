#ifndef FSYSTEM_H

#define FSYSTEM_H

#define MAX_FILE_SIZE 512
#define MAX_NAME 13 


#define SPACE_FOR_SIZE 2
#define SIZE_OF_ALL_FILES 131072 //256 plikow po maks 0,5kB
#define SIZE_OF_DESCRIPTORS 4096 //256 Plikow - nazwa maksymalnie 13 znakow plus mapa zajetosci i dwa znaki na rozmiar rzeczywisty pliku pamieci

#define NAMES_START 256 //od tego bajtu zaczynaja sie nazwy 

#define DATA_START 4096 // od tego bajtu zaczynaja sie kolejne pliki bedzie duza segmentacja wewnetrzna

int create_system(char* name);

int check_empty_space(char *name);

int display_directory(char *name);

int delete_system(char* name_of_file_system);

int add_file(char* name_of_fsystem, char* name_of_file);

int check_if_name_exist(char* name_of_fsystem, char* name_of_file); //w razie co zwraca nr bloku gdzie jest dany plik 

int isStringEqual(char * name1, char * name2, int size);

int clearSpace(char * name, int i);

int delete_file(char* name_of_fsystem, char* name_of_file);

int copy_file_outside(char* name_of_fsystem, char* name_of_file);

int size_of_name(char *name);

void char2short(unsigned char * c, unsigned short * s);

#endif //FSYSTEM_h
