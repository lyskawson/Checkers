#include "CheckersBoard.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include <memory>


int main(int argc, const char * argv[])
{

    const int BLACK = 1;
    const int WHITE = 0 ;



    if(argc < 4 )
    {
        std::cout << "Usage: ./prog interface turn depth random_seed ip-address ip-port << std::endl";
        return 1;
    }
    std::string interface = argv[1];
    int player = -1;
    if (strcasecmp(argv[2], "BLACK") == 0)
        player = BLACK;
    if (strcasecmp(argv[2], "WHITE") == 0)
        player = WHITE;
    if (player < 0)
    {
        std::cerr << "Invalid player specification in argv[2]: " << argv[2] << std::endl;
        return -1;
    }

    int depth = atoi(argv[3]);

    int random_seed = 0;


    if (argc > 4)
        random_seed = std::atoi(argv[4]);





    if(interface=="GUI" || interface=="gui")
    {
        CheckersBoard board;
        std::unique_ptr<Player> whitePlayer;
        std::unique_ptr<Player> blackPlayer;
        if(player == WHITE)
        {
            whitePlayer = std::make_unique<ComputerPlayer>(Owner::WHITE, board);
            blackPlayer = std::make_unique<HumanPlayer>(Owner::BLACK, board);
        }
        else
        {
            whitePlayer = std::make_unique<HumanPlayer>(Owner::WHITE, board);
            blackPlayer = std::make_unique<ComputerPlayer>(Owner::BLACK, board);
        }
        int drawCounter = 0;
        int moveNum = 0;
        const int drawLimit  = 40;
        while (true)
        {
            ++moveNum;
            ++drawCounter;
            if (drawCounter > drawLimit)
            {
                std::cerr << " Exceeded limit of moves by kings without progress - draw" << std::endl;
                std::cout << "DRAW" << std::endl;
                std::exit(40);
            }
            int player = moveNum%2;


            if (player == 0)
            {
                whitePlayer->makeMove(board, depth, random_seed);

            }
            else
            {
                blackPlayer->makeMove(board, depth, random_seed);
            }

            if (board.isGameOver(1-player))
            {
                std::cout << "Winner: " << (player == 0 ? "White" : "Black") << std::endl;
                std::exit(0);
            }

            if (board.isManMoved())
            {
                drawCounter = 0;
            }
        }
    }
    else
    {
        char buf[1024];
        std::string ip_address = "localhost";
        int ip_port = 12345;

        if (argc > 5)
            ip_address = argv[5];
        if (argc > 6)
            ip_port = std::atoi(argv[6]);

        struct sockaddr_in serv_addr;
        struct hostent *serv_hostent;

        int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (serv_sock < 0)
        {
            perror("socket");
            return errno;
        }

        serv_hostent = gethostbyname(ip_address.c_str());
        if (serv_hostent == nullptr)
        {
            std::cerr << argv[0]<< " :Unknown IP address " << ip_address << std::endl;
            return -1;
        }

        serv_addr.sin_family = AF_INET;
        memcpy(&serv_addr.sin_addr, serv_hostent->h_addr, serv_hostent->h_length);
        serv_addr.sin_port = htons(ip_port);

        //std::cout << "Connecting with server ...\n";
        if (connect(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("connect");
            exit(-1);
        }
        //std::cout << "Connection established, starting game\n";
        int moveNum = 0; // blacks makes even moves
        if (player == WHITE)
            moveNum = 0; //white make odd moves

        CheckersBoard board;
        Owner playerColor = static_cast<Owner>(player);
        ComputerPlayer computer(playerColor, board);

        while(true)
        {
            if ((moveNum>0) || (player==BLACK))
            {
                //std::cout << "Sending my move to the server: " << std::endl;
                std::string aiMove = computer.sendMove(board, depth, random_seed);
                if (write(serv_sock, aiMove.c_str(), aiMove.length()) < 0)
                {
                    perror("write");
                    exit(errno);
                }
            }
            //std::cout << "Waiting for opponent's move ..." << std::endl;
            int n = read(serv_sock, buf, sizeof buf); //read returns the number of bytes read

            if (n < 0)
            {
                perror("read");
                exit(errno);
            }
            if (n == 0)
            {
                //std::cout << "Broker closed the connection  ..." << std::endl;
                exit(0);
            }
            buf[n] = 0;
            //std::cout << "Received opponent's move: " << buf;
            if (buf[n - 1] != '\n')
                std::cout << std::endl;

            if (!board.makeMoveFromNotation(buf, 1-player))
            {
                std::cerr << argv[0] << " failed to perform the received move" << std::endl;
                exit(-1);
            }
            ++moveNum;
        }
    }



    return 0;

}


/*
int main()
{
    CheckersBoard board;
    HumanPlayer blackPlayer(BLACK, board);
    ComputerPlayer whitePlayer(WHITE, board);

    int depth = 5;
    unsigned int random_seed =  1;
    int drawCounter = 0;
    int moveNum = 0;
    const int drawLimit  = 40;


    while (true)
    {
        ++moveNum;
        ++drawCounter;
        if (drawCounter > drawLimit)
        {
            std::cerr << " Exceeded limit of moves by kings without progress - draw" << std::endl;
            std::cout << "DRAW" << std::endl;
            board.displayBoard();
            std::exit(40);  // exceeded limit of moves by kings without progress - draw
        }
        int player = moveNum%2;


        if (player == 0)
        {
            whitePlayer.sendMove(board, depth, random_seed);

        }
        else
        {
            blackPlayer.makeMove(board, depth, random_seed);
        }

        if (board.isGameOver(1-player))
        {
            std::cout << "Winner: " << (player == 0 ? "White" : "Black") << std::endl;
            board.displayBoard();
            std::exit(0);  // end the game
        }

        if (board.isManMoved())
        {
            drawCounter = 0;
        }

    }
    return 0;

}
*/