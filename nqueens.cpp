#include <iostream>
#include <vector>

using namespace std;

class Problem {
private:
    vector<vector<int>> startState;
public:
    // startState
    Problem(vector<vector<int>> startState) {
        startState = startState;
    }

    // goalStateTest
    int conflictNum(vector<vector<int>> chessboard) {
        int conflicts = 0;
        int n = chessboard.size();
        int conflictsRow = 0;
        int conflictsCol = 0;

        for(int i = 0; i < n; i++) {
            conflictsRow = 0;
            conflictsCol = 0;

            for(int j = 0; j < n; j++) {
                conflictsRow += chessboard[i][j];
                conflictsCol += chessboard[j][i];
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
                conflictsDiagRight += chessboard[i][j];
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
                conflictsDiagRight += chessboard[i][j];
                j--;
                i++;
            }
            conflicts += (conflictsDiagRight * (conflictsDiagRight - 1)) / 2;
        }

        for(int k = n - 1; k >= 0; k--) {
            conflictsDiagLeft = 0;
            int i = k;
            for(int j = 0; j < n - k; j++) {
                conflictsDiagLeft += chessboard[i][j];
                i++;
            }
            conflicts += (conflictsDiagLeft * (conflictsDiagLeft - 1)) / 2;
        }

        for(int k = 1; k < n; k++) {
            conflictsDiagLeft = 0;
            int i = 0;
            for(int j = k; j < n; j++) {
                conflictsDiagLeft += chessboard[i][j];
                i++;
            }
            conflicts += (conflictsDiagLeft * (conflictsDiagLeft - 1)) / 2;
        }

        return conflicts;
    }

    // getNextStates
};

class State {

};


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
    Problem test(chessboard);
    cout << test.conflictNum(chessboard) << endl;

    return 0;
}