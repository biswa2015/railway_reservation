#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "util.h"

void *thread_handler(void *param)
{
	struct user usr;
	struct train trn;
	struct booking book;

	int val, ret, nbytes = -1, sync = 0, train_num, type;
	int cli_fd = *(int*)param;

	do{
			
		nbytes = read(cli_fd, &val, sizeof(val));
		//printf("%d \n", val);
			
		if(val == SIGN_UP) 
		{
			printf("Sign up\n");
			read(cli_fd, &usr, sizeof(usr));
			ret = add_user(usr);
			write(cli_fd, &ret, sizeof(ret));
			printf("Sign up complete\n");	
			if(usr.type == 1)
				view_normal_user();		
		}
		else if(val == SIGN_IN)
		{
			printf("Sign In\n");
			read(cli_fd, &usr, sizeof(usr));
			ret = is_user_valid(usr);
			write(cli_fd, &ret, sizeof(ret));
			printf("Sign In complete\n");
			if(usr.type == 1)
				view_normal_user();		
		}
		else if(val == ADD_TRN)
		{
			view_train();
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &trn, sizeof(trn));
			ret = add_train(trn);
			write(cli_fd, &ret, sizeof(ret));
			view_train();
		}
		else if(val == VIEW_TRN)
		{
			view_train();
		}
		else if(val == MOD_TRN)
		{
			view_train();
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &trn, sizeof(trn));
			ret = modify_train(trn);
			write(cli_fd, &ret, sizeof(ret));
			view_train();		
		}
		else if(val == DEL_TRN)
		{
			view_train();
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &train_num, sizeof(train_num));
			ret = delete_train(train_num);
			write(cli_fd, &ret, sizeof(ret));
			view_train();		
		}
		else if(val == TCKT_BOOK)
		{
			view_train();
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &book, sizeof(book));
			view_booking(book.usr);
			ret = ticket_booking(book);
			write(cli_fd, &ret, sizeof(ret));
			view_booking(book.usr);
			view_train();
		}
		else if(val == VIEW_BOOK)
		{
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &usr, sizeof(usr));
			view_booking(usr);
		}
		else if(val == UPDT_BOOK)
		{
			view_train();
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &usr, sizeof(usr));
			view_booking(usr);
			
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &book, sizeof(book));
			ret = update_booking(book);
			write(cli_fd, &ret, sizeof(ret));
			view_booking(usr);
			view_train();
		}
		else if(val == CNCL_BOOK)
		{
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &usr, sizeof(usr));
			view_booking(usr);
			
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &book, sizeof(book));
			ret = cancel_booking(book);
			write(cli_fd, &ret, sizeof(ret));
			view_booking(usr);
			view_train();	
		}
		else if(val == ADD_USR)
		{
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &usr, sizeof(usr));
			ret = add_user(usr);
			write(cli_fd, &ret, sizeof(ret));
			view_user();
		}
		else if(val == VIEW_USR)
		{
			view_user();
		}
		else if(val == DEL_USR)
		{
			view_user();
			write(cli_fd, &sync, sizeof(sync));
			read(cli_fd, &usr, sizeof(usr));
			ret = delete_user(usr);
			write(cli_fd, &ret, sizeof(ret));
			view_user();
		}
			
	}while(nbytes > 0);
	
	printf("connection closed \n");
	close(cli_fd);		
}

int main()
{	
	struct user usr;
	struct train trn;
	pthread_t thread_id;
	struct sockaddr_in server, client;    
	int sock_fd, cli_size, cli_fd, temp, acno;
	char username[size], password[size], sample[size];
	
	int val, ret, nbytes;
	    
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);    
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = INADDR_ANY;    
	server.sin_port = htons(PORT);    
	
	
	if(bind(sock_fd, (void *)(&server), sizeof(server)) < 0)    
	{        
		perror("bind:");        
		exit(EXIT_FAILURE);    
	}    
	
	listen(sock_fd, 5);   
	
	while(1)
	{
	
		cli_size = sizeof(client);        
		if((cli_fd = accept(sock_fd, (struct sockaddr*)&client,&cli_size)) < 0) 
		{            
			perror("accept:");            
			exit(EXIT_FAILURE);        
		}
		printf("Connection established \n");
		
		pthread_create(&thread_id ,NULL, thread_handler, (void*)&cli_fd);		
	}        

}


