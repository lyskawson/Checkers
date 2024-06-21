/*Compile on Linux:
gcc -o broker -g -Wall -std=c17 -pedantic broker.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define BUFSPACE 1024

#define WHITE 0
#define BLACK 1
#define OPPONENT(player) (1-player)
char *playername[]={"BIALY", "CZARNY"};
char *winnername[]={"WW", "BW", "DRAW"}; 


#define BLACK_M 51
#define WHITE_M 52
#define BLACK_K 53
#define WHITE_K 54
#define EMPTY_F 55
#define INVALID 56
#define OUTSIDE 57
#define IS_PIECE(piece) ((piece>=BLACK_M)&&(piece<=WHITE_K))
#define IS_MAN(piece)   ((piece>=BLACK_M)&&(piece<=WHITE_M))
#define IS_KING(piece)  ((piece>=BLACK_K)&&(piece<=WHITE_K))
#define OWNER(piece)    ((piece-50)%2)
char *symbols[]={"!!!", " B ", " W ", "(B)", "(W)", "___", "   ", "***", "???"};

int hor[34]={-1,2,4,6,8,1,3,5,7,2,4,6,8,1,3,5,7,2,4,6,8,1,3,5,7,2,4,6,8,1,3,5,7,-1};
int ver[34]={-1,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,-1};
int board[10][10] =   { { 57, 57, 57, 57, 57, 57, 57, 57, 57, 57 },
			{ 57, 56, 51, 56, 51, 56, 51, 56, 51, 57 },
			{ 57, 51, 56, 51, 56, 51, 56, 51, 56, 57 },
			{ 57, 56, 51, 56, 51, 56, 51, 56, 51, 57 },
			{ 57, 55, 56, 55, 56, 55, 56, 55, 56, 57 },
			{ 57, 56, 55, 56, 55, 56, 55, 56, 55, 57 },
			{ 57, 52, 56, 52, 56, 52, 56, 52, 56, 57 },
			{ 57, 56, 52, 56, 52, 56, 52, 56, 52, 57 },
			{ 57, 52, 56, 52, 56, 52, 56, 52, 56, 57 },
			{ 57, 57, 57, 57, 57, 57, 57, 57, 57, 57 } };

int fields[10][10] =  { { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2 },
			{ -2, -1,  1, -1,  2, -1,  3, -1,  4, -2 },
			{ -2,  5, -1,  6, -1,  7, -1,  8, -1, -2 },
			{ -2, -1,  9, -1, 10, -1, 11, -1, 12, -2 },
			{ -2, 13, -1, 14, -1, 15, -1, 16, -1, -2 },
			{ -2, -1, 17, -1, 18, -1, 19, -1, 20, -2 },
			{ -2, 21, -1, 22, -1, 23, -1, 24, -1, -2 },
			{ -2, -1, 25, -1, 26, -1, 27, -1, 28, -2 },
			{ -2, 29, -1, 30, -1, 31, -1, 32, -1, -2 },
			{ -2, -2, -2, -2, -2, -2, -2, -2, -2, -2 } };

/* Move result codes, mostly used in the make-move function:
   Negative - programming errors, illegal or impossible situations
   0 - valid move
   100 - valid move + pawn promotion
   1 - attempt of an illegal move - 'from' square is incorrect
   2 - attempt of an illegal move - 'to' square is incorrect
   3 - attempt of an illegal move - no player's piece on 'from' square
   4 - attempt of an illegal move - 'to' square is occupied
   5 - attempt of an illegal ordinary move - move geometry error
   6 - attempt of an illegal capture move - move geometry error
   7 - attempt of an illegal capture move - no opponent's piece between 'from' and 'to'
   8 - attempt of an illegal ordinary/capture move - pawn moving backwards
   ---- conditions 1-8 are checked in the make_move function; the rest in the main program
   9 - attempt of an illegal ordinary move - player has a capture available
   10 - attempt of an illegal capture move - after pawn promotion, backward capture
   11 - attempt of an illegal capture move - further capture possible in final square
   12..19 - codes reserved for other cases of illegal moves

   Program result codes (status), plus game result signaling:
   Negative
   - programming errors, illegal or impossible situations
     - interpreted as an unresolved game, like a draw
   0 - normal game result, displayed on stdout as BW (black wins) or WW (white wins)
       (capture of all opponent's pieces, or opponent has no legal move)
   1-19 - loss due to an attempt to make an illegal move
   20 - loss due to not making a move within the allotted time
   21 - loss due to an attempt to move out of turn
   22 - loss due to closing the connection with the broker
   40 - draw - 20 moves by kings without a capture
   41 - draw - exceeded game time
*/

