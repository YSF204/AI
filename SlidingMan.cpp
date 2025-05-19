/*Names : 1- Yousef AL Bakri
            ID : 221037

          2- Ismail Jboor
            ID : 221051
*/
#include <iostream>
#include <vector>
#include <cstdlib>  
#include <stack>

using namespace std;
stack<char> moveStack;

const char actionChars[4] = {'W', 'S', 'D', 'A'};

struct QValues {
    double W = 0;
    double S = 0;
    double D = 0;
    double A = 0;
};

struct Position {
    int x;
    int y;
};


bool isValidMove(int x, int y, int n) {
    return (x >= 0 && x < n && y >= 0 && y < n);
}

int Reward(int x, int y, vector<vector<int>>& grid) {
    //Monster
    if (grid[x][y] == 2) {
        return -100; 
    }
    
    //GOAL
    if (grid[x][y] == 3){
        return 100;
    }

    // NOT GOAL OR MONSTER
    return 0;  
}

bool TwoStepCheck() {
    double randomNum = (rand() % 201) / 100.0; 
    if (randomNum < 0.5) {
        return true; 
    } else {
        return false; 
    }
}

Position makeMove(int currentX, int currentY, char action, int n, bool twoSteps) {
    Position newPos;

    newPos.x = currentX;
    newPos.y = currentY;
    int steps;
    if (twoSteps) {
        steps = 2;
    } else {
        steps = 1;
    }
    
    for(int i = 0; i < steps; i++) {
        int X = newPos.x;
        int Y = newPos.y;
        
        switch(action) {
            case 'W': 
            case 'w':
                X--;
                break;

            case 'S':
            case 's':
                X++;
                break;

            case 'D':
            case 'd':
                Y++;
                break;
                
            case 'A':
            case 'a':
                Y--;
                break;
        }
        
        if (isValidMove(X, Y, n)) {
            newPos.x = X;
            newPos.y = Y;
        } else {
           cout<<"You hit a wall !\n";
            break; 
        }
    }
    
    return newPos;
}

double Get_The_Max_Q_Value(int x, int y, vector<vector<QValues>>& Qtabel) {
    //SUPPOSE W IS THE MAX , THEN CHECK ALL FOR MAX
    double maxQ = Qtabel[x][y].W;  
    
    for(int i = 1; i < 4; i++) {
        char action = actionChars[i];
        if(Qtabel[x][y].W > maxQ) {
            maxQ = Qtabel[x][y].W;
        }
    }
    return maxQ;
}

void PrintGrid(vector<vector<int>>& grid) {

    int n = grid.size();

    for(int i = 0; i < n; i++) {

        for(int j = 0; j < n; j++) {
            
            cout << grid[i][j] << "    ";
        }
        cout << "\n";
    }
}


void PrintQ(vector<vector<QValues>>& Qtabel, char action, string actionN) {
    int n = Qtabel.size();

    cout << "\n=== Q-Values for " << actionN << " Action ===\n";

    for(int i = 0; i < n; i++) {

        for(int j = 0; j < n; j++) {

            double value = 0;

            switch(action) {
                case 'W': 
                    value = Qtabel[i][j].W;
                 break;
                case 'S': 
                    value = Qtabel[i][j].S;
                     break;
                case 'D': 
                     value = Qtabel[i][j].D; 
                    break;
                case 'A': 
                    value = Qtabel[i][j].A; 
                    break;
            }
            cout << value << "          ";
        }
        cout <<endl;
    }
}


