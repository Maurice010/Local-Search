#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

class State {
public:
    vector<int> state;
    State(const vector<int>& state) : state{state} {}

    tuple<int, int, int> generateRandState() {
        int i = rand() % state.size();
        int prevj = state[i];
        int j = rand() % state.size();
        state[i] = j;
        return make_tuple(i, prevj, j);
    }
};

class Problem {
public:
    vector<vector<int>> currentBoard;
    State currentState;
    // startState
    Problem(const vector<vector<int>>& board, const State& initialState) : currentBoard{board},  currentState{initialState} {}

    void showBoard(vector<vector<int>>& board) {
        int size = board.size();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    // goalStateTest
    int conflictNum(vector<vector<int>> board) {
        int conflicts = 0;
        int n = board.size();
        int conflictsRow = 0;
        int conflictsCol = 0;

        for(int i = 0; i < n; i++) {
            conflictsRow = 0;
            conflictsCol = 0;

            for(int j = 0; j < n; j++) {
                conflictsRow += board[i][j];
                conflictsCol += board[j][i];
            }
            conflicts += (conflictsRow * (conflictsRow - 1)) / 2;
            conflicts += (conflictsCol * (conflictsCol - 1)) / 2;
        }

        int conflictsDiagRight = 0;
        int conflictsDiagLeft = 0;

        for(int k = 0; k < n; k++) {
            conflictsDiagRight = 0;
            int i = 0;
            int j = k;
            while(j >= 0) {
                conflictsDiagRight += board[i][j];
                i++;
                j--;
            }
            conflicts += (conflictsDiagRight * (conflictsDiagRight - 1)) / 2;
        }

        for(int k = 1; k < n; k++) {
            conflictsDiagRight = 0;
            int j = n - 1;
            int i = k;
            while(j >= k) {
                conflictsDiagRight += board[i][j];
                j--;
                i++;
            }
            conflicts += (conflictsDiagRight * (conflictsDiagRight - 1)) / 2;
        }

        for(int k = n - 1; k >= 0; k--) {
            conflictsDiagLeft = 0;
            int i = k;
            for(int j = 0; j < n - k; j++) {
                conflictsDiagLeft += board[i][j];
                i++;
            }
            conflicts += (conflictsDiagLeft * (conflictsDiagLeft - 1)) / 2;
        }

        for(int k = 1; k < n; k++) {
            conflictsDiagLeft = 0;
            int i = 0;
            for(int j = k; j < n; j++) {
                conflictsDiagLeft += board[i][j];
                i++;
            }
            conflicts += (conflictsDiagLeft * (conflictsDiagLeft - 1)) / 2;
        }

        return conflicts;
    }

    // getNextState
    // TODO: it changes current state but it would be better if it just returned
    //       the next state and let the algo do the rest
    void getNextState(vector<vector<int>>& currentBoard, State& currentState) {
        int size = currentBoard.size();
        int tmpObjValue = conflictNum(currentBoard);

        State nextState = currentState;
        vector<vector<int>> nextBoard = currentBoard;

        State tmpState = currentState;
        vector<vector<int>> tmpBoard = currentBoard;

        for(int i = 0; i < size; i++) {
            for(int j = 0; j < size; j++) {
                if(j != currentState.state[i]) {
                    nextState.state[i] = j;
                    nextBoard[nextState.state[i]][i] = 1;
                    nextBoard[currentState.state[i]][i] = 0;

                    int nextObjValue = conflictNum(nextBoard);

                    if(nextObjValue <= tmpObjValue) {
                        tmpObjValue = nextObjValue;
                        tmpState = nextState;
                        tmpBoard = nextBoard;
                    }

                    nextBoard[nextState.state[i]][i] = 0;
                    nextState.state[i] = currentState.state[i];
                    nextBoard[currentState.state[i]][i] = 1;
                }
            }
        }

        currentState = tmpState;
        currentBoard = tmpBoard;
    }
};

void hillClimb(Problem& problem) {
    while(true) {
        vector<vector<int>> currBoard = problem.currentBoard;
        State currState = problem.currentState;

        problem.getNextState(problem.currentBoard, problem.currentState);
        if(currState.state == problem.currentState.state) {
            problem.showBoard(problem.currentBoard);
            break;
        } else if(problem.conflictNum(currBoard) == problem.conflictNum(problem.currentBoard)) {
            tuple<int, int, int> coord = problem.currentState.generateRandState();
            problem.currentBoard[get<0>(coord)][get<2>(coord)] = 1;
            problem.currentBoard[get<0>(coord)][get<1>(coord)] = 0;
        }
    }
}

int main() {
    vector<vector<int>> chessboard(4, vector<int>(4, 0));
    for (int i = 0; i < chessboard.size(); i++) {
        for (int j = 0; j < chessboard.size(); j++) {
            chessboard[i][j] = 0;
        }
    }

    chessboard[2][0] = 1;
    chessboard[1][1] = 1;
    chessboard[2][2] = 1;
    chessboard[1][3] = 1;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << chessboard[i][j] << " ";
        }
        cout << endl;
    }

    vector<int> state(chessboard.size());
    state[0] = 2;
    state[1] = 1;
    state[2] = 2;
    state[3] = 1;

    State initialState(state);

    Problem test(chessboard, initialState);
    cout << test.conflictNum(chessboard) << endl;
    hillClimb(test);

    return 0;
}