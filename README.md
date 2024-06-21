
## Project description:
This project implements a Checkers game where an AI can play against a human or another AI using a client and broker . The AI uses an alpha-beta pruning algorithm to evaluate the best possible moves. The project includes functionality for evaluating the board state, generating possible moves, and making moves on the board.
 

## Project Structure
* **main.cpp**: Contains the main function of the program.
* **CheckersBoard.h / CheckersBoard.cpp**: Define the CheckersBoard class, which includes methods for generating moves, making moves, undoing moves, and evaluating the board state.
* **ComputerPlayer.h / ComputerPlayer.cpp**: Define the ComputerPlayer class, which includes methods for the AI to make moves using the alpha-beta pruning algorithm.
* **Humanplayer.h / Humanplayer.cpp**: Define the HumanPlayer class, which includes methods for handling human player moves and input validation.
* **Controller.h / Controller.cpp**: Define the Controller class, which manages the game flow, coordinates between players, and handles the game state.

## Compiling Project
To compile the project, navigate to the project directory and use the following command:
```
g++ -std=c++14 -o prog main.cpp CheckersBoard.cpp HumanPlayer.cpp ComputerPlayer.cpp
```

## Usage
To execute the driver program, use the following command:
```
./checkers interface turn depth [random_seed] [ip-address] [ip-port]
```
- **interface**: Chooses the mode of operation between GUI and NET.
    - `GUI`: Interactive play on the terminal against a human.
    - `NET`: Network play via a broker program. A simple broker program will be provided later along with a sample network interface code.
- **turn**: Selects the role of the player controlled by the program, either `WHITE` or `BLACK`.
    - If the argument value is `BLACK`, the program plays as black and makes the first move. In this case, the program should immediately generate and display its first move on the board in GUI mode or send it to the network interface.
    - If the argument value is `WHITE`, the program plays as white and waits for the partner's move—received either from the GUI or the network interface—before making its move.
- **depth**: Specifies the depth of analysis for the MinMax algorithm. The program should build a tree of all possible states resulting from `depth` allowable moves by both players (i.e., `depth/2` moves by each player).
- **random_seed** (Optional): If provided, sets the seed for the random number generator to initialize the pseudorandom number generator for generating moves. This means that multiple runs of the program with the same seed value (and the same `turn` parameter) will result in identical program moves, as long as the opponent also makes the same moves as before. If this parameter is not provided, the seed should be generated randomly to ensure that each run of the program has a very high probability of using a different seed.
- **ip-address**, `ip-port` (Optional): Specify the internet address and port number of the game broker to connect to in order to send and receive moves. These parameters are only meaningful if the `interface` parameter is `NET`. If these parameters are not provided, the default value for `ip-address` is `localhost`, and the default value for `ip-port` is `12345`.

## Game Comntrols 
Enter moves in the format: source-destination or sourcexdestination for jumps. For example, 12 16 for a normal move or 19 24 for a capturing move. Numbers of fields are included in displayed board.

## Error Handling
The program includes basic error handling. If the number of arguments provided is less than 4, it will display a usage message.

## Author
This program was written by Aleksander Lyskawa.

## Future Enhancements

Implement a graphical user interface (GUI) for a more interactive experience.
Improve the AI's evaluation function for more strategic play.



