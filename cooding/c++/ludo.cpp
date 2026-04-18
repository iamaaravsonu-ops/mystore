// ludo.cpp
// Console Ludo game with dice animation, AI, colored output.
// Compile: g++ -std=c++17 -O2 -pthread -o ludo ludo.cpp

#include <bits/stdc++.h>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono_literals;

static std::mt19937 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());

constexpr int MAIN_PATH = 52;
constexpr int HOME_STEPS = 6; // steps inside home column to finish
constexpr int FINISHED_STEPS = MAIN_PATH + HOME_STEPS; // 58

// ANSI colors
const string RESET = "\033[0m";
const vector<string> PLAYER_COL = {
    "\033[38;5;196m", // red
    "\033[38;5;208m", // orange
    "\033[38;5;46m",  // green
    "\033[38;5;21m"   // blue
};

struct Token {
    int stepsFromStart; // -1 = at home; 0..51 main path; 52..57 home column; 58 = finished
    Token() : stepsFromStart(-1) {}
    bool atHome() const { return stepsFromStart == -1; }
    bool finished() const { return stepsFromStart >= FINISHED_STEPS; }
    bool onMainPath() const { return stepsFromStart >= 0 && stepsFromStart < MAIN_PATH; }
    int posOnMain(int startIndex) const {
        if (!onMainPath()) return -1;
        int s = (startIndex + stepsFromStart) % MAIN_PATH;
        return s;
    }
    int inHomeIndex() const { // 0..HOME_STEPS-1
        if (stepsFromStart >= MAIN_PATH && stepsFromStart < FINISHED_STEPS)
            return stepsFromStart - MAIN_PATH;
        return -1;
    }
};

struct Player {
    string name;
    bool isHuman;
    int startIndex; // 0,13,26,39
    array<Token,4> tokens;
    int id; // 0..3
    Player() {}
    Player(int pid, string n, bool human, int start): id(pid), name(n), isHuman(human), startIndex(start) {}
    bool hasMovable(int roll) const {
        for (int i=0;i<4;i++) {
            if (canMoveToken(i, roll)) return true;
        }
        return false;
    }
    bool canMoveToken(int idx, int roll) const {
        const Token &t = tokens[idx];
        if (t.finished()) return false;
        if (t.atHome()) {
            return roll == 6;
        } else {
            int newSteps = t.stepsFromStart + roll;
            return newSteps <= FINISHED_STEPS;
        }
    }
    int completedCount() const {
        int c=0;
        for(auto &t: tokens) if(t.finished()) c++;
        return c;
    }
};

struct Game {
    array<Player,4> players;
    vector<int> safeSquares; // main path indices that are safe
    int currentPlayer;
    bool ended = false;

    Game() {
        // default names, start indexes: 0,13,26,39
        players[0] = Player(0,"Player 1", true, 0);
        players[1] = Player(1,"Player 2", false, 13);
        players[2] = Player(2,"Player 3", false, 26);
        players[3] = Player(3,"Player 4", false, 39);

        // choose safe squares: all players' startIndex + a few classic safe positions
        safeSquares = {0, 8, 13, 21, 26, 34, 39, 47};
    }

    bool isSafePosition(int pos) const {
        for (int s: safeSquares) if (s==pos) return true;
        return false;
    }

    // returns pair<playerId, tokenIdx> if captured, else (-1,-1)
    pair<int,int> tryCapture(int moverPid, int moverTokenIdx) {
        Player &p = players[moverPid];
        Token &t = p.tokens[moverTokenIdx];
        if (!t.onMainPath()) return {-1,-1};
        int pos = t.posOnMain(p.startIndex);
        if (isSafePosition(pos)) return {-1,-1};
        for (int pid=0; pid<4; pid++) {
            if (pid==moverPid) continue;
            for (int ti=0; ti<4; ti++) {
                Token &ot = players[pid].tokens[ti];
                if (ot.onMainPath() && ot.posOnMain(players[pid].startIndex) == pos) {
                    // capture
                    ot.stepsFromStart = -1;
                    return {pid, ti};
                }
            }
        }
        return {-1,-1};
    }

