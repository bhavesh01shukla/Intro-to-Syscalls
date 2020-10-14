#include<stdio.h>
#include<stdlib.h>
#include<string.h>	
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>	
#include<errno.h> 
extern int errno;  

//function for printing file and directory permissions
void func(char str[],char com[],int f,char res[])
{	
	char *file;
	if(f==1){file="oldfile";}
	else if(f==2){file="newfile";}
	else if(f==3){file="directory";}

	write(1,str,strlen(str));
	write(1," has ",strlen(" has "));
	write(1,com,strlen(com));
	write(1," permissions on ",strlen(" permissions on "));
	write(1,file,strlen(file));
	write(1,": ",2);
	write(1,res,strlen(res));
	//write(1," ",2);
	write(1,"\n",2);
}

int main(int num,char *in[])
{

	long long int size=1000*1000*10;  //size of buffer is approx 10mB
	char *x=(char *)calloc(size,sizeof(char));  //buffer for original
	char *y=(char *)calloc(size,sizeof(char));  //buffer for reversed
	long long int org,rev;  //file descriptor

		

	//check if no of inputs is correct	
	if(num!=4)
	{
		write(1,"give correct num of inputs.program name and specified input only.",strlen("give correct num of inputs.program name and specified input only."));
		//printf("give correct num of inputs.program name and location of input file1 only.\n");
		write(1,"\n",2);
		return 0;
	}
	
	//checking if the directory exists or not
	struct stat filecheck;
    if(stat(in[3],&filecheck)==0 && S_ISDIR(filecheck.st_mode))    
    {
    	write(1,"Directory is created: ",strlen("Directory is created: "));
		write(1,"Yes",strlen("Yes"));
		write(1,"\n",2);
    }
    else
    {
    	write(1,"Directory is created: ",strlen("Directory is created: "));
		write(1,"No",strlen("No"));
		write(1,"\n",2);	
    }

	for(int i=1;i<4;i++)
	{
		write(1,"\n",2);
		if(i==1)
		{write(1,"oldfile:",strlen("oldfile:"));}
		else if(i==2)
		{write(1,"newfile:",strlen("newfile:"));}
		else if(i==3)
		{write(1,"directory:",strlen("directory:"));}
		write(1,"\n",2);

		struct stat fileStat;
	    if(stat(in[i],&fileStat) < 0)    
	    {
	    	write(1,"Does not exists",strlen("Does not exists"));
	    	write(1,"\n",2);
	    	continue;
	    }

		if(fileStat.st_mode & S_IRUSR)
	    {func("user","read",i,"yes");}
		else{func("user","read",i,"no");}

	    if(fileStat.st_mode & S_IWUSR)
	    {func("user","write",i,"yes");}
		else{func("user","write",i,"no");}

	    if(fileStat.st_mode & S_IXUSR) 
	    {func("user","execute",i,"yes");}
		else{func("user","execute",i,"no");}

	    if(fileStat.st_mode & S_IRGRP)
	    {func("group","read",i,"yes");}
		else{func("group","read",i,"no");}

	    if(fileStat.st_mode & S_IWGRP)
	    {func("group","write",i,"yes");}
		else{func("group","write",i,"no");}

	    if(fileStat.st_mode & S_IXGRP)
	    {func("group","execute",i,"yes");}
		else{func("group","execute",i,"no");}

	    if(fileStat.st_mode & S_IROTH)
	    {func("others","read",i,"yes");}
		else{func("others","read",i,"no");}

	    if(fileStat.st_mode & S_IWOTH) 
	    {func("others","write",i,"yes");}
		else{func("others","write",i,"no");}

	    if(fileStat.st_mode & S_IXOTH) 
	    {func("others","execute",i,"yes");}
		else{func("others","execute",i,"no");}
	}
	

	org=open(in[1],O_RDONLY,0);  //check while debugging
	rev=open(in[2],O_RDONLY,0);  //check while debugging

	if (org>=0 && rev>=0)  //if both files open successfully   
	{

		long long int l1=lseek(org,0,SEEK_END);
		long long int l2=lseek(rev,0,SEEK_END);

		if(l1==l2)  //if both files are of same size
		{
			long long int total_size=l1;
			lseek(org,0,SEEK_SET);
			lseek(rev,0,SEEK_END);

			lseek(rev,-size,SEEK_CUR);  //check while debugging
			while(total_size>=size)
			{
				//printf("hi\n");

				//buffer x for org and buffer y for rev
				read(org,x,size);
				read(rev,y,size);
				//reversing the y buffer(rev file)
				for(long long int i=0;i<size/2;i++)
				{
					char temp=y[i];
					y[i]=y[size-i-1];
					y[size-i-1]=temp;
				}

				//check if the 2 buffer are same
				for(long long int i=0;i<size;i++)
				{
					if(x[i]!=y[i])
					{
						//printf("xyz\n");
						write(1,"\n",2);
						write(1,"Whether file contents are reversed in newfile: No",strlen("Whether file contents are reversed in newfile: No"));
						write(1,"\n",2);
						return 0;
					}
				}
				total_size-=size;
				//check cursor positions
				if(total_size>=size)
				{lseek(rev,-2*size,SEEK_CUR);}  //check while debugging

			}
			if(total_size>0 && total_size<size)
			{
				lseek(rev,0,SEEK_SET);//set cursor of rev to 0;
				char *a=(char *)calloc(total_size,sizeof(char));//buffer for org file
				char *b=(char *)calloc(total_size,sizeof(char));//buffer for rev file

				read(org,a,total_size); 
				read(rev,b,total_size);  //reverse this and match

				for(long long int i=0;i<total_size/2;i++)  //reversing buffer b
				{
					char temp=b[i];
					b[i]=b[total_size-i-1];
					b[total_size-i-1]=temp;	
				}
				//match a and reversed b now
				for(long long int i=0;i<total_size;i++)
				{
					if(a[i]!=b[i])
					{
						//printf("abc\n");
						write(1,"\n",2);
						write(1,"Whether file contents are reversed in newfile: No",strlen("Whether file contents are reversed in newfile: No"));
						write(1,"\n",2);
						return 0;
					}
				}
			}
			write(1,"\n",2);
			write(1,"Whether file contents are reversed in newfile: Yes",strlen("Whether file contents are reversed in newfile: Yes"));
			write(1,"\n",2);
		}
		else
		{
			write(1,"\n",2);
			write(1,"Whether file contents are reversed in newfile: No",strlen("Whether file contents are reversed in newfile: No"));
			write(1,"\n",2);
			return 0;
		}

	} 	
	else
	{
		write(1,"\n",2);
		if(org<0 && rev<0)
		{perror("oldfile");perror("newfile");}
		else if(org>=0 && rev<0)
		{perror("newfile");}
		else if(org<0 && rev>=0)
		{perror("oldfile");}

		write(1,"\n",2);
		write(1,"Unable to open files.Thus,cannot check if reversed or not",strlen("Unable to open files.Thus,cannot check if reversed or not"));
		write(1,"\n",2);
		return 0;
	}
	return 0;
}