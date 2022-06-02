#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include "util.h"

struct user usr;
struct user clnt;
struct train trn_new;

int agent = 0;
int credentials();
int admin_user_add();

int main()
{
	char buff[size], to_server[size], username[size], password[size];
	int i, q, ret, ch, val, sock_fd, sync = 0; ;
	
	struct sockaddr_in server;    
	   
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);    
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = INADDR_ANY;    
	server.sin_port = htons(PORT);   
	connect(sock_fd, (void *)(&server), sizeof(server)); 
	 
	printf("\n=================== Welcome To Railway Ticket Reservation System ===================\n");
	printf(" Login(0) or Sign up(1) ");
	scanf("%d", &val);
	
	ret = credentials();
	if(ret == -1)
	{
		printf("wrong credential input\n");
		exit(EXIT_FAILURE);
	}
	
	if(val == SIGN_IN || val == SIGN_UP)
	{
		write(sock_fd, &val, sizeof(val));
		sleep(1);
		write(sock_fd, &clnt, sizeof(clnt));
		printf("server reply \n");
		read(sock_fd, &ret, sizeof(ret));
		 
		if(ret == -1 && val == SIGN_IN)
		{
			printf("Wrong type, username or password\n");
			return 0;
		}
		else if (ret == USR_EXTS && val == SIGN_UP)
		{
			printf("user already exists\n");
			return 0;
		}
		
		else if(ret == -1 && val == SIGN_UP)
		{
			printf("problem adding user\n");
			exit(EXIT_FAILURE);
		}
		else if(ret == 0)
		{
			printf("Sign up / Sign in successful\n");
		}					
	}
	else 
	{
		printf("wrong choice\n");
		exit(EXIT_FAILURE);
	}		  
		
	switch(clnt.type)
	{
		case 2: /* admin */
				while(1)
				{
					admin_menu();
					scanf("%d", &val);
				
					if(val == 1)
					{ 
						train_menu(); 
						scanf("%d", &val);
						val = val * 10;
						write(sock_fd, &val, sizeof(val));
						if(val == ADD_TRN)
						{
							read(sock_fd, &sync, sizeof(sync));
						
							printf("Adding train\n");
							trn_new.ticket_max = 10;
							trn_new.ticket_count = 0;
							printf("train no.: ");
							scanf("%d", &trn_new.train_no);
							write(sock_fd, &trn_new, sizeof(trn_new));
							read(sock_fd, &ret, sizeof(ret));
							if(ret == 0)
								printf("Train addition successful\n");
							else
								printf("Train addition unsuccessful\n");	
						}
						else if(val == VIEW_TRN)
						{
						//view_train();
						}
						else if(val == MOD_TRN)
						{
							read(sock_fd, &sync, sizeof(sync));
						
							struct train trn_mod;
						
							printf(" ===DISCLAIMER : DONOT REDUCE TOTAL SEATS BELOW OCCUPIED SEATS===\n");
							printf("Train details you want to modify\n");
							printf("train no. "); 	scanf("%d", &trn_mod.train_no);
							printf("total seats.");	scanf("%d", &trn_mod.ticket_max);
					
							write(sock_fd, &trn_mod, sizeof(trn_mod));
							read(sock_fd, &ret, sizeof(ret));
							if(ret == 0) printf("Modified succesfully\n");
							else printf("Modification not succesful\n");
						
						//view_train(); 
						}
					
						else if(val == DEL_TRN)
						{
							read(sock_fd, &sync, sizeof(sync));
						
							int train_num;
							printf("train no. "); 	
							scanf("%d", &train_num);
						
							write(sock_fd, &train_num, sizeof(train_num));
							read(sock_fd, &ret, sizeof(ret));
							if(ret == 0)
								printf("Train deletion successful\n");
							else
								printf("Train deletion unsuccessful\n");
						}		
						else
							break;	
					}
					
					else if (val == 2) 
					{
						user_menu(); 
						scanf("%d", &val);
						val = val * 100;
						write(sock_fd, &val, sizeof(val));
						if(val == ADD_USR)
						{
							read(sock_fd, &sync, sizeof(sync));
							admin_user_add();
							write(sock_fd, &usr, sizeof(usr));
							read(sock_fd, &ret, sizeof(ret));
							
							if(ret == 0)
								printf("Successful User addition \n");
							else
								printf("Unsuccessful User addition \n");			
							
						}
						else if(val == VIEW_USR)
						{
							
						}
						else if(val == DEL_USR)
						{
							read(sock_fd, &sync, sizeof(sync));
							admin_user_add();
							write(sock_fd, &usr, sizeof(usr));
							read(sock_fd, &ret, sizeof(ret));
							
							if(ret == 0)
								printf("Successful User deletion \n");
							else
								printf("Unsuccessful User deletion \n");						
						}
						else
							break;
					}
					else 
						break;
					
				}
				
				
					/*else if (val == 2) 
					{
						user_menu(); 
						scanf("%d", &val);
						val = val * 10;
						write(sock_fd, &val, sizeof(val));
						if(val == ADD_USR)
						{
						}
						else if(val == ADD_USR)
						{
						}
						else if(val == ADD_USR)
						{
						}
						else if(val == ADD_USR)
						{
						}
						else
							break;
					}*/		
			break;
		default: /* normal or agent */
				while(1)
				{
					agent = clnt.type;
					
					if(agent == 1)
					{
						printf(" For which user you want to modify");
						ret = credentials();	
						if(ret == -1)
						{
							printf("wrong credential input\n");
							exit(EXIT_FAILURE);
						}
					}
					
					normal_agent_menu();
					scanf("%d", &val);
					val = val * 1000;
					write(sock_fd, &val, sizeof(val));
					if(val == TCKT_BOOK)
					{
						read(sock_fd, &sync, sizeof(sync));
						struct booking book;
						book.usr.type = clnt.type;
						strncpy(book.usr.username, clnt.username, sizeof(clnt.username));
						strncpy(book.usr.password, clnt.password, sizeof(clnt.password));
						
						book.id = rand();
						printf("Enter booking id. : %d \n", book.id);
												
						printf("Enter train no. :");
						scanf("%d", &book.train_no);
						
						printf("Enter tickets :");
						scanf("%d", &book.ticket_booked);
						
						write(sock_fd, &book, sizeof(book));
						read(sock_fd, &ret, sizeof(ret));
						
						if(ret == -1)
							printf("Ticket booking successful\n");
						else
							printf("Ticket booking unsuccessful\n");	
					}
					else if(val == VIEW_BOOK)
					{
						read(sock_fd, &sync, sizeof(sync));
						write(sock_fd, &clnt, sizeof(clnt));
					}
					else if(val == UPDT_BOOK)
					{
						read(sock_fd, &sync, sizeof(sync));
						write(sock_fd, &clnt, sizeof(clnt));
						read(sock_fd, &sync, sizeof(sync));
						
						struct booking book;
						book.usr.type = clnt.type;
						strncpy(book.usr.username, clnt.username, sizeof(clnt.username));
						strncpy(book.usr.password, clnt.password, sizeof(clnt.password));

						printf("Enter booking id. :"); 
						scanf("%d", &book.id);
												
						printf("Enter train no. :");
						scanf("%d", &book.train_no);
						
						printf("Enter tickets :");
						scanf("%d", &book.ticket_booked); 
						
						write(sock_fd, &book, sizeof(book));
						read(sock_fd, &ret, sizeof(ret));
						
						if(ret == -1)
							printf("Ticket updation successful\n");
						else
							printf("Ticket updation unsuccessful\n");
					}
					else if(val == CNCL_BOOK)
					{
						read(sock_fd, &sync, sizeof(sync));
						write(sock_fd, &clnt, sizeof(clnt));
						read(sock_fd, &sync, sizeof(sync));
						
						struct booking book;
						book.usr.type = clnt.type;
						strncpy(book.usr.username, clnt.username, sizeof(clnt.username));
						strncpy(book.usr.password, clnt.password, sizeof(clnt.password));

						printf("Enter booking id. :"); 
						scanf("%d", &book.id);
												
						printf("Enter train no. :");
						scanf("%d", &book.train_no);
						
						printf("Enter tickets :");
						scanf("%d", &book.ticket_booked); 
						
						write(sock_fd, &book, sizeof(book));
						read(sock_fd, &ret, sizeof(ret));
						
						if(ret == -1)
							printf("Ticket cancellation successful\n");
						else
							printf("Ticket cancellation unsuccessful\n");	
					}
					
					else
						break;
				}	
					
			break;		
	}
	
	close(sock_fd);
	printf("log out\n");

}