    void rollDiceAnimation(int &outRoll) {
        uniform_int_distribution<int> dist(1,6);
        // animation frames
        for (int i=0;i<10;i++){
            int r = dist(rng);
            cout << "\rRolling: [" << r << "] ";
            cout.flush();
            this_thread::sleep_for(60ms + chrono::milliseconds(i*10));
        }
        // final roll
        int final = dist(rng);
        cout << "\rRolling: [" << final << "]     \n";
        outRoll = final;
    }

    void displayBoard() {
        // Simple board display: show main path indices in a line with tokens
        // We'll print two rows for readability: indices 0..25 and 26..51
        auto showRow = [&](int start, int end) {
            for (int i=start;i<=end;i++){
                string slot = to_string(i);
                if (slot.size()<2) slot = " "+slot;
                cout << "+" << string(4,'-');
            }
            cout << "+\n";
            for (int i=start;i<=end;i++){
                int pos = i % MAIN_PATH;
                string cell = "    ";
                // find tokens occupying this pos
                vector<pair<int,char>> marks;
                for (int pid=0; pid<4; pid++){
                    for (int ti=0; ti<4; ti++){
                        const Token &t = players[pid].tokens[ti];
                        if (t.onMainPath() && t.posOnMain(players[pid].startIndex) == pos) {
                            char ch = 'A' + pid; // A,B,C,D for player tokens
                            marks.push_back({pid,ch});
                        }
                    }
                }
                if (!marks.empty()) {
                    string s="";
                    for (auto &m: marks) {
                        s += PLAYER_COL[m.first];
                        s += m.second;
                        s += RESET;
                    }
                    // pad
                    while ((int)s.size() < 4) s += ' ';
                    if (isSafePosition(pos)) {
                        cout << "|" << "\033[48;5;240m" << s.substr(0,4) << RESET;
                    } else {
                        cout << "|" << s.substr(0,4);
                    }
                } else {
                    if (isSafePosition(pos)) {
                        cout << "|" << "\033[48;5;240m" << "    " << RESET;
                    } else cout << "|" << "    ";
                }
            }
            cout << "|\n";
            for (int i=start;i<=end;i++){
                cout << "+" << string(4,'-');
            }
            cout << "+\n";
            // index row
            for (int i=start;i<=end;i++){
                string slot = to_string(i%MAIN_PATH);
                if (slot.size()<2) slot = " "+slot;
                cout << "|" << " " << slot << " ";
            }
            cout << "|\n";
        };

        cout << "\n----- LUDO BOARD (main path) -----\n";
        showRow(0,25);
        showRow(26,51);

        // show each player's home tokens
        cout << "\nPlayer token status:\n";
        for (int pid=0; pid<4; pid++){
            auto &p = players[pid];
            cout << PLAYER_COL[pid] << p.name << RESET << " (Start@" << p.startIndex << ") : ";
            for (int ti=0; ti<4; ti++){
                Token &t = p.tokens[ti];
                char label = 'a'+ti;
                cout << "[" << label << ":";
                if (t.atHome()) cout << "H";
                else if (t.finished()) cout << "F";
                else if (t.onMainPath()) cout << "M" << t.posOnMain(p.startIndex);
                else cout << "C" << t.inHomeIndex();
                cout << "] ";
            }
            cout << "\n";
        }
        cout << "----------------------------------\n\n";
    }

    // Move token (assume valid). Return capture info if any.
    pair<int,int> moveToken(int pid, int tid, int roll) {
        Player &p = players[pid];
        Token &t = p.tokens[tid];
        if (t.atHome()) {
            // must be roll==6
            t.stepsFromStart = 0;
        } else {
            t.stepsFromStart += roll;
            if (t.stepsFromStart > FINISHED_STEPS) {
                // shouldn't happen because we check validity, but cap
                t.stepsFromStart = FINISHED_STEPS;
            }
        }
        // check finished
        if (t.stepsFromStart >= FINISHED_STEPS) {
            t.stepsFromStart = FINISHED_STEPS;
            return {-1,-1};
        }
        // if landed on main path, try capture
        if (t.onMainPath()) {
            return tryCapture(pid, tid);
        }
        return {-1,-1};
    }