int main() {
    int n;
    int monsterCount;

    cout << "Enter grid size: ";
    cin >> n;

    Position player, goal;

    cout << "Enter player position (x y): ";
    cin >> player.x >> player.y;

    cout << "Enter number of monsters: ";
    cin >> monsterCount;

    //GRID
    vector<vector<int>> grid(n, vector<int>(n, 0));
    //QTABEL
    vector<vector<QValues>> Qtabel(n, vector<QValues>(n));
    //MONSTER
    vector<Position> monsters(monsterCount);

    
    //POSITIONS FOR MONSTERS IF WE HAVE MORE THAN 1 
    for (int i = 0; i < monsterCount; ++i) {

        cout << "Enter monster " << i + 1 << " position (x y): ";

        cin >> monsters[i].x >> monsters[i].y;

        grid[monsters[i].x][monsters[i].y] = 2;




    }

    cout << "Enter goal position (x y): ";
    cin >> goal.x >> goal.y;


    grid[player.x][player.y] = 1;

    grid[goal.x][goal.y] = 3;

    // Y FROM AI SLIDES , WE CAN CHANGE IT TO WHAT EVER WE WANT
    double y = 0.8;
    bool end = false;

    cout << "\nInitial Grid:\n";
    PrintGrid(grid);

     PrintQ(Qtabel, 'W', "UP");
        PrintQ(Qtabel, 'S', "DOWN");
        PrintQ(Qtabel, 'D', "RIGHT");
        PrintQ(Qtabel, 'A', "LEFT");

    
   
    //AS LONG AS WE DIDN'T HIT A MONSTER OR WE FOUND THE GOAL
    while (!end) {
        cout << "\nYour Position is (" << player.x << "," << player.y << "):\n";
        cout << "Enter move (W=Up, S=Down, D=Right, A=Left): ";

        char action;
        cin >> action;
        moveStack.push(action);

        // WE SAVE THE OLD POSITION FOR Q TABLE 
        int prevX = player.x;
        int prevY = player.y;

        //WE CHECK IF WE TAKE ONE STEP OR TWO , IF YES -> 2 STEPS , NO -> 1 STEP
        bool twoSteps = TwoStepCheck();

        //IT WILL GET THE NEW POSITON IN STRUCT , X AND Y
        player = makeMove(player.x, player.y, action, n, twoSteps);

        //IF WE FIND GOAL , WE ARE DONE
        if (grid[player.x][player.y] == 3) {
            cout << "\nYou Reached the goal\n";
            end = true;
            break;  
        }
            // IF WE GOT HIT BY ANY MONSTER WE ARE DONE 
        for (int i = 0; i < monsterCount; ++i) {
            if (player.x == monsters[i].x && player.y == monsters[i].y) {
                cout << "\nThe monster got you !\n";
                end = true;
                break;
            }
        }

        //GET OUT OF TEH WHILE LOOP BECAUSE WE LOST OR WON 
        if (end == true ){
            break;
        }

        // UPDATING THE PREV PLAYER VALUE ON THE GRID AFTER A MOVEMENT
        if (grid[prevX][prevY] == 1) {
            grid[prevX][prevY] = 0;
            grid[player.x][player.y] = 1;
        }

        //WE GET 100 FOR GOAL OR -100 FOR MONSTER , IF WE ARE CLOSE TO IT OR HIT IT 
        double R = Reward(player.x, player.y, grid);


        double maxNextQ = Get_The_Max_Q_Value(player.x, player.y, Qtabel);

        // HERE WE UPDATE THE THE VALUE OF THE PREV POSIOTION AFTER EACH MOVE BASED ON OUR MOVE (W, S, A OR D)
        if (action == 'W' || action == 'w') {

            Qtabel[prevX][prevY].W = R + y * maxNextQ;

        } else if (action == 'S' || action == 's') {

            Qtabel[prevX][prevY].S = R + y * maxNextQ;

        } else if (action == 'D' || action == 'd') {

            Qtabel[prevX][prevY].D = R + y * maxNextQ;

        } else if (action == 'A' || action == 'a') {

            Qtabel[prevX][prevY].A = R + y * maxNextQ;

        }

        //Here we give prediction for all actions:
        // SO THE USER CAN CHOOSE THE BEST ONE FOR IT

        for(int i = 0; i < 4; i++) {
            char nextAction = actionChars[i];

            Position nextPos = makeMove(player.x, player.y, nextAction, n, false);

            if(isValidMove(nextPos.x, nextPos.y, n)) {
                double nextReward = Reward(nextPos.x, nextPos.y, grid);

                if (nextAction == 'W' || nextAction == 'w') {

                    Qtabel[player.x][player.y].W = nextReward;

                } else if (nextAction == 'S' || nextAction == 's') {

                    Qtabel[player.x][player.y].S = nextReward;

                } else if (nextAction == 'D' || nextAction == 'd') {

                    Qtabel[player.x][player.y].D = nextReward;

                } else if (nextAction == 'A' || nextAction == 'a') {

                    Qtabel[player.x][player.y].A = nextReward;
                }
            }
        }

        system("cls");

         cout << "\nInitial Grid:\n";

        PrintGrid(grid);
        PrintQ(Qtabel, 'W', "UP");
        PrintQ(Qtabel, 'S', "DOWN");
        PrintQ(Qtabel, 'D', "RIGHT");
        PrintQ(Qtabel, 'A', "LEFT");

    }
    // HERE WE PRINT THE MOVEMENT WE DID AFTER WE BREAK OUT OF THE WHILE LOOP
    cout << "Moves made: ";
    while (!moveStack.empty()) {
        cout << moveStack.top() << " ";
        moveStack.pop();
    }
    cout << endl;

    return 0;
}
