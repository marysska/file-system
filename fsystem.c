#include <stdlib.h>
#include <fsystem.h>
#include <stdio.h>
#include <memory.h>


int create_system(char* name){
	FILE* file_system;
	long size, i;
	char * empty_file;
	file_system=fopen(name, "w");
	if (file_system==NULL){
		printf("Cannot open file\n");
		return -1;
	}
	empty_file=malloc((SIZE_OF_ALL_FILES + SIZE_OF_DESCRIPTORS+1)* sizeof(char));
	memset(empty_file, 0, SIZE_OF_ALL_FILES + SIZE_OF_DESCRIPTORS+1);
	empty_file[SIZE_OF_ALL_FILES + SIZE_OF_DESCRIPTORS]='#';
	fseek(file_system, 0, 0);
	size=fwrite(empty_file,sizeof(char) ,(SIZE_OF_ALL_FILES + SIZE_OF_DESCRIPTORS+1), file_system);
	if (size!=(SIZE_OF_ALL_FILES+SIZE_OF_DESCRIPTORS+1)){
		printf("something went wrong, cannot create system\n");
		return -1;

	}
	fclose(file_system);
	free(empty_file);
	printf("success: created file system\n");
	return 0;



}

int check_empty_space(char *name){
	int result, i;
	result=0;
	char map[NAMES_START];
	FILE *system;
	system=fopen(name, "r");
	fread(map, sizeof(char), NAMES_START, system);
	fclose(system);
	for (i=0; i<NAMES_START-1; i++){
		if(map[i]==0){
			result++;
		}
	}
	//printf("there are %d free blocks in memory \n", result);
	return result;
}



int display_directory(char *name_of_file_system){
	printf("Displaying the directory\n");
	int result, i;
	result=0;
	char map[NAMES_START];
	char name[MAX_NAME];
	//char table_size[SPACE_FOR_SIZE];
	unsigned short int size;
	FILE *system;
	char c[2];
	system=fopen(name_of_file_system, "r");
	fread(map, sizeof(char), NAMES_START, system);
	int index;
	index=0;
	for (i=0; i<NAMES_START; i++){
		if(map[i]!=0){

			fseek(system, (NAMES_START+i*(MAX_NAME+SPACE_FOR_SIZE)) ,0);

			fread(name, sizeof(char), MAX_NAME, system);
			fseek(system, (NAMES_START+i*(MAX_NAME+SPACE_FOR_SIZE)+MAX_NAME) ,0);
			fread(c, sizeof(char), SPACE_FOR_SIZE, system);
			
			char2short(c, &size);
			printf("%d . File %s real size %u in block number %d \n", index, name, size, i);
			index++; 
		}
	}
	fseek(system, 0 ,2);
	long int siz;
	siz=ftell(system);
	fclose(system);
	result=index;
	return result;
}



int add_file(char* name_of_fsystem, char* name_of_file){
	int result, i;
	result=0;
	int space;
	char map[NAMES_START];
	long int size;
	unsigned short int size_to_put;
	FILE *system, *file_to_add;
	if (check_if_name_exist(name_of_fsystem, name_of_file)!=-1){
		printf("file with this name already exist!\n");
		return -3;
	}

	
	if ((file_to_add=fopen(name_of_file, "r"))==NULL) {
     	printf ("there is no file with name %s \n", name_of_file);
     	return -4;
     }


	fseek(file_to_add, 0, SEEK_END);
	size = ftell(file_to_add);
	if (size > MAX_FILE_SIZE){
		printf("file is too big\n");
		fclose(file_to_add);
		return -1;
	}

	space=check_empty_space(name_of_fsystem);
	if (space==0){
		printf("There is no space for the file\n");
		fclose(file_to_add);
		return -2;
	}
	system=fopen(name_of_fsystem, "r");
	fseek(system, 0, 0);
	fread(map, sizeof(char), NAMES_START, system);
	int index;
	index=0;
	for (i=0; i<NAMES_START; i++){
		if(map[i]==0){
			break;
		}
	}
	char k[1];
	k[0]=1;
	size_to_put=size;
	char *c;

	fclose(system);
	system=fopen(name_of_fsystem, "r+");
	//w i mamy w nr bloku w ktorym jest miejsce
	fseek(system, i, 0);			//mapa
	fwrite(k, sizeof(char), 1, system);

	fseek(system, (NAMES_START+i*(MAX_NAME+SPACE_FOR_SIZE)) ,0);   //name
	fwrite(name_of_file, sizeof(char), size_of_name(name_of_file), system);

	fseek(system, (NAMES_START+i*(MAX_NAME+SPACE_FOR_SIZE)+MAX_NAME) ,0); //size
	fwrite(&size_to_put, sizeof(char)*2, 1, system);

	c=malloc((size)* sizeof(char));

	fseek(file_to_add, 0, 0); // odzytaj plik
	fread(c, sizeof(char), size, file_to_add);
	fclose(file_to_add);

	fseek(system, (DATA_START+i*(MAX_FILE_SIZE)) ,0); //file
	fwrite(c, sizeof(char), size, system);
	//fseek(system, (SIZE_OF_ALL_FILES + SIZE_OF_DESCRIPTORS+1) ,0);
	fclose(system);
	printf("success: file %s added to block nr %d \n", name_of_file, i);
	result=i;
	return result;


}