    // simple AI pick
    int aiChooseToken(int pid, int roll) {
        Player &p = players[pid];
        // 1. if can finish with a token, pick that
        for (int i=0;i<4;i++) {
            if (!p.canMoveToken(i, roll)) continue;
            Token t = p.tokens[i];
            int newSteps = (t.atHome()? 0 : t.stepsFromStart) + (t.atHome()? (roll==6?0:1000) : roll);
            // careful: if at home and roll==6 newSteps = 0 -> not finishing. Only check if after move stepsFromStart >= FINISHED_STEPS
            if (t.atHome()) {
                if (roll==6) {
                    // will move to stepsFromStart=0 -> cannot finish
                }
            } else {
                if (t.stepsFromStart + roll >= FINISHED_STEPS) return i;
            }
        }
        // 2. capture move (simulate moves and check if landing would capture)
        for (int i=0;i<4;i++) {
            if (!p.canMoveToken(i, roll)) continue;
            Token t = p.tokens[i];
            int newSteps;
            if (t.atHome()) { if (roll!=6) continue; newSteps = 0; }
            else newSteps = t.stepsFromStart + roll;
            if (newSteps < MAIN_PATH) {
                int pos = (p.startIndex + newSteps) % MAIN_PATH;
                if (!isSafePosition(pos)) {
                    // check if any opponent token is on pos
                    for (int op=0;op<4;op++) if (op!=pid) {
                        for (int ot=0;ot<4;ot++){
                            if (players[op].tokens[ot].onMainPath() && players[op].tokens[ot].posOnMain(players[op].startIndex)==pos) {
                                return i;
                            }
                        }
                    }
                }
            }
        }
        // 3. move the token that is furthest (max stepsFromStart)
        int best=-1, bestSteps=-1000;
        for (int i=0;i<4;i++) {
            if (!p.canMoveToken(i, roll)) continue;
            int s = p.tokens[i].atHome() ? -1000 : p.tokens[i].stepsFromStart;
            if (s > bestSteps) { bestSteps = s; best = i; }
        }
        if (best != -1) return best;
        // 4. if nothing else, pick first valid
        for (int i=0;i<4;i++) if (p.canMoveToken(i, roll)) return i;
        return -1;
    }

