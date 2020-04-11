# SoalShiftSISOP20_modul3_A01

```
- Mochamad Haikal Ghiffari        05111840000095
- Nikodemus Siahaan               05111840000151
```

# Soal Nomor 3
Source code : [soal3.c](https://github.com/GhiffariHaki/SoalShiftSISOP20_modul3_A01/blob/master/soal3/soal3.c)

Soal ini memerintahkan untuk menyimpan file dalam folder yang namanya sesuai dengan ekstensinya. Jika tidak ada ekstensi, maka file akan ditaruh dalam folder Unknown. Dalam Program untuk memecahkan soal ini, kami membaginya menjadi 2 fungsi yaitu, Fungsi Main dan Fungsi Movinfile. Berikut adalah penjelasan tentang fungsi-fungsi tersebut.

# Fungsi MovinFile

Fungsi Movinfile dimulai dengan langkah mendapatkan ekstensinya. Disini saya menggunakan strtok untuk mendapatkan namafile terlebih dahulu setelah itu saya strtok lagi namafile yang telah didapat dengan . dan ambil kata dengan . terakhir sebagai ekstensi. Setelah itu ekstensi semuanya menjadi lower case agar mencegah terjadinya double direktori dengan ekstensi sama. Potongan kodenya adalah sebagai berikut :
```
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
```

Setelah itu, kita mengecek apabila direktori tentang ekstension tersebut telah ada. Oleh karena itu, kita mengecek apakah nama direktori-direktori sama dengan nama ekstension. Jika, tidak ada kami membuat direktori tersebut. Dan jika tidak ada ekstensi maka akan dibuat folder Unknown. Potongan kodenya adalah sebagai berikut:

```
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
```
Setelah itu, file akan dipindahkan kedalam folder ekstensi yang telah dibuat sebelumnya. File dipindahkan menggunakan perintah Rename alamat folder. Alamat yang telah dimasukan di argumen akan diubah menjadi working directory terus ditambahkan dengan Ekstensi atau Unknown setelah itu di rename. Potongan Kodenya adalah sebagai berikut :
```
    //MEMINDAHKAN FILE DENGAN RENAME ALAMAT
    strcpy(source,arg);
    strcpy(target,cwd);
    strcat(target,"/");
    if(n == 1 ){
        strcat(target,"Unknown");
    }
    else{
        strcat(target,ext);
    }
    strcat(target,"/");
    strcat(target,namafile);
    rename(source,target);
```

# Fungsi Main

Fungsi main didalam program ini bertugas untuk mengecheck argumen karena user dapat memilih memasukan banyak alamat file untuk disortir   (-f),  atau satu direktori untuk disortir, dan juga pilihan untuk menyortir semua file dalam working directory.

```
    // CASE -f
    if (strcmp(argv[1],"-f") == 0) {
    	//THREAD PROCESS
        for(j = 2 ; j < argc ; j++ ){
            pthread_create(&(tid[i]),NULL,movinfile,argv[j]);
            pthread_join(tid[i],NULL);
            i++;
        }
    }
```

Dalam case untuk argumen (-f) user dapat menginput banyak alamat file. Oleh karena itu, kumpulan argumen tersebut akan dimasukan kedalam fungsi movinfile yang ditugaskan untuk menyortir.

```
//CASE -d
    else if (strcmp(argv[1],"-d") == 0 && argc == 3) {
        i = 0;
        DIR *fd, *fdo;
        struct dirent *masuk;
        fd = opendir(argv[2]); //jika perintah '*' maka line ini akan menjadi "fd = opendir(cwd);

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
```

Case untuk -d dan * potongan kodenya sangat mirip karena hanya mengubah alamat direktori yang akan dimasukinya. Cara kerjanya adalah kita membuka sebuah direktori (fd) setelah itu kita membacanya (readdir) setelah itu kita simpan alamatfilenya lalu dimasukan kedalam fungsi movinfile.
