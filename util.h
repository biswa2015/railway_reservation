#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define size 1024
#define PORT 8558

#define SIGN_IN 0
#define SIGN_UP 1
#define USR_EXTS 8

#define ADD_TRN 10
#define VIEW_TRN 20
#define MOD_TRN 30
#define DEL_TRN 40

#define ADD_USR 100
#define VIEW_USR 200
#define MOD_USR 300
#define DEL_USR 400

#define TCKT_BOOK 1000
#define VIEW_BOOK 2000
#define UPDT_BOOK 3000
#define CNCL_BOOK 4000

struct user{

	int active;
	int type;  // 0 -> normal 1 -> agent 2-> admin
	char username[size];
	char password[size];
};

struct train{

	int train_no;
	int ticket_max;
	int ticket_count;
};

struct booking{

	int id;
	int train_no;  
	int ticket_booked;
	struct user usr;
};

int cancel_booking(struct booking book);

int is_user_valid(struct user clnt)
{
	struct user usr;
	int fd = open("user.txt", O_RDONLY, 0766);
	
	if(fd == -1)
		return -1;	
	
	while(read(fd, &usr, sizeof(usr)) > 0)
	{
		if((strcmp(usr.username, clnt.username) == 0 ) && (strcmp(usr.password, clnt.password) == 0 ))
		return 0;
	}
	
	return -1;
}

