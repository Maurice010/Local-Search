#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

class State {
public:
    vector<int> state;
    vector<vector<int>> board;
    State(const vector<int>& state, const vector<vector<int>>& board) : state{state}, board{board} {}

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
    State currentState;

    Problem(const State& initialState) : currentState{initialState} {}

    void showBoard(vector<vector<int>>& board) {
        int size = board.size();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    int objectiveFunction(State& currentState) {
        vector<vector<int>> board = currentState.board;
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

    State getNextState(State& currentState) {
        int size = currentState.board.size();
        int tmpObjValue = objectiveFunction(currentState);

        State nextState(currentState.state, currentState.board);
        State tmpState(currentState.state, currentState.board);

        for(int i = 0; i < size; i++) {
            for(int j = 0; j < size; j++) {
                if(j != currentState.state[i]) {
                    nextState.state[i] = j;
                    nextState.board[nextState.state[i]][i] = 1;
                    nextState.board[currentState.state[i]][i] = 0;

                    int nextObjValue = objectiveFunction(nextState);

                    if(nextObjValue <= tmpObjValue) {
                        tmpObjValue = nextObjValue;
                        tmpState = nextState;
                    }

                    nextState.board[nextState.state[i]][i] = 0;
                    nextState.state[i] = currentState.state[i];
                    nextState.board[currentState.state[i]][i] = 1;
                }
            }
        }

        return tmpState;
    }
};

void hillClimb(Problem& problem) {
    int iteration = 0;
    while(iteration <= 1000) {
        State newState = problem.getNextState(problem.currentState);
        if(newState.state == problem.currentState.state) {
            cout << "Finished after: " << iteration << " iterations." << endl;
            problem.showBoard(problem.currentState.board);
            break;
        } else if(problem.objectiveFunction(newState) == problem.objectiveFunction(problem.currentState)) {
            tuple<int, int, int> coord = problem.currentState.generateRandState();
            problem.currentState.board[get<0>(coord)][get<2>(coord)] = 1;
            problem.currentState.board[get<0>(coord)][get<1>(coord)] = 0;
        }
        problem.currentState = newState;
        iteration++;
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

    vector<int> state(chessboard.size());
    state[0] = 2;
    state[1] = 1;
    state[2] = 2;
    state[3] = 1;

    State initialState(state, chessboard);

    Problem test(initialState);
    cout << "Initial objective function value: " << test.objectiveFunction(initialState) << endl;
    test.showBoard(test.currentState.board);
    hillClimb(test);

    return 0;
}