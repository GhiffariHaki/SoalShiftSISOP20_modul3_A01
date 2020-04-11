#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

char cwd[100];
pthread_t tid[3]; //inisialisasi dummytokay untuk menampung thread dalam kasus ini ada 2 thread
pid_t child;
char *dummytok[4],*dummytok2[20],namafile[100],dummyinput[100];
int n = 0, m = 0;

int length=5; //inisialisasi jumlah untuk looping

void* movinfile(void *arg)
{
    strcpy(dummyinput,arg);
    char *token,*token1;

	unsigned long i=0;
	pthread_t id=pthread_self();

// SARING NAMA FILE (DAPET EXTENSION)
    token1 = strtok(dummyinput, "/");
    while( token1 != NULL ) {
        dummytok2[m] = token1;
        m++;
        token1 = strtok(NULL, "/");
    }
    strcpy(namafile,dummytok2[m-1]);

    token = strtok(dummytok2[m-1], ".");
    while( token != NULL ) {
        dummytok[n] = token;
        n++;
        token = strtok(NULL, ".");
    }
	
	//SEMUANYA DIKECILIN
    char ext[100];
    strcpy(ext,dummytok[n-1]);
    for(int i = 0; ext[i]; i++){
       ext[i] = tolower(ext[i]);
    }
    
    DIR *folder, *folderopen;
    struct dirent *entry;
    char tujuanfile[100];
    folder = opendir(cwd);
    int available = 0;

	//CEK APAKAH DIREKTORI DAN EXTENSION SAMA
    if( n > 1 ){
        if(folder == NULL)printf("error\n");
        while( (entry=readdir(folder)) ){
            if(strcmp(entry->d_name,ext) == 0 && entry->d_type == 4){
                available = 1;
                break;
            }
        }

	//MAKE DIRECTORY EXTENSION
    if(available == 0){
            strcpy(tujuanfile,cwd);
            strcat(tujuanfile,"/");
            strcat(tujuanfile,ext);
            mkdir(tujuanfile, 0777);
        }
    }
    //MAKE DIRECTORY UNKNOWN
    else{
        strcpy(tujuanfile,cwd);
        strcat(tujuanfile,"/");
        strcat(tujuanfile,"Unknown");
        mkdir(tujuanfile, 0777);
    }

    char source[1024], target[1024];

    strcpy(source,arg);
    strcpy(target,cwd);
    strcat(target,"/");
    if(n == 1 ){
        strcat(target,"Unknown");
    }
    else{
        strcat(target,ext);
    }
    
    //MEMINDAHKAN FILE DENGAN RENAME ALAMAT
    strcat(target,"/");
    strcat(target,namafile);
    rename(source,target);
    
	//RESET
	n = 0;
    m = 0;

	return NULL;
}

//-----------------------------------------------------------------------------//
//------------------------------------MAIN-------------------------------------//
//-----------------------------------------------------------------------------//

int main(int argc, char *argv[]) {
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }
    int i=0,j=0,err;
    char alamatfile[100];

    // CASE -f
    if (strcmp(argv[1],"-f") == 0) {
    	//THREAD PROCESS
        for(j = 2 ; j < argc ; j++ ){
            pthread_create(&(tid[i]),NULL,movinfile,argv[j]);
            pthread_join(tid[i],NULL);
            i++;
        }
    }

    else if (strcmp(argv[1],"*") == 0 && argc == 2) {
        i = 0;
        DIR *fd, *fdo;
        struct dirent *masuk;
        fd = opendir(cwd);

        if(fd == NULL) printf("error\n");
        while( (masuk=readdir(fd)) ){
            if ( !strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, "..") )
            continue;
            //printf("%s %d\n",masuk->d_name,masuk->d_type);

            strcpy(alamatfile,cwd);
            strcat(alamatfile,"/");
            strcat(alamatfile,masuk->d_name);
            //d_type = 8 maksudnya cek apa itu file
            //THREAD PROCESS
	    if(masuk->d_type == 8){
            	pthread_create(&(tid[i]),NULL,movinfile,alamatfile); //membuat thread
            	pthread_join(tid[i],NULL);
            	i++;
            }
        }

    }

    //CASE -d
    else if (strcmp(argv[1],"-d") == 0 && argc == 3) {
        i = 0;
        DIR *fd, *fdo;
        struct dirent *masuk;
        fd = opendir(argv[2]);

        if(fd == NULL) printf("error\n");
        while( (masuk=readdir(fd)) )
        {
            if ( !strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, "..") ) continue;
            //printf("%s %d\n",masuk->d_name,masuk->d_type);
		
            strcpy(alamatfile,argv[2]);
            strcat(alamatfile,"/");
            strcat(alamatfile,masuk->d_name);
            //d_type = 8 maksudnya cek apa itu file
            //THREAD PROCESS
	    if(masuk->d_type == 8){
            	pthread_create(&(tid[i]),NULL,movinfile,alamatfile); //membuat thread
           		pthread_join(tid[i],NULL);
            	i++;
            }
        }
    }
    else printf("error\n");

return 0; 
}
