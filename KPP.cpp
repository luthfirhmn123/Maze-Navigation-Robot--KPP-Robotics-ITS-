#include <bits/stdc++.h>
using namespace std;

class RobotNavigator {
protected:
    static const int MAX_N = 15;
    char mazeGridBuffer[MAX_N][MAX_N];
    int gridSize;
    int startRow, startCol;
    int FlagRow, FlagCol;
    int goal_row, goal_col;
    vector<pair<int,int>> pathToFlag;
    vector<pair<int,int>> pathToGoal;

    void methodLoadMaze(vector<string> RowData) { // --- Menyalin maze dari soal 1 - 3 ke array 2D ini (maze grid buffer) --- (2)
        for (int i = 0; i < gridSize; i++)
            for (int j = 0; j < gridSize; j++)
                mazeGridBuffer[i][j] = RowData[i][j];
    }

    void methodLocateMarkers() { // --- Menscan maze untuk menemukan lokasi S, F, dan G. Lalu menyimpan koordinatnya --- (3)
        for (int Row = 0; Row < gridSize; Row++) {
            for (int col_idx = 0; col_idx < gridSize; col_idx++) {
                char CurrentTile = mazeGridBuffer[Row][col_idx];
                if (CurrentTile == 'S') { startRow = Row; startCol = col_idx; }
                else if (CurrentTile == 'F') { FlagRow = Row; FlagCol = col_idx; }
                else if (CurrentTile == 'G') { goal_row = Row; goal_col = col_idx; }
            }
        }
    }

    // --- FUNGSI INTI, mencari jalur terpendek dengan BFS --- (4)
    vector<pair<int,int>> methodFindPath(int srcRow, int srcCol, int DestRow, int dest_col) {
        vector<vector<bool>> Visited(gridSize, vector<bool>(gridSize, false));
        vector<vector<pair<int,int>>> ParentMap(gridSize, vector<pair<int,int>>(gridSize, {-1,-1}));
        queue<pair<int,int>> q_traversalNode;
        q_traversalNode.push({srcRow, srcCol});
        Visited[srcRow][srcCol] = true;
        int dRow[4] = {-1, 1, 0, 0};
        int DCol[4] = {0, 0, -1, 1};
        while (!q_traversalNode.empty()) {
            pair<int,int> currentNode = q_traversalNode.front();
            q_traversalNode.pop();
            if (currentNode.first == DestRow && currentNode.second == dest_col) break;
            for (int Dir = 0; Dir < 4; Dir++) {
                int nextRow = currentNode.first + dRow[Dir];
                int NextCol = currentNode.second + DCol[Dir];
                if (nextRow < 0 || nextRow >= gridSize || NextCol < 0 || NextCol >= gridSize) continue;
                if (Visited[nextRow][NextCol]) continue;
                char tile_here = mazeGridBuffer[nextRow][NextCol];
                if (tile_here == '#' || tile_here == 'X') continue;
                Visited[nextRow][NextCol] = true;
                ParentMap[nextRow][NextCol] = currentNode;
                q_traversalNode.push({nextRow, NextCol});
            }
        }
        vector<pair<int,int>> ResultPath;
        pair<int,int> walker_pos = {DestRow, dest_col};
        while (walker_pos.first != -1) {
            ResultPath.push_back(walker_pos);
            walker_pos = ParentMap[walker_pos.first][walker_pos.second];
        }
        reverse(ResultPath.begin(), ResultPath.end());
        return ResultPath;
    }

    string methodDirectionLabel(pair<int,int> FromNode, pair<int,int> to_node) { // --- Penerjemah untuk menemukan up, down, dll --- (5)
        if (to_node.first == FromNode.first - 1) return "UP";
        if (to_node.first == FromNode.first + 1) return "DOWN";
        if (to_node.second == FromNode.second - 1) return "LEFT";
        return "RIGHT";
    }

