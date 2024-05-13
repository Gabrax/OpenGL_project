#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <cmath>

#include "Shader.h"
#include "Input.h"
#include "GameMap.h"
#include "Title.h"
#include "Select.h"
#include "Player.h"
#include "Enemy.h"
#include "WinBox.h"
#include "ChooseMode.h"
#include "modeBorder.h"
#include "Background.h"

namespace Logic {

    
    constexpr float changeX = 0.59f;
    constexpr float changeY = 1.07f;
    constexpr int BOARD_SIZE = 3;
    bool isEnd = false;
    bool PvPmode = false;
    bool PvEmode = false;

    std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> board;

    struct Game {

        Game() : isPlayerTurn(true) {
            board = createEmptyBoard();
        }

        void initialize() {
            map.BindAndLoad();
            title.BindAndLoad();
            border.BindAndLoad();
            mode.BindAndLoad();
            modeBorder.BindAndLoad();
            box.BindAndLoad();
        }

        void update() {
            
            title.Render();
            title.Delete();

            if (!title.render) {
                // MENU //

                mode.Render();
                modeBorder.Render();
                handleBorderInput();
                //std::cout << modeBorder.getNewY() << '\n';

                // MENU // 
                if(PvPmode){
                    // PVP PVP PVP //

                    //std::cout << border.getNewX() << " " << border.getNewY() << '\n';

                    map.Render();
                    border.Render();
                    if(!isEnd){
                        PVPhandlePlayersInput();
                    }
                    
                    InstPlay.RenderAllPlayers();
                    InstEnem.RenderAllenemies();

                    score = countPlayers(board);

                    char Pwins = checkifPwins(board);
                    if(Pwins == 'P'){
                        isEnd = true;
                        
                        box.Pwin = true;
                        box.Render();
                    }
                    
                    char Ewins = checkifEwins(board);
                    if(Ewins == 'E'){
                        isEnd = true;

                        box.Ewin = true;
                        box.Render();
                    }

                    if(Pwins != 'P' && Ewins != 'E' && score == 9){
                        isEnd = true;

                        box.Draw = true;
                        box.Render();
                    }
                    
                    if(Input::KeyPressed(GAB_KEY_R)){
                        ResetGame(isEnd);
                    } else if(Input::KeyPressed(GAB_KEY_RIGHT_CONTROL)){
                        goBack();
                    }
                    // PVP PVP PVP // 
                }else if(PvEmode){
                    // PVE PVE PVE //
                    //std::cout << "PVE\n";
                    map.Render();
                    border.Render();
                    if(!isEnd){
                        PVEhandlePlayersInput();
                    }
                    
                    InstPlay.RenderAllPlayers();
                    InstEnem.RenderAllenemies();

                    score = countPlayers(board);

                    char Pwins = checkifPwins(board);
                    if(Pwins == 'P'){
                        isEnd = true;
                        
                        box.Pwin = true;
                        box.Render();
                    }
                    
                    char Ewins = checkifEwins(board);
                    if(Ewins == 'E'){
                        isEnd = true;

                        box.Ewin = true;
                        box.Render();
                    }

                    if(Pwins != 'P' && Ewins != 'E' && score == 9){
                        isEnd = true;

                        box.Draw = true;
                        box.Render();
                    }
                    
                    if(Input::KeyPressed(GAB_KEY_R)){
                        ResetGame(isEnd);
                    } else if(Input::KeyPressed(GAB_KEY_RIGHT_CONTROL)){
                        goBack();
                    } 
                    // PVE PVE PVE //
                }
            }
        }

        void printTuple() {
            std::cout << "-----------" << '\n';
            for (const auto& tuple : check) {
            
                char ch = std::get<0>(tuple);
                float val1 = std::get<1>(tuple);
                float val2 = std::get<2>(tuple);

                std::cout << ch << " " << val1 << " " << val2 << '\n';
            }
        }
        
