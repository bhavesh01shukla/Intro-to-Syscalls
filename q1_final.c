#include<stdio.h>
#include<stdlib.h>
#include<string.h>	
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>	
#include<errno.h> 
extern int errno;  

int main(int num,char *in[])
{
	struct stat st = {0};
	if (stat("Assignment", &st) == -1) 
	{
   		 mkdir("Assignment", 0700);
	}

	long long int size=1000*1000*10;  //size of buffer is approx 10mB
	char *buf=(char *)calloc(size,sizeof(char));
	long long int s,d;  //file descriptor
	char name[500];  //array storing file_name
	char path[500]="Assignment/";

	//printf("%s\n",in[1]);
	long long int pos=-1; // position of '/' in in[1] 
	long long int len;

	if(num==2)
	{	
		len=strlen(in[1]);		
		for(long long int k=len-1;k>=0;k--)
		{
			if(in[1][k]=='/')
			{
				pos=k;
				break;
			}
		}
	  

		//check if the file is in same folder or not by finding posn of '/'
		if(pos==-1)
		{
			//printf("in same folder\n");
			for(long long int j=0;j<=len-1;j++)
			{
				name[j]=in[1][j];
			}
			//printf("%s\n",name);
		}
		else
		{
			//printf("in differnt folder\n");
			long long int index=0;
			for(long long int j=pos+1;j<=len-1;j++)
			{
				name[index]=in[1][j];
				index++;
			}
			//printf("%s\n",name);
		}
	}
	else
	{

		write(1,"give correct num of inputs.program name and location of input file only.",strlen("give correct num of inputs.program name and location of input file only."));
		//printf("give correct num of inputs.program name and location of input file1 only.\n");
		write(1,"\n",2);
		return 0;
	}

	s=open(in[1],O_RDONLY,0);  //check while debugging
	if (s < 0) 
	{
		//write(1,"unable to open file",strlen("unable to open file"));
		//write(1,"\n",2);
		perror("input file");
		return 0;
	} 
	
	else
	{

		long long int total_size=lseek(s,0,SEEK_END);
		long long int full=total_size;  //use while finding %
		//printf("%lld\n",total_size);
		long long int itr=total_size/size;
		long long int rem=total_size%size;

		//making the path create new file
		strcat(path,name);
		//printf("%s\n",path);


    	d=open(path,O_WRONLY | O_CREAT | O_TRUNC, 0600);  //check  
    	long long int offset=0;
   	
    	lseek(s,0,SEEK_END);
    	lseek(s,-size,SEEK_CUR);

    	char ans[10];
    	//printf("%lld %lld\n",full,total_size);


    	while(total_size>=size)
    	{
    		
			long long int temp=read(s,buf,size);
			//length of buf is size
			for(long long int i=0;i<size/2;i++)  //reversing buf
			{
				char var=buf[i];
				buf[i]=buf[size-i-1];
				buf[size-i-1]=var;
			}
			total_size-=size;
			long long int temp2=write(d,buf,size);  //check if this should be int or llint
	
			if(total_size>=size)
			{lseek(s,-2*size,SEEK_CUR);}  //check while debugging

			long long int per=((full-total_size)*100)/full;
			//printf("total_size=%lld full=%lld per=%lld\n",total_size,full,per);
			long long int ones=per%10+48;
			long long int tens=(per/10)%10+48;
			long long int hundred=per/100+48;
			ans[0]=hundred;
			ans[1]=tens;
			ans[2]=ones;
			ans[3]='%';
			ans[4]='\0';
			//printf("%s\n",ans);
			write(1,ans,5);  //check this syntax while debugging
			write(1,"\r",2);
		}

		if(total_size>0 && total_size<size)
		{
			long long int pt=lseek(s,0,SEEK_SET);
			char *arr=(char *)calloc(total_size,sizeof(char));
			long long int temp3=read(s,arr,total_size);
			for(long long int j=0;j<total_size/2;j++)
			{
				char var=arr[j];
				arr[j]=arr[total_size-j-1];
				arr[total_size-j-1]=var;
			}
			long long int temp4=write(d,arr,total_size);


			total_size=0;	
			long long int per=((full-total_size)*100)/full;
			//printf("total_size=%lld full=%lld per=%lld\n",total_size,full,per);
			long long int ones=per%10+48;
			long long int tens=(per/10)%10+48;
			long long int hundred=per/100+48;
			ans[0]=hundred;
			ans[1]=tens;
			ans[2]=ones;
			ans[3]='%';
			ans[4]='\0';
			//printf("%s\n",ans);
			write(1,ans,5);  //check this syntax while debugging
			write(1,"\r",2);
			//printf("%s\n",arr);
		}
	}

	long long int t1=close(s);
	long long int t2=close(d); 

	return 0;
}
