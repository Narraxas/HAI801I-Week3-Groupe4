#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <ctime>
#include <algorithm>
using namespace std;

int occurences = 0;
int nul = 0;
int vX = 0;
int vO = 0;
int noeuds = 0;

bool isFull(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == ' ') return false;
        }
    }
    return true;
}

int evaluate(const vector<vector<char>>& board, int N) {
    for (int i = 0; i < N; ++i) {
        bool rowWin= true , colWin = true;
        for (int j = 0; j < N; ++j) {
            if(board[i][j] != board[i][0]) rowWin=false;
            if(board[j][i] != board[0][j]) colWin=false;
        }
        if(rowWin){
            if(board[i][0] == 'X') return 1;
            else return -1;
        }
        if(colWin){
            if(board[0][i] == 'X') return 1;
            else return -1;
        }
    }

    bool diag1WinO=true , diag1WinX=true , diag2WinO=true , diag2WinX=true;
    for (int i = 0; i < N; ++i) {
        if (board[i][i] != 'X') diag1WinX = false;
        if (board[i][i] != 'O') diag1WinO = false;
        if (board[i][N - 1 - i] != 'X') diag2WinX = false;
        if (board[i][N - 1 - i] != 'O') diag2WinO = false;
    }
    if (diag1WinX || diag2WinX) return 1;
    if (diag1WinO || diag2WinO) return -1;

    return 0;  
}

int minimax(vector<vector<char>>& board, int depth, bool is_maximizing, int N, int alpha, int beta) {
    int score = evaluate(board, N);
    noeuds++;
    if (score != 0) return score;
    if (isFull(board)) return 0;

    if (is_maximizing) {
        int best_score = INT_MIN;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    score = minimax(board, depth + 1, false, N, alpha, beta);
                    board[i][j] = ' ';
                    best_score = max(score, best_score);
                    alpha = max(alpha, best_score);
                    if (beta <= alpha) {
                        return best_score;
                    }
                }
            }
        }
        return best_score;
    } else {
        int best_score = INT_MAX;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    score = minimax(board, depth + 1, true, N, alpha, beta);
                    board[i][j] = ' ';
                    best_score = min(score, best_score);
                    beta = min(beta, best_score);
                    if (beta <= alpha) {
                        return best_score; 
                    }
                }
            }
        }
        return best_score;
    }
}


int main() {
    clock_t start_time = clock();

    ifstream file("dataset.txt");
    if (!file.is_open()) {
        cerr << "Erreur lors de l'ouverture du fichier." << endl;
        return 1;
    }

    string line;
    int N = 3;
    if (getline(file, line)) {
        N = stoi(line);
        cout << "Taille de la grille: " << N << "x" << N << endl;
    }

    while (getline(file,line)) {
        vector<vector<char>> board(N, vector<char>(N, ' '));

        char player = line[0];
        int index = 1;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N && index < line.size(); ++j) {
                board[i][j] = line[index++]; 
            }
        }

        cout << "Évaluation de la grille pour le joueur " << player << endl;
        int result = minimax(board, 0, player == 'X', N, INT_MIN, INT_MAX);

        if (result == 1) {
            cout << "Victoire pour " << player << endl;
            occurences++;
            if (player == 'X') vX++;
            else vO++;
        } else if (result == -1) {
            cout << "Défaite pour " << player << endl;
            occurences++;
            if (player == 'O') vX++;
            else vO++;
        } else {
            cout << "Match nul pour " << player << endl;
            nul++;
            occurences++;
        }
    }

    file.close();

    double execution_time = static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC;
    cout << "Temps d'exécution : " << execution_time << " secondes" << endl;
    cout << "Nb d'occurences traitées : " << occurences << endl;
    cout << "Nb de victoires pour X : " << vX << endl;
    cout << "Nb de victoires pour O : " << vO << endl;
    cout << "Nb de matchs nuls : " << nul << endl;
    cout << "Nb de noeuds visités : " << noeuds << endl;

    return 0;
}