    void methodPrintDirections(vector<pair<int,int>> &Segment) { // --- Mencetak pergerakan tersebut --- (6)
        for (size_t Idx = 1; Idx < Segment.size(); Idx++) {
            cout << methodDirectionLabel(Segment[Idx - 1], Segment[Idx]);
            if (Idx != Segment.size() - 1) cout << ", ";
        }
        cout << "\n";
    }

public:
    string robotName;

    RobotNavigator(int SizeN, string Name) { // --- NYIMPEN GRID SIZENYA & NAMA ROBOT--- (1)
        gridSize = SizeN;
        robotName = Name;
    }

    void Viz(pair<int,int> RobotPos) { // --- Untuk visualisasi 'R' yang bergerak tiap langkah --- (7)
        for (int Row = 0; Row < gridSize; Row++) {
            for (int Col = 0; Col < gridSize; Col++) {
                if (Row == RobotPos.first && Col == RobotPos.second) cout << 'R';
                else cout << mazeGridBuffer[Row][Col];
                cout << ' ';
            }
            cout << "\n";
        }
        cout << "\n";
    }

    virtual void methodExecuteMission(vector<string> MazeRows) = 0; // --- menetapkan bahwa setiap robot harus punya cara menjalankan misi --- (8)
};

class AutoRobot : public RobotNavigator {
public:
    AutoRobot(int SizeN, string Name) : RobotNavigator(SizeN, Name) {} // --- meneruskan parameter ke constructor base class --- (9)

    void methodExecuteMission(vector<string> MazeRows) override { // --- yang mengatur keseluruhan --- (10)
        methodLoadMaze(MazeRows);
        methodLocateMarkers();

        pathToFlag = methodFindPath(startRow, startCol, FlagRow, FlagCol);
        pathToGoal = methodFindPath(FlagRow, FlagCol, goal_row, goal_col);

        cout << "ROBOT NAME    : " << robotName << "\n";
        cout << "MAP LOADED    : " << gridSize << " x " << gridSize << "\n";
        cout << "START POSITION: (" << startRow << "," << startCol << ")\n";

        cout << "PATH TO FLAG  : ";
        methodPrintDirections(pathToFlag);
        for (auto &StepNode : pathToFlag) Viz(StepNode);
        cout << "FLAG CAPTURED : (" << FlagRow << "," << FlagCol << ")\n\n";

        cout << "PATH TO BASE  : ";
        methodPrintDirections(pathToGoal);
        for (auto &step_node2 : pathToGoal) Viz(step_node2);
        cout << "BASE REACHED  : (" << goal_row << "," << goal_col << ")\n";
        cout << "MISSION COMPLETE\n";

        int total_step_count = (int)(pathToFlag.size() - 1) + (int)(pathToGoal.size() - 1);
        cout << "TOTAL MOVES   : " << total_step_count << "\n";
        cout << "==================================================\n";
    }
};

int main() {
    vector<string> Soal1 = {
        "#######",
        "#S#...#",
        "#.#.#G#",
        "#...#.#",
        "###...#",
        "#X..F.#",
        "#######"
    };

    vector<string> Soal2 = {
        "#########",
        "#S..#...#",
        "#.#.#.#G#",
        "#.#...#.#",
        "#...#...#",
        "###.#..##",
        "#X..#..F#",
        "#.###X..#",
        "#########"
    };

    vector<string> Soal3 = {
        "###########",
        "#S..#.....#",
        "#.#.#.###.#",
        "#.#...#G#.#",
        "#...#.#.#.#",
        "###.#...#.#",
        "#...###.#.#",
        "#.#..X..#.#",
        "#.#.###.#F#",
        "#X....X...#",
        "###########"
    };

    AutoRobot robot1(7, "Atlas");
    cout << "SOAL 1\n";
    robot1.methodExecuteMission(Soal1);

    AutoRobot robot2(9, "Atlas");
    cout << "SOAL 2\n";
    robot2.methodExecuteMission(Soal2);

    AutoRobot robot3(11, "Atlas");
    cout << "SOAL 3\n";
    robot3.methodExecuteMission(Soal3);

    return 0;
}