    void run() {
        while (!ended) {
            displayBoard();
            Player &p = players[currentPlayer];
            cout << PLAYER_COL[p.id] << p.name << RESET;
            if (p.isHuman) cout << " (Human)";
            cout << " -- it's your turn.\n";

            int roll=0;
            rollDiceAnimation(roll);

            bool extraTurn = false;
            bool anyValid = p.hasMovable(roll);
            if (!anyValid) {
                cout << "No valid moves.\n";
                if (roll==6) extraTurn = true;
            } else {
                int chosenToken = -1;
                if (p.isHuman) {
                    // list options
                    cout << "Roll = " << roll << ". Choose a token to move (1-4):\n";
                    for (int i=0;i<4;i++){
                        cout << (i+1) << ") ";
                        Token &t = p.tokens[i];
                        if (t.finished()) cout << "(Finished) ";
                        else if (t.atHome()) cout << "(At Home)";
                        else if (t.onMainPath()) cout << "(Main@" << t.posOnMain(p.startIndex) << ")";
                        else cout << "(HomeCol " << t.inHomeIndex() << ")";
                        if (p.canMoveToken(i, roll)) cout << " ---> Can move";
                        cout << "\n";
                    }
                    while (true) {
                        cout << "Token number (1-4): ";
                        string line;
                        if(!getline(cin,line)) exit(0);
                        if (line.empty()) continue;
                        int x = stoi(line);
                        if (x>=1 && x<=4 && p.canMoveToken(x-1, roll)) {
                            chosenToken = x-1; break;
                        } else {
                            cout << "Invalid choice or cannot move that token. Try again.\n";
                        }
                    }
                } else {
                    chosenToken = aiChooseToken(p.id, roll);
                    if (chosenToken==-1) {
                        cout << p.name << " (AI) has no move (unexpected)\n";
                    } else {
                        cout << p.name << " (AI) chooses token " << (chosenToken+1) << "\n";
                        this_thread::sleep_for(600ms);
                    }
                }

                if (chosenToken != -1) {
                    // animate token stepping one by one
                    Token &t = p.tokens[chosenToken];
                    int stepsToMove = (t.atHome() ? (roll==6?1:0) : roll);
                    // if atHome and roll==6, first step moves onto start (stepsFromStart 0).
                    // We'll simulate incremental steps for "realistic" feel.
                    if (t.atHome()) {
                        cout << "Bringing token out of home...\n";
                        this_thread::sleep_for(350ms);
                        t.stepsFromStart = 0; // we move onto starting square
                        stepsToMove = 0; // already moved the one step that gets onto board
                    } else {
                        for (int s=0;s<roll;s++){
                            t.stepsFromStart += 1;
                            // cap
                            if (t.stepsFromStart >= FINISHED_STEPS) { t.stepsFromStart = FINISHED_STEPS; break; }
                            // brief display
                            cout << "\rMoving token... step " << (s+1) << "/" << roll << "    ";
                            cout.flush();
                            this_thread::sleep_for(180ms);
                        }
                        cout << "\n";
                    }

                    // If the token just reached FINISHED_STEPS, it finished
                    if (t.finished()) {
                        cout << PLAYER_COL[p.id] << p.name << RESET << "'s token finished!\n";
                    } else {
                        // try capture after the full move
                        if (t.onMainPath()) {
                            auto cap = tryCapture(p.id, chosenToken);
                            if (cap.first != -1) {
                                cout << "Captured " << players[cap.first].name << "'s token " << (char)('a'+cap.second) << "!\n";
                            }
                        }
                    }
                }
            }

            // if rolled 6: extra turn (unless no moves? still extra turn allowed typically)
            if (roll==6) {
                cout << "Rolled a 6 — you get another turn!\n";
                extraTurn = true;
            }

            // check for victory (a player with all 4 finished)
            for (int pid=0; pid<4; pid++) {
                if (players[pid].completedCount() == 4) {
                    cout << PLAYER_COL[pid] << players[pid].name << RESET << " has finished all tokens and wins the game! 🎉\n";
                    ended = true;
                }
            }

            if (!extraTurn) {
                currentPlayer = (currentPlayer + 1) % 4;
            }
            // small pause
            this_thread::sleep_for(350ms);
        }
        cout << "Game over. Thanks for playing!\n";
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== Console Ludo (C++) ===\n";
    cout << "Options: Choose how many human players (1-4). Remaining are AI.\n";
    int humans = 1;
    while (true) {
        cout << "How many human players? (1-4) [default 1]: ";
        string line;
        if (!getline(cin,line)) return 0;
        if (line.empty()) { humans = 1; break; }
        try {
            int v = stoi(line);
            if (v>=1 && v<=4) { humans = v; break; }
        } catch(...) {}
        cout << "Invalid. Try again.\n";
    }

    Game G;
    // assign human flags for first N players
    for (int i=0;i<4;i++){
        G.players[i].isHuman = (i < humans);
        // allow custom names for humans
        if (G.players[i].isHuman) {
            cout << "Enter name for Player " << (i+1) << " [default " << G.players[i].name << "]: ";
            string name;
            if (getline(cin, name)) {
                if (!name.empty()) G.players[i].name = name;
            }
        } else {
            G.players[i].name = string("AI ") + to_string(i+1);
        }
    }
    cout << "Starting game with " << humans << " human(s) and " << (4-humans) << " AI(s).\n";
    cout << "Press Enter to start.";
    string tmp;
    getline(cin,tmp);

    G.currentPlayer = 0;
    G.run();

    return 0;
}