#define DRAW_LIMIT 20
struct timeval timeout = { 10, 0 };


int errno;
void show_board_state();
int make_move(int player, int from, int to, int CAPTURE);
int can_capture(int field);
int can_move(int field);
int check_lostgame(int player);


int main(int argc, char *argv[])
{
  char buf[BUFSPACE], player_bl[BUFSPACE], player_wh[BUFSPACE];
  int black_serv_sock, black_cli_sock, white_serv_sock, white_cli_sock,
    sock, sock2, maxsock;
  int black_port_num, white_port_num;
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t addr_len;

  fd_set readsocks, testsocks;
  int i, j, pid1, pid2, msg_len, player, res, move_num, draw_counter;
  int move_from, move_to, move_to2, nchars, offset;

  fprintf(stderr,
	  "Broker program for english draughts.\n");


  if (argc!=3) 
  {
    fprintf(stderr, "%s: Error in invocation - there must be two arguments (%d).\n",
	    argv[0], argc);
    exit(-1);
  }

  strcpy(player_bl, argv[1]);
  strcpy(player_wh, argv[2]);
  fprintf(stderr, "Player_Bl=\"%s\", Player_Wh=\"%s\"\n", player_bl, player_wh);


  for (i=0; i<10; ++i) {
    for (j=0; j<10; ++j) {
      if (fields[i][j] == -2) {	
	if ((i!=0)&&(i!=9)&&(j!=0)&&(j!=9))
	  fprintf(stderr,
		  "Invalid field value -2 not on the edge [i,j]=%d,%d\n",
		  i, j);
      } else if (fields[i][j] == -1) {	
	if ((i%2)!=(j%2)) {
	  fprintf(stderr,
		  "Invalid field value -1 not on an odd white square [i,j]=%d,%d\n",
		  i, j);
	}
      } else {	
	if (ver[fields[i][j]] != i)
	  fprintf(stderr,
		  "The vertical coordinate does not match i=%d, hor[%d]=%d !!\n",
		  i, fields[i][j], ver[fields[i][j]]);
	if (hor[fields[i][j]] != j)
	  fprintf(stderr,
		  "The horizontal coordinate does not match j=%d, hor[%d]=%d !!\n",
		  j, fields[i][j], hor[fields[i][j]]);
      }
    }
  }

  for (i=0; i<10; ++i) 
  {
    
    for (j=0; j<10; ++j) {
      sprintf(buf, "%2d", fields[i][j]);
      fprintf(stderr, "%3s", fields[i][j]==-2?"**":fields[i][j]==-1?"  ":buf);
      
    }
    fprintf(stderr, "\n");
  }


  black_serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  white_serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  if ((black_serv_sock < 0)||(white_serv_sock < 0)) {
    perror("socket black/white");
    exit(-1);
  }


  bzero((char*)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(0);
  addr_len = sizeof(serv_addr);

  if (bind(black_serv_sock, (struct sockaddr *) &serv_addr, addr_len) < 0) {
    perror("bind1");
    exit(-1);
  }

  if (getsockname(black_serv_sock, (struct sockaddr *) &serv_addr, &addr_len)) {
    perror("getsockname black");
    exit(-1);
  }
  black_port_num = ntohs(serv_addr.sin_port);
  fprintf(stderr, "Number of port for black: %d\n", black_port_num);

  if (listen(black_serv_sock, 1) < 0) {
    perror("listen black");
    exit(-1);
  }

  bzero((char*)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(0);
  addr_len = sizeof(serv_addr);
  if (bind(white_serv_sock, (struct sockaddr *) &serv_addr, addr_len) < 0) {
    perror("bind2");
    exit(-1);
  }
  if (getsockname(white_serv_sock, (struct sockaddr *) &serv_addr, &addr_len)) {
    perror("getsockname white");
    exit(-1);
  }
  white_port_num = ntohs(serv_addr.sin_port);
  fprintf(stderr, "Number of port for white: %d\n", white_port_num);

  if (listen(white_serv_sock, 1) < 0) {
    perror("listen white");
    exit(-1);
  }


  if (strcmp(player_bl, "-") != 0) {
    if ((pid1=fork())==0) {
      close(black_serv_sock);
      close(white_serv_sock);
      sprintf(buf, "%d", black_port_num);
      execl(player_bl,player_bl,"NET","BLACK","8","1291","localhost",buf,NULL);
      perror("execl1");
      exit(-1);
    }
    fprintf(stderr, "Process for black started pid=%d.\n", pid1);
  }
  if (strcmp(player_wh, "-") != 0) 
  {
    if ((pid2=fork())==0) 
    {
      close(black_serv_sock);
      close(white_serv_sock);
      sprintf(buf, "%d", white_port_num);
      execl(player_wh,player_wh,"NET","WHITE","8","15031","localhost",buf,NULL);
      perror("execl2");
      exit(-1);
    }
    fprintf(stderr, "Process for white started pid=%d.\n", pid2);
  }
	    
  fprintf(stderr, "Waiting for players to connect.\n");
  int twoplayers = 0; 
  FD_ZERO(&readsocks);
  FD_SET(black_serv_sock, &readsocks);
  FD_SET(white_serv_sock, &readsocks);
  maxsock = MAX(black_serv_sock, white_serv_sock) + 1;
  while (twoplayers<2) {
    testsocks = readsocks;
    if ((res=select(maxsock, &testsocks, (fd_set *) 0, (fd_set *) 0,
		    (struct timeval *) 0)) < 0) { /* tu czekamy do skutku */
      perror("select serv_sock");
      exit(-1);
    }
    if (FD_ISSET(black_serv_sock, &testsocks)) {
      addr_len = sizeof(cli_addr);
      black_cli_sock = accept(black_serv_sock, (struct sockaddr *) &cli_addr, &addr_len);
      if (black_cli_sock == -1) {
	perror("accept black");
	exit(-1);
      }
      fprintf(stderr, "Black has failed to connect.\n");
      FD_CLR(black_serv_sock, &readsocks);
      ++twoplayers;
    }
    if (FD_ISSET(white_serv_sock, &testsocks)) {
      addr_len = sizeof(cli_addr);
      white_cli_sock = accept(white_serv_sock, (struct sockaddr *) &cli_addr, &addr_len);
      if (white_cli_sock == -1) {
	perror("accept white");
	exit(-1);
      }
      fprintf(stderr, "White has connected.\n");
      FD_CLR(white_serv_sock, &readsocks);
      ++twoplayers;
    }
  }
  
  fprintf(stderr, "The game begins, black moves first.\n");
  show_board_state();
  FD_ZERO(&readsocks);
  FD_SET(black_cli_sock, &readsocks);
  FD_SET(white_cli_sock, &readsocks);
  maxsock = MAX(black_cli_sock, white_cli_sock) + 1;
  move_num = 0;
  draw_counter = 0;
  while(1) {

    ++move_num;
    ++draw_counter;
    if (draw_counter > DRAW_LIMIT) {
      fprintf(stderr,"Move limit exceeded for kings without progress - draw.\n");
      printf("DRAW\n");
      exit(40);	
    }

    
    player = move_num%2;

    testsocks = readsocks;
    struct timeval timeout2 = timeout;
    if ((res=select(maxsock, &testsocks, (fd_set *) 0, (fd_set *) 0,
		    (struct timeval *) &timeout2)) < 0) {
      perror("select cli_sock");
      exit(-1);
    }
    if (res>1) {
      fprintf(stderr, "Function select has returned: %d (>1) - co robic??\n",
	      res);
    }
    if (res==0) { 
      fprintf(stderr, "Player %s did not make a move on time, lost, game over.\n",
	      playername[player]);
      printf("%s\n", winnername[OPPONENT(player)]);
      exit(20); 
    }

    if (FD_ISSET(black_cli_sock, &testsocks))
      { sock = black_cli_sock;
	sock2 = white_cli_sock; }
    else
    if (FD_ISSET(white_cli_sock, &testsocks))
      { sock = white_cli_sock;
        sock2 = black_cli_sock; }
    else {
      fprintf(stderr, "The select function returned, but it's not from any of the clients.\n");
      exit(-1);
    }


    if (((sock==black_cli_sock)&&(player==WHITE)) ||
	((sock==white_cli_sock)&&(player==BLACK))) {

      fprintf(stderr, "%s made a move out of turn - loses.\n",
	      playername[OPPONENT(player)]);
      printf("%s\n", winnername[player]);
      exit(21); 
    }


    if ((msg_len=read(sock, buf, sizeof buf)) < 0) {
      perror("read move");
      exit(-1);
    }

    if (msg_len==0) {
      fprintf(stderr, "Player %s closed connection, lost, game over.\n",
	      playername[player]);
      printf("%s\n", winnername[OPPONENT(player)]);
      exit(22); 
    }


    buf[msg_len] = 0;
    fprintf(stderr, "%6s: %s", playername[player], buf);
    if (buf[msg_len-1] != '\n') fprintf(stderr,"\n");


    if (2==sscanf(buf, "%d-%d", &move_from, &move_to)) {
      fprintf(stderr, "        move from field %d to %d\n", move_from, move_to);

      for (i=1; i<=32; ++i) {
	int piece = board[ver[i]][hor[i]];
	if (IS_PIECE(piece)&&(player==OWNER(piece)))
	  if (1==can_capture(i)) {
	    fprintf(stderr,
		    "Illegal ordinary move by player %s, possible capture in square %d, game over.\n",
		    playername[player], i);
	    printf("%s\n", winnername[OPPONENT(player)]);
	    exit(9); 
	  }
      }

      if (((res=make_move(player, move_from, move_to, 0))%100)!=0) {
	fprintf(stderr, "Illegal ordinary move, code %d by player %s, game over.\n",
		res, playername[player]);
	printf("%s\n", winnername[OPPONENT(player)]);
	exit(res);
      }


      if (IS_MAN(board[ver[move_to]][hor[move_to]]))
	draw_counter = 0;
    } 


    else if (2==sscanf(buf, "%dx%d%n", &move_from, &move_to, &nchars)) {
      fprintf(stderr, "        capture from field %d to %d\n", move_from, move_to);
      if (((res=make_move(player, move_from, move_to, 1))%100)!=0) {
	fprintf(stderr, "Illegal capture, code %d by player %s, game over.\n",
		res, playername[player]);
	printf("%s\n", winnername[OPPONENT(player)]);
	exit(res);
      }
      offset=nchars;


      while (1==sscanf(buf+offset, "x%d%n", &move_to2, &nchars)) {
	fprintf(stderr, "        further capture from field %d na %d\n",
		move_to, move_to2);

	if (res==100) { 
	  fprintf(stderr,
		  "Illegal attempt to continue capture after pawn promotion, %s loses.\n",
		  playername[player]);
	  printf("%s\n", winnername[OPPONENT(player)]);
	  exit(10); 
	}
	if (((res=make_move(player, move_to, move_to2, 1))%100)!=0) {
	  fprintf(stderr,
		  "Illegal continued capture, code %d by player %s, game over.\n",
		  res, playername[player]);
	  printf("%s\n", winnername[OPPONENT(player)]);
	  exit(res);
	}
	offset+=nchars;
	move_to = move_to2;
      }	


      if ((1==can_capture(move_to))&&(res!=100)) 
      { 
	                               
	fprintf(stderr,
		"Illegal capture by player %s, further capture possible, game over.\n",
		playername[player]);
	printf("%s\n", winnername[OPPONENT(player)]);
	exit(11); 
      }

     
      draw_counter = 0;
    } 

    show_board_state();

    if (1==check_lostgame(OPPONENT(player))) {
      fprintf(stderr, "Player %s has 0 pieces and lost.\n",
	      playername[OPPONENT(player)]);
      printf("%s\n", winnername[player]);
      exit(0); 
    }

    int is_stuck=1;
    for (i=1; i<=32; ++i) {
      int piece = board[ver[i]][hor[i]];
      if (IS_PIECE(piece)&&(OPPONENT(player)==OWNER(piece)))
	if ((1==can_move(i))||(1==can_capture(i))) { is_stuck = 0; break; }
    }
    if (1==is_stuck) {
      fprintf(stderr,
	      "Player %s has no legal moves or captures left, game over.\n",
	      playername[OPPONENT(player)]);
      printf("%s\n", winnername[player]);
      exit(0); 
    }


    if (write(sock2, buf, msg_len) < 0) {
      perror("write move");
      exit(-1);
    }
  } 
} 

void show_board_state()
{
  for (int i=0; i<10; ++i) 
  {
    for (int j=0; j<10; ++j) 
    {
      fprintf(stderr, "%3s", symbols[board[i][j]-50]);
    }
    fprintf(stderr, "\n");
  }
}

int make_move(int player, int from, int to, int CAPTURE)
{
  int result, piece_mid, 
    piece_from=board[ver[from]][hor[from]],
    field_to=board[ver[to]][hor[to]];

  if ((from<1)||(from>32)) return 1;
  if ((to<1)||(to>32)) return 2;
  if (( ! IS_PIECE(piece_from)) || (OWNER(piece_from) != player)) return 3;
  if (field_to != EMPTY_F) return 4;
  if ((CAPTURE==0)&&((abs(ver[from]-ver[to])!=1)||(abs(hor[from]-hor[to])!=1)))
    return 5;
  if ((CAPTURE==1)&&((abs(ver[from]-ver[to])!=2)||(abs(hor[from]-hor[to])!=2)))
    return 6;
  if (CAPTURE==1) {
    piece_mid=board[(ver[from]+ver[to])/2][(hor[from]+hor[to])/2];
    if ((!IS_PIECE(piece_mid))||(OWNER(piece_mid)!=OPPONENT(player))) return 7;
  }
  if (IS_MAN(piece_from)) {
    if (((player==BLACK)&&((to-from)<3)) ||
	((player==WHITE)&&((from-to)<3))) 
      return 8;
  }


  result = 0;
  if (IS_MAN(piece_from) &&
      (((player==BLACK)&&(ver[to]==8)) ||
       ((player==WHITE)&&(ver[to]==1))))
    result += 100; 


  board[ver[to]][hor[to]] = board[ver[from]][hor[from]];
  if (result==100) board[ver[to]][hor[to]] += 2;
  board[ver[from]][hor[from]] = EMPTY_F;
  if (CAPTURE==1) {
    board[(ver[from]+ver[to])/2][(hor[from]+hor[to])/2] = EMPTY_F;
  }

  return result;
}

int can_capture(int field)
{
  int vert=ver[field], horz=hor[field], piece=board[vert][horz], vert2, vert3;

  
  if (OWNER(piece)==BLACK) 
  { 
    vert2 = vert+1; vert3 = vert+2;
    if ((vert3<=8)&&((horz-2)>=1)) 
      if ((board[vert3][horz-2]==EMPTY_F) &&
	  IS_PIECE(board[vert2][horz-1]) &&
	  (OWNER(board[vert2][horz-1])==WHITE)) return 1;
    if ((vert3<=8)&&((horz+2)<=8)) 
      if ((board[vert3][horz+2]==EMPTY_F) &&
	  IS_PIECE(board[vert2][horz+1]) &&
	  (OWNER(board[vert2][horz+1])==WHITE)) return 1;
  }
  if (OWNER(piece)==WHITE) { 
    vert2 = vert-1; vert3 = vert-2;
    if ((vert3>=1)&&((horz-2)>=1)) 
      if ((board[vert3][horz-2]==EMPTY_F) &&
	  IS_PIECE(board[vert2][horz-1]) &&
	  (OWNER(board[vert2][horz-1])==BLACK)) return 1;
    if ((vert3>=1)&&((horz+2)<=8)) 
      if ((board[vert3][horz+2]==EMPTY_F) &&
	  IS_PIECE(board[vert2][horz+1]) &&
	  (OWNER(board[vert2][horz+1])==BLACK)) return 1;
  }
  if (IS_MAN(piece)) return 0; 


  if (OWNER(piece)==WHITE) { 
    vert2 = vert+1; vert3 = vert+2;
    if ((vert3<=8)&&((horz-2)>=1)) 
      if ((board[vert3][horz-2]==EMPTY_F) &&
	  IS_PIECE(board[vert2][horz-1]) &&
	  (OWNER(board[vert2][horz-1])==BLACK)) return 1;
    if ((vert3<=8)&&((horz+2)<=8)) 
      if ((board[vert3][horz+2]==EMPTY_F) &&
	  IS_PIECE(board[vert2][horz+1]) &&
	  (OWNER(board[vert2][horz+1])==BLACK)) return 1;
  }
  if (OWNER(piece)==BLACK) 
  {
    vert2 = vert-1; vert3 = vert-2;
    if ((vert3>=1)&&((horz-2)>=1)) 
      if ((board[vert3][horz-2]==EMPTY_F) &&
	  IS_PIECE(board[vert2][horz-1]) &&
	  (OWNER(board[vert2][horz-1])==WHITE)) return 1;
    if ((vert3>=1)&&((horz+2)<=8)) 
      if ((board[vert3][horz+2]==EMPTY_F) &&
	  IS_PIECE(board[vert2][horz+1]) &&
	  (OWNER(board[vert2][horz+1])==WHITE)) return 1;
  }

  return 0; 
}

int can_move(int field)
{
  int vert=ver[field], horz=hor[field], piece=board[vert][horz];


  if (OWNER(piece)==BLACK) 
  { 
    if (board[vert+1][horz-1]==EMPTY_F) 
        return 1; 
    if (board[vert+1][horz+1]==EMPTY_F) 
        return 1; 
  }
  if (OWNER(piece)==WHITE) 
  
  { 
    if (board[vert-1][horz-1]==EMPTY_F) 
        return 1; 
    if (board[vert-1][horz+1]==EMPTY_F) 
        return 1; 
  }
  if (IS_MAN(piece)) 
    return 0; 


  if (OWNER(piece)==WHITE) 
  { 
    if (board[vert+1][horz-1]==EMPTY_F) 
        return 1; 
    if (board[vert+1][horz+1]==EMPTY_F) 
        return 1;
  }
  if (OWNER(piece)==BLACK) 
  { 
    if (board[vert-1][horz-1]==EMPTY_F) 
        return 1; 
    if (board[vert-1][horz+1]==EMPTY_F) 
        return 1; 
  }
  return 0; 
}


int check_lostgame(int player)
{
  int i, count=0, content;
  for (i=1; i<=32; ++i) {
    content=board[ver[i]][hor[i]];
    if (IS_PIECE(content)&&(OWNER(content)==player))
      ++count;
  }
  if (count==0) return 1; else return 0;
}