            void printBoard(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        std::cout << board[i][j];
                        if (j < BOARD_SIZE - 1) {
                            std::cout << " | ";
                        }
                    }
                    std::cout << std::endl;
                    if (i < BOARD_SIZE - 1) {
                        std::cout << "---------" << std::endl;
                    }
                }
            }

            

    private:

        std::vector<std::tuple<char,float,float>> check;
        std::vector<std::pair<float,float>> mapCoord = {
            {-1.0f, 0.53f},  {-0.35f, 0.53f},  {0.3f, 0.53f},
            {-1.0f, -0.64f}, {-0.35f, -0.64f}, {0.3f, -0.64f},
            {-1.0f, -1.81f}, {-0.35f, -1.81f}, {0.3f, -1.81f}
        };
        
        GameMap map;
        StartScreen title;
        SelectBorder border;
        InstantiatePlayer InstPlay; 
        InstantiateEnemy InstEnem; 
        ChooseMode mode;
        modeBorder modeBorder; 
        WinBox box;
        bool isPlayerTurn;
        unsigned int score;


            // MENU INPUT //     
            void handleBorderInput(){
                if (Input::KeyPressed(GAB_KEY_UP)) {
                    modeBorder.UpdatePosition(0.0f, 1.0f);
                } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
                    modeBorder.UpdatePosition(0.0f, -1.0f);
                }else if (Input::KeyPressed(GAB_KEY_ENTER)){
                    if(std::abs(modeBorder.getNewY() + 0.1f) >= 1.0f){
                        PvPmode = true;
                        map.render = true;
                    }else if(std::abs(modeBorder.getNewY() + 0.1f) < 0.001f){
                        PvEmode = true;
                        map.render = true;
                    }
                }
            }

            void goBack(){
                PvPmode = false;
                PvEmode = false;
                mode.render = true;
                modeBorder.render = true;
                map.render = false;
                isPlayerTurn = true;
                isEnd = false;
                box.Pwin = false;
                box.Ewin = false;
                box.Draw = false;
                
                check.clear();
                InstPlay.clearPlayer();
                InstEnem.clearEnemy();
                ClearBoard();
                border.ResetPosition();
            }
            // MENU INPUT //

            // PVP MODE //
            void PVPhandlePlayersInput() {
                if (isPlayerTurn) {
                    handlePlayerInput(InstPlay);
                } else {
                    handleEnemyInput(InstEnem);
                }
            }

            void handlePlayerInput(InstantiatePlayer& instPlayer) {
                if (Input::KeyPressed(GAB_KEY_UP)) {
                    border.UpdatePosition(0.0f, changeY);
                } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
                    border.UpdatePosition(0.0f, -changeY);
                } else if (Input::KeyPressed(GAB_KEY_LEFT)) {
                    border.UpdatePosition(-changeX, 0.0f);
                } else if (Input::KeyPressed(GAB_KEY_RIGHT)) {
                    border.UpdatePosition(changeX, 0.0f);
                } else if (Input::KeyPressed(GAB_KEY_X)) {
                    float newX = border.getNewX();
                    float newY = border.getNewY();

                    if (!PositionTaken(newX, newY)) {
                        check.emplace_back('P',newX, newY);
                        updateBoard(board, 'P', newX, newY);
                        Player player; 
                        player.BindAndLoad();
                        player.UpdatePositionFromBorder(newX, newY);
                        instPlayer.AddPlayer(player);

                        isPlayerTurn = !isPlayerTurn;
                    }
                }
            }

            void handleEnemyInput(InstantiateEnemy& InstEnem) {
                if (Input::KeyPressed(GAB_KEY_UP)) {
                    border.UpdatePosition(0.0f, changeY);
                } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
                    border.UpdatePosition(0.0f, -changeY);
                } else if (Input::KeyPressed(GAB_KEY_LEFT)) {
                    border.UpdatePosition(-changeX, 0.0f);
                } else if (Input::KeyPressed(GAB_KEY_RIGHT)) {
                    border.UpdatePosition(changeX, 0.0f);
                } else if (Input::KeyPressed(GAB_KEY_X)) {
                    float newX = border.getNewX();
                    float newY = border.getNewY();

                    if (!PositionTaken(newX, newY)) {
                        check.emplace_back('E',newX, newY);
                        updateBoard(board, 'E', newX, newY);
                        Enemy enemy;
                        enemy.BindAndLoad();
                        enemy.UpdatePositionFromBorder(newX, newY);
                        InstEnem.AddEnemy(enemy);

                        isPlayerTurn = !isPlayerTurn;
                    }
                }
            }
            // PVP MODE //

            // PVE MODE //

            void PVEhandlePlayersInput() {
                if (isPlayerTurn) {
                    handlePlayerInput(InstPlay);
                } else {
                    handleAiInput(InstEnem);
                }
            }

            void handleAiInput(InstantiateEnemy& InstEnem) {
                if(!isPlayerTurn){
                    float newX, newY;
                    bool positionFound = false;

                    while (!positionFound) {
                        int randomIndex = rand() % mapCoord.size();
                        newX = mapCoord[randomIndex].first;
                        newY = mapCoord[randomIndex].second;

                        std::cout << newX << " "<< newY << '\n';

                        if (!PositionTaken(newX, newY)) {
                            positionFound = true;
                            check.emplace_back('E', newX, newY);
                            updateBoard(board, 'E', newX, newY);
                            Enemy enemy;
                            enemy.BindAndLoad();
                            enemy.UpdatePositionFromBorder(newX, newY);
                            InstEnem.AddEnemy(enemy);

                            isPlayerTurn = !isPlayerTurn;
                        }
                    }
                }
            }


            // PVE MODE // 

            // BOARD LOGIC //
            void ResetGame(bool reset){
                if(reset = true){
                    isPlayerTurn = true;
                    isEnd = false;
                    board = createEmptyBoard();
                    check.clear();
                    InstEnem.Delete();
                    InstPlay.Delete();
                }
            }

            bool PositionTaken(float x, float y) {
                for (const auto& i : check) {
                    float val1 = std::get<1>(i);
                    float val2 = std::get<2>(i);
                    if (val1 == x && val2 == y) {
                        return true;
                    }
                }
                return false;
            }

            void updateBoard(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board, char player, float x, float y) {
                // Find the closest coordinates in mapCoord
                float minDist = std::numeric_limits<float>::max();
                int closestIndex = -1;
                for (size_t i = 0; i < mapCoord.size(); ++i) {
                    float dist = std::hypot(x - mapCoord[i].first, y - mapCoord[i].second); //Computes the square root of the sum of the squares of x and y
                    if (dist < minDist) {
                        minDist = dist;
                        closestIndex = static_cast<int>(i);
                    }
                }

                // Map the closest coordinates to the corresponding cell on the board
                if (closestIndex != -1) {
                    int row = closestIndex / BOARD_SIZE;
                    int col = closestIndex % BOARD_SIZE;
                    board[row][col] = player;
                }
            }

            unsigned int countPlayers(std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board){
                unsigned int count = 0;
                for(const auto& i : board){
                    for(char j : i){
                        if(j == 'P' || j == 'E'){
                            count++;
                        }
                    }
                }
                return count;
            }

            std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> createEmptyBoard() {
                std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE> emptyBoard;

                for (auto& row : emptyBoard) {
                    for (char& cell : row) {
                        cell = ' ';
                    }
                }
                return emptyBoard;
            }

            void ClearBoard(){
                for(auto& i : board){
                    i.fill(' ');
                }
            }

            char checkifPwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
                // Check horizontally
                for (const auto& row : board) {
                    int count = 0;
                    for (char cell : row) {
                        if (cell == 'P') {
                            count++;
                            if (count == 3) return 'P'; 
                        } else {
                            count = 0; 
                        }
                    }
                }

                // Check vertically
                for (size_t col = 0; col < BOARD_SIZE; col++) {
                    int count = 0;
                    for (size_t row = 0; row < BOARD_SIZE; row++) {
                        if (board[row][col] == 'P') {
                            count++;
                            if (count == 3) return 'P'; 
                        } else {
                            count = 0; 
                        }
                    }
                }

                // Check diagonally (top-left to bottom-right)
                for (size_t i = 0; i <= BOARD_SIZE - 3; i++) {
                    for (size_t j = 0; j <= BOARD_SIZE - 3; j++) {
                        if (board[i][j] == 'P' && board[i+1][j+1] == 'P' && board[i+2][j+2] == 'P') {
                            return 'P'; 
                        }
                    }
                }

                // Check diagonally (top-right to bottom-left)
                for (size_t i = 0; i <= BOARD_SIZE - 3; i++) {
                    for (size_t j = BOARD_SIZE - 1; j >= 2; j--) {
                        if (board[i][j] == 'P' && board[i+1][j-1] == 'P' && board[i+2][j-2] == 'P') {
                            return 'P'; 
                        }
                    }
                }

                // If no winner found
                return '\0';
            }

            char checkifEwins(const std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>& board) {
                // Check horizontally
                for (const auto& row : board) {
                    int count = 0;
                    for (char cell : row) {
                        if (cell == 'E') {
                            count++;
                            if (count == 3) return 'E'; 
                        } else {
                            count = 0; 
                        }
                    }
                }

                // Check vertically
                for (size_t col = 0; col < BOARD_SIZE; col++) {
                    int count = 0;
                    for (size_t row = 0; row < BOARD_SIZE; row++) {
                        if (board[row][col] == 'E') {
                            count++;
                            if (count == 3) return 'E'; 
                        } else {
                            count = 0; 
                        }
                    }
                }

                // Check diagonally (top-left to bottom-right)
                for (size_t i = 0; i <= BOARD_SIZE - 3; i++) {
                    for (size_t j = 0; j <= BOARD_SIZE - 3; j++) {
                        if (board[i][j] == 'E' && board[i+1][j+1] == 'E' && board[i+2][j+2] == 'E') {
                            return 'E'; 
                        }
                    }
                }

                // Check diagonally (top-right to bottom-left)
                for (size_t i = 0; i <= BOARD_SIZE - 3; i++) {
                    for (size_t j = BOARD_SIZE - 1; j >= 2; j--) {
                        if (board[i][j] == 'E' && board[i+1][j-1] == 'E' && board[i+2][j-2] == 'E') {
                            return 'E'; 
                        }
                    }
                }

                // If no winner found
                return '\0';
            }
            // BOARD LOGIC //
    };
} 