int credentials()
{
	char su[5]; 
	
	printf("Acc type normal(0) or agent(1) or Admin(2): \n");
	scanf("%d", &clnt.type);
	
	if((clnt.type != 0) && (clnt.type != 1) && (clnt.type != 2))
		return -1;
	
	if(clnt.type == 2) //admin
	{
		printf("Enter admin previleges password\n");
		scanf("%s", su);
		if(strncmp(su, "iiitb", 5) != 0)
			return -1;
	}
	
		
	printf("enter username: ");
	scanf("%s", clnt.username);
	 	
	printf("enter password: ");
	scanf("%s", clnt.password);
	
	return 0;
}

int admin_user_add()
{
	char su[5];
	printf("Acc type normal(0) or agent(1) or Admin(2): \n");
	scanf("%d", &usr.type);
	
	if((usr.type != 0) && (usr.type != 1) && (usr.type != 2))
		return -1;
	
	if(usr.type == 2) //admin
	{
		printf("Enter admin previleges password\n");
		scanf("%s", su);
		if(strncmp(su, "iiitb", 5) != 0)
			return -1;
	}
	
		
	printf("enter username: ");
	scanf("%s", usr.username);
	 	
	printf("enter password: ");
	scanf("%s", usr.password);
	
	return 0;
}