int add_user(struct user clnt) 
{
	int ret = is_user_valid(clnt);
	if(ret == 0)
		return USR_EXTS; 
		
	int fd = open("user.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	ret = write(fd, &clnt, sizeof(clnt));
	close(fd);
	if(ret == -1)
		return -1;
	else
		return 0; 	
}

/*int active_user(struct user clnt)
{
	struct user usr;
	usr.active = 1;
	int fd = open("user.txt", O_RDWR , 0766);
	while(read(fd, &usr, sizeof(usr)) > 0)
	{
		if((strncmp() == 0) && (strncmp() == 0))
	}	 
}

void inactive_user(struct user usr)
{
	printf("do somrthinn");
}*/

int view_user()
{
	struct user clnt;
	int fd = open("user.txt", O_RDONLY, 0766);
	
	if(fd < 2)
	{
		printf("No Users\n");
		return 0;
	}
	
	while(read(fd, &clnt, sizeof(clnt)))
	{
		printf("type %d\n", clnt.type);
		printf("username %s\n", clnt.username);
		printf("password %s\n\n", clnt.password);
	}
	
	close(fd);
}

int view_normal_user()
{
	struct user clnt;
	int fd = open("user.txt", O_RDONLY, 0766);
	
	if(fd < 2)
	{
		printf("No Users\n");
		return 0;
	}
	
	while(read(fd, &clnt, sizeof(clnt)))
	{
		if(clnt.type == 0)
		{
			printf("type %d\n", clnt.type);
			printf("username %s\n", clnt.username);
			printf("password %s\n\n", clnt.password);
		}
	}
	
	close(fd);
}


int delete_user(struct user usr)
{
	struct train trn;
	struct user clnt;
	struct booking book;
	
	int fd_book = open("booking.txt", O_RDONLY, 0766);
	int fd_book_new = open("booking_new_1.txt", O_WRONLY | O_CREAT, 0766);
	
	int fd_user = open("user.txt", O_RDONLY, 0766);
	int fd_user_new = open("user_new.txt", O_WRONLY | O_CREAT, 0766);
	
	
	if(fd_book == -1 || fd_user == -1)
	{
		printf("No ticket bookings or no users\n");
		return -1;
	}	
	
	while(read(fd_book, &book, sizeof(book)) > 0)
		write(fd_book_new, &book, sizeof(book));
		
	close(fd_book);
	close(fd_book_new);
	
	fd_book = open("booking_new_1.txt", O_RDONLY, 0766);	 
	
	printf("zzzzzzzzzzzzzzzzzzzzzzzz");
	
	while(read(fd_book, &book, sizeof(book)) > 0)
	{
		if(	(strncmp(usr.username, book.usr.username, sizeof(usr.username)) == 0) &&
			 	(strncmp(usr.password, book.usr.password, sizeof(usr.password)) == 0))
		{
			cancel_booking(book);
		}	 	
	}
	
	close(fd_book);
	
	while(read(fd_user, &clnt, sizeof(clnt)) > 0)
	{
		if(	(strncmp(usr.username, clnt.username, sizeof(clnt.username)) == 0) &&
			 	(strncmp(usr.password, clnt.password, sizeof(usr.password)) == 0))
			 	{
			 	}
		else
			write(fd_user_new, &clnt, sizeof(clnt));
			 	
	}
	
	close(fd_user);
	close(fd_user_new);
	
	//getchar();
	
	remove("booking_new_1.txt");
	//rename("booking_new_1.txt", "booking.txt");

	remove("user.txt");
	rename("user_new.txt", "user.txt");	
	
	return 0;
}

void normal_agent_menu()
{
	printf("\t1. Book Ticket\n");
	printf("\t2. View Bookings\n");
	printf("\t3. Update Booking\n");
	printf("\t4. Cancel booking\n");
	printf("\t5. Logout\n");
	printf("\tYour Choice: ");
}

void admin_menu()
{
	printf("\n\t1. operations on train\n");
	printf("\t2. operations on user\n");
	printf("\t3. Logout\n");
	printf("\t Your Choice: ");
}

void train_menu()
{
	printf("\t1. Add train\n");
	printf("\t2. View train\n");
	printf("\t3. Modify train\n");
	printf("\t4. Delete train\n");
	printf("\t Your Choice: ");
}

void user_menu()
{
	printf("\t1. Add User\n");
	printf("\t2. View users\n");
	//printf("\t3. Modify user\n");
	printf("\t4. Delete user\n");
	printf("\t Your Choice: ");
}

int add_train(struct train trn)
{
	int ret;
	int fd = open("train.txt",  O_RDWR | O_CREAT | O_APPEND, 0766);
	ret = write(fd, &trn, sizeof(trn));
	close(fd);
	if(ret == -1)
		return ret;
	else
	{
		printf("Adding train no %d", trn.train_no);	
		return 0;
	}		
}

int view_train()
{
	printf("\n======== view train =========\n");
	struct train trn;
	int fd = open("train.txt",  O_RDONLY, 0744);
	if(fd < 2)
	{
		printf("No trains\n");
		return 0;
	}
	
	while(read(fd, &trn, sizeof(trn)) > 0)
	{
		printf("train no. %d\n", trn.train_no);
		printf("total seats. %d\n", trn.ticket_max);
		printf("vacant seats. %d\n", (trn.ticket_max - trn.ticket_count));
		printf("ocupied seats. %d\n\n", trn.ticket_count);
	}
	close(fd);
	
	printf("\n=============================\n");
	
	return 0;	
}

int modify_train(struct train mod_trn)
{
	struct train trn;
	int fd, ret = -1, input = 0;
	
	fd = open("train.txt",  O_RDWR , 0766);
	if(fd == -1)
	{
		printf("No trains\n");
		return ret;
	}
	
	while(read(fd, &trn, sizeof(trn)) > 0)
	{
		input++;
		if(trn.train_no == mod_trn.train_no)
		{
			ret = 0;
			break;
		}
	}
	
	if(ret == 0)
	{
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (input - 1) * sizeof(trn);
		lock.l_len = sizeof(trn); 
		lock.l_pid = getpid();
		
		fcntl(fd, F_SETLKW, &lock);
		
		trn.ticket_max = mod_trn.ticket_max;
		lseek(fd, -1*sizeof(trn), SEEK_CUR);
		write(fd, &trn, sizeof(trn));
		lock.l_type = F_UNLCK;
	
		fcntl(fd, F_SETLK, &lock);
		printf("Modified train no %d", mod_trn.train_no);
	}
	
	return ret;
}

int delete_train(int train_num)
{
	struct train trn;
	struct booking book;
	int ret = -1;
	int fd = open("train.txt",  O_RDONLY, 0766);
	int fd_new = open("train_new.txt",  O_WRONLY | O_CREAT, 0766);
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0; 
	lock.l_pid = getpid();
	
	fcntl(fd, F_SETLKW, &lock);
	while(read(fd, &trn, sizeof(trn)))
	{
		if(trn.train_no != train_num)
			write(fd_new, &trn, sizeof(trn));
		else
		{
			ret = 0;
			printf("train num %d deleted\n", train_num);
		}		
	}
	
	close(fd);
	close(fd_new);
	
	remove("train.txt");
	rename("train_new.txt", "train.txt");
	
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	
	fd = open("booking.txt",  O_RDONLY, 0766);
	fd_new = open("booking_new.txt",  O_WRONLY | O_CREAT, 0766);
	
	while(read(fd, &book, sizeof(book)))
	{
		if(book.train_no != train_num)
			write(fd_new, &book, sizeof(book));		
	}
	
	remove("booking.txt");
	rename("booking_new.txt", "booking.txt");

	close(fd);
	close(fd_new);	
		
	return ret;
}

/*int ticket_booking(struct booking book)
{
	struct train trn;
	struct booking ticket;
	int fd_train, fd_book, ret = -1, input = 0, new_count = book.ticket_count;
	
	fd_train = open("train.txt",  O_RDWR , 0766);
	if(fd_train == -1)
	{
		printf("No trains\n");
		return ret;
	}
	
	while(read(fd_train, &trn, sizeof(trn)) > 0)
	{
		input++;
		if(trn.train_no == book.train_no)
		{
			ret = 0;
			break;
		}
	}
	
	if (ret == 0)
	{
		struct flock lock_train, lock_book;
		
		lock_train.l_type = F_WRLCK;
		lock_train.l_whence = SEEK_SET;
		lock_train.l_start = (input - 1) * sizeof(trn);
		lock_train.l_len = sizeof(trn); 
		lock_train.l_pid = getpid();
		
		lock_book.l_type = F_WRLCK;
		lock_book.l_whence = SEEK_SET;
		lock_book.l_start = 0;
		lock_book.l_len = 0; 
		lock_book.l_pid = getpid();
	
		fd_book = open("booking.txt",  O_RDWR | O_CREAT | O_EXCL, 0766);
			
		fcntl(fd_book, F_SETLKW, &lock_book);
		fcntl(fd_train, F_SETLKW, &lock_train);
		
		while(read(fd_book, &ticket, sizeof(ticket)))
		{
			if(	(strncmp(ticket.usr.username, book.usr.username, sizeof(ticket.usr.username)) == 0) &&
			 	(strncmp(ticket.usr.password, book.usr.password, sizeof(ticket.usr.password)) == 0))
			 {
			 	if(ticket.train_no == book.train_no)
			 	{
			 		lseek(fd_book, -1*sizeof(ticket), SEEK_CUR);
			 		book.ticket_count += ticket.ticket_count; 
			 		break;
			 	}
			 }
		}
		
		
		trn.ticket_count += new_count;
		lseek(fd_train, -1*sizeof(trn), SEEK_CUR);
		
		write(fd_train, &trn, sizeof(trn));
		write(fd_book, &book, sizeof(book));
	
		lock_book.l_type = F_UNLCK;
		lock_train.l_type = F_UNLCK;
		
		fcntl(fd_book, F_SETLK, &lock_book);
		fcntl(fd_train, F_SETLK, &lock_train);

	}	
	
	return ret;
}*/

int ticket_booking(struct booking book)
{
	struct train trn;
	struct booking ticket;
	int fd_train, fd_book, ret = -1, input = 0;
	
	fd_train = open("train.txt",  O_RDWR , 0766);
	if(fd_train == -1)
	{
		printf("No trains\n");
		return ret;
	}
	
	while(read(fd_train, &trn, sizeof(trn)) > 0)
	{
		input++;
		if(trn.train_no == book.train_no)
		{
			ret = 0;
			break;
		}
	}
	
	if (ret == 0)
	{
		struct flock lock_train, lock_book;
		
		lock_train.l_type = F_WRLCK;
		lock_train.l_whence = SEEK_SET;
		lock_train.l_start = (input - 1) * sizeof(trn);
		lock_train.l_len = sizeof(trn); 
		lock_train.l_pid = getpid();
		
		lock_book.l_type = F_WRLCK;
		lock_book.l_whence = SEEK_SET;
		lock_book.l_start = 0;
		lock_book.l_len = 0; 
		lock_book.l_pid = getpid();
	
		fd_book = open("booking.txt",  O_RDWR | O_CREAT | O_APPEND, 0766);
			
		fcntl(fd_book, F_SETLKW, &lock_book);
		fcntl(fd_train, F_SETLKW, &lock_train);		
		
		trn.ticket_count += book.ticket_booked;
		lseek(fd_train, -1*sizeof(trn), SEEK_CUR);
		
		write(fd_train, &trn, sizeof(trn));
		lseek(fd_book, 0, SEEK_END);
		write(fd_book, &book, sizeof(book));
	
		lock_book.l_type = F_UNLCK;
		lock_train.l_type = F_UNLCK;
		
		fcntl(fd_book, F_SETLK, &lock_book);
		fcntl(fd_train, F_SETLK, &lock_train);

	}	
	
	return ret;
}

int update_booking(struct booking book)
{
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	struct train trn;
	struct booking ticket;
	int fd_train, fd_book, ret1 = -1, ret2 = -1, input1 = 0, input2 = 0;
	
	fd_train = open("train.txt",  O_RDWR , 0766);
	if(fd_train == -1)
	{
		printf("No trains\n");
		return ret1;
	}
	
	while(read(fd_train, &trn, sizeof(trn)) > 0)
	{
		input1++;
		if(trn.train_no == book.train_no)
		{
			ret1 = 0;
			break;
		}
	}
	
	if(ret1 == 0)
	{
		fd_book = open("booking.txt",  O_RDWR , 0766);
		if(fd_book == -1)
		{
			printf("No Bookings\n");
			return ret2;
		}
		
		while(read(fd_book, &ticket, sizeof(ticket)) > 0)
		{
			input2++;
			if((ticket.id == book.id) && (ticket.train_no == book.train_no))
			{
				ret2 = 0;
				break;
			}
		}
		if(ret2 == 0)
		{
			struct flock lock_train, lock_book;
		
			lock_train.l_type = F_WRLCK;
			lock_train.l_whence = SEEK_SET;
			lock_train.l_start = (input1 - 1) * sizeof(trn);
			lock_train.l_len = sizeof(trn); 
			lock_train.l_pid = getpid();
			
			lock_book.l_type = F_WRLCK;
			lock_book.l_whence = SEEK_SET;
			lock_book.l_start = (input2 - 1) * sizeof(book);
			lock_book.l_len = sizeof(book); 
			lock_book.l_pid = getpid();
			
			fcntl(fd_book, F_SETLKW, &lock_book);
			fcntl(fd_train, F_SETLKW, &lock_train);		
			
			trn.ticket_count += book.ticket_booked;
			lseek(fd_train, -1*sizeof(trn), SEEK_CUR);
			write(fd_train, &trn, sizeof(trn));
			
			ticket.ticket_booked +=book.ticket_booked;
			lseek(fd_book, -1*sizeof(book), SEEK_CUR);
			write(fd_book, &ticket, sizeof(ticket));
		
			lock_book.l_type = F_UNLCK;
			lock_train.l_type = F_UNLCK;
			
			fcntl(fd_book, F_SETLK, &lock_book);
			fcntl(fd_train, F_SETLK, &lock_train);
			
			printf("ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ\n");
		}
	}
	
	return (ret1 | ret2);
}

int cancel_booking(struct booking book)
{
	struct train trn;
	struct booking ticket;
	int fd_train, fd_book, ret1 = -1, ret2 = -1, input1 = 0, input2 = 0;
	
	fd_train = open("train.txt",  O_RDWR , 0766);
	if(fd_train == -1)
	{
		printf("No trains\n");
		return ret1;
	}
	
	while(read(fd_train, &trn, sizeof(trn)) > 0)
	{
		input1++;
		if(trn.train_no == book.train_no)
		{
			ret1 = 0;
			break;
		}
	}
	
	if(ret1 == 0)
	{
		fd_book = open("booking.txt",  O_RDWR , 0766);
		if(fd_book == -1)
		{
			printf("No Bookings\n");
			return ret2;
		}
		
		while(read(fd_book, &ticket, sizeof(ticket)) > 0)
		{
			input2++;
			if((ticket.id == book.id) && (ticket.train_no == book.train_no))
			{
				ret2 = 0;
				break;
			}
		}
		

		if(ret2 == 0)
		{
			struct flock lock_train, lock_book;
		
			lock_train.l_type = F_WRLCK;
			lock_train.l_whence = SEEK_SET;
			lock_train.l_start = (input1 - 1) * sizeof(trn);
			lock_train.l_len = sizeof(trn); 
			lock_train.l_pid = getpid();
			
			lock_book.l_type = F_WRLCK;
			lock_book.l_whence = SEEK_SET;
			lock_book.l_start = (input2 - 1) * sizeof(book);
			lock_book.l_len = sizeof(book); 
			lock_book.l_pid = getpid();
			
			fcntl(fd_book, F_SETLKW, &lock_book);
			fcntl(fd_train, F_SETLKW, &lock_train);		
			
			trn.ticket_count -= book.ticket_booked;
			lseek(fd_train, -1*sizeof(trn), SEEK_CUR);
			write(fd_train, &trn, sizeof(trn));
			
			if(ticket.ticket_booked != book.ticket_booked)
			{
				/*close(fd_book);
				
				fd = open("booking.txt",  O_RDONLY, 0766);
				fd_new = open("booking_new.txt",  O_WRONLY | O_CREAT, 0766);
	
				while(read(fd, &book, sizeof(book)))
				{
					if(book.train_no != train_num)
					write(fd_new, &book, sizeof(book));		
				}
	
				remove("booking.txt");
				rename("booking_new.txt", "booking.txt");

				close(fd);
				close(fd_new);	
				return 0;*/
				
				ticket.ticket_booked -=book.ticket_booked;
				lseek(fd_book, -1*sizeof(book), SEEK_CUR);
				write(fd_book, &ticket, sizeof(ticket));
		
				lock_book.l_type = F_UNLCK;
				lock_train.l_type = F_UNLCK;
			
				fcntl(fd_book, F_SETLK, &lock_book);
				fcntl(fd_train, F_SETLK, &lock_train);
				
				close(fd_book);
				close(fd_train);
			}
			else
			{
				int fd = open("booking.txt",  O_RDONLY, 0766);
				int fd_new = open("booking_new.txt",  O_WRONLY | O_CREAT, 0766);
	
				while(read(fd, &ticket, sizeof(ticket)))
				{
					if((ticket.id == book.id) && (ticket.train_no == book.train_no))
					{
					}
					else
						write(fd_new, &ticket, sizeof(ticket));		
				}
	
				remove("booking.txt");
				rename("booking_new.txt", "booking.txt");

				close(fd);
				close(fd_new);	
				return 0;
			}
			
			
		}
	}
	
	return (ret1 | ret2);
	
}

int view_booking(struct user usr)
{
	printf("\n======== view booking =========\n");
	printf("Username : %s\n",usr.username);
	printf("password : %s\n",usr.password);
	
	int flag = 1;
	struct booking book;
	int fd = open("booking.txt",  O_RDONLY, 0744);
	if(fd < 2)	
	{
		printf("No booking\n"); 
		return 0;
	}
	while(read(fd, &book, sizeof(book)) > 0)
	{
		if((strncmp(book.usr.username, usr.username, sizeof(book.usr.username)) == 0) 
		&& (strncmp(book.usr.password, usr.password, sizeof(book.usr.password)) == 0))
		{
			printf("booking id. %d\n", book.id);
			printf("train no. %d\n", book.train_no);
			printf("tickets booked. %d\n\n", book.ticket_booked);
			flag = 0;	
		}
		
	}
	close(fd);
	if (flag) printf("No booking\n"); 
	printf("\n=============================\n");
	
	return 0;	
}

void view_all_booking()
{
	printf("\n======== view all booking =========\n");	
	struct booking book;
	int fd = open("booking.txt",  O_RDONLY, 0744);
	while(read(fd, &book, sizeof(book)) > 0)
	{
		printf("booking id %d\n", book.id);
		printf("train no. %d\n", book.train_no);
		printf("tickets booked. %d\n\n", book.ticket_booked);
	}
	
	close(fd);
}