int check_if_name_exist(char* name_of_fsystem, char* name_of_file){
						//zwraca -1 jesli nie jest, a nr jesli jest 
	int result, i;
	result=-1;
	char map[NAMES_START];
	char name[MAX_NAME];
	FILE *system;
	system=fopen(name_of_fsystem, "r");
	fread(map, sizeof(char), NAMES_START, system);
	
	for (i=0; i<NAMES_START-1; i++){
		if(map[i]!=0){
			
			fseek(system, (NAMES_START+i*(MAX_NAME+SPACE_FOR_SIZE)) ,0);
			fread(name, sizeof(char), MAX_NAME, system);
			if (isStringEqual(name_of_file, name, MAX_NAME)!=0){
				fclose(system);
				return i;
			}
		}
	}


	fclose(system);
	return -1;




}


int delete_file(char* name_of_fsystem, char* name_of_file){
	int i;
	i=check_if_name_exist( name_of_fsystem, name_of_file);
	if (i==-1){
		printf("There isn't any file with this name in the disc\n");
		return -1;

	}
	return clearSpace(name_of_fsystem,  i);

}




int isStringEqual(char * name1, char * name2, int size){
	int i=0;
	for (i=0; i<size-1; ++i){
		if(name1[i]!=name2[i]){
			return 0;
		}
		if (name1[i]==name2[i] && name2[i]==0) return 1;
	}
	return 1;
}


int clearSpace(char * name, int i){
	int result;
	result=0;
	char c[1];
	c[0]=0;
	char descriptor[MAX_NAME+SPACE_FOR_SIZE]={0};
	char file_space[MAX_FILE_SIZE]={0};

	FILE *system;

	system=fopen(name, "r+");
	fseek(system, i, 0);			//wyczysc mape
	fwrite(c, sizeof(char), 1, system);
	
	fseek(system, (NAMES_START+i*(MAX_NAME+SPACE_FOR_SIZE)) ,0);
	fwrite(descriptor, sizeof(char), MAX_NAME+SPACE_FOR_SIZE, system);

	fseek(system, (DATA_START+i*(MAX_FILE_SIZE)) ,0);
	fwrite(file_space, sizeof(char), MAX_FILE_SIZE, system);
	fclose(system);
	printf("success: Block number %d is empty\n", i);
	return result;

}

int size_of_name(char *name){
	int i, size=0;
	for(i=0; i<MAX_NAME-1; i++){
		if (name[i]!=0){
			size++;
		}

	}
	return size;
}


void char2short(unsigned char * c, unsigned short * s){
	*s=(c[1] <<8 | c[0]);

}



int copy_file_outside(char* name_of_fsystem, char* name_of_file){
	int i;
	i=check_if_name_exist( name_of_fsystem,  name_of_file);
	if (i==-1){

		printf("There isn't any file %s in the virtual disc\n", name_of_file);
		return -1;
	}
	char size[SPACE_FOR_SIZE];
	unsigned short int s_size;
	FILE *system, *out_file;
	system=fopen(name_of_fsystem, "r");
	fseek(system, (NAMES_START+i*(MAX_NAME+SPACE_FOR_SIZE)+MAX_NAME) ,0);
	fread(size, sizeof(char), SPACE_FOR_SIZE, system);
	char2short(size, &s_size);
	char * data;
	data=malloc(s_size*sizeof(char));
	fseek(system, (DATA_START+i*(MAX_FILE_SIZE)) ,0);
	fread(data, sizeof(char), s_size, system);
	fclose(system);
	out_file=fopen(name_of_file, "w");
	fwrite(data, sizeof(char), s_size, out_file);
	fclose(out_file);
	printf("success: File %s is now on the LINUX disc\n", name_of_file);
	return i;


}


int display_space_map(char *name){
	printf("Virtual Disc memory\n");
	int result, i;
	result=0;
	char map[NAMES_START];
	//char table_size[SPACE_FOR_SIZE];
	unsigned short int size;
	FILE *system;
	char c[2];
	system=fopen(name, "r");
	fread(map, sizeof(char), NAMES_START, system);
	int index;
	index=0;
	for (i=0; i<NAMES_START; i++){
		if(map[i]==0){
			printf("index %3d   0/%3d \t", i, MAX_FILE_SIZE);
		}
		else{


			fseek(system, (NAMES_START+i*(MAX_NAME+SPACE_FOR_SIZE)+MAX_NAME) ,0);
			fread(c, sizeof(char), SPACE_FOR_SIZE, system);		
			char2short(c, &size);
			printf("index %3d %3d/%d \t", i,size, MAX_FILE_SIZE);
			
		}
		index++;
		if (index%4==0){
			printf("\n");
			index=0;
		}
	}
	fclose(system);
	result=index;
	return result;




}





int delete_system(char* name){
	int result;
	result=remove(name);
	if (result!=0){
		printf("something went wrong, cannot remove system\n");
		return result;
	}
	printf("success: delete the virtual disc\n");
	return result;

}
