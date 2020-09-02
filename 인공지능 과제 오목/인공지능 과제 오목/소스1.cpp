#include <stdio.h>
#include <vector>
#include <Windows.h>

using namespace std;

int INF = 10000000;
int human = 1;
int AI = 2;
int color, is_easy;

class point {
public:
    int x;
    int y;

    point() {
        this->x = 0;
        this->y = 0;
    }

    point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class Board {
public:
    int board[20][20] = { 0 };
    point rstXY = point();

    int prun(int alpha, int beta, int depth, int player) {
        //    display();
        vector<point> child = possibleNode();

        int uptoDepth = is_easy;
        if (child.size() == 0 || depth == uptoDepth || gameOver() == true) {
            int val = evaluate();
            //            printf("evaluate : %d\n", val);
            return evaluate();
        }

        if (alpha >= beta) {
            if (player == human) {
                return -INF;
            }
            if (player == AI) {
                return INF;
            }
        }

        int maxV = -INF, minV = INF;
        for (int i = 0; i < child.size(); i++) {
            int curScore;
            if (player == human) {
                move(child[i].x, child[i].y, human);
                curScore = prun(alpha, beta, depth + 1, AI);
                //                printf("curScore Human : %d\n", curScore);
                if (curScore < minV) {
                    minV = curScore;
                }
                //                printf("minV : %d\n", minV);
                if (curScore < beta) {
                    beta = curScore;
                }
                //                printf("beta : %d\n", beta);
            }
            else if (player == AI) {
                move(child[i].x, child[i].y, AI);
                curScore = prun(alpha, beta, depth + 1, human);
                //                printf("curScore AI: %d\n", curScore);
                if (curScore > maxV) {
                    maxV = curScore;
                    //                    printf("bbbbbbbbbbbbbbbbbbbbbbbbbb\n");
                    if (depth == 0) {
                        //                        printf("aaaaaaaaaaaaaaaaaaaaaaa\n");
                        rstXY = point(child[i].x, child[i].y);
                    }
                }
                if (curScore > alpha) {
                    alpha = curScore;
                }
            }
            //            printf("<%d> \n", curScore);
            board[child[i].y][child[i].x] = 0;

            if (curScore == INF || curScore == -INF) {
                //                printf("%d %d break!!\n",minV, maxV);
                break;
            }
        }
        if (player == human)
            return minV;
        if (player == AI)
            return maxV;
    }

    // 가능한 모든 노드들을 찾아서 반환
    vector<point> possibleNode() {
        vector<point> rst;
        for (int i = 1; i <= 15; i++)
            for (int j = 1; j <= 15; j++) {
                if (board[i][j] == 0) {
                    int flag = 0;
                    for (int k = -2; k <= 2; k++) {
                        if (i + k <= 15 && 1 <= i + k && 1 <= j - 2 && board[i + k][j - 2] != 0) {
                            flag = 1;
                            break;
                        }
                        if (i + k <= 15 && 1 <= i + k && 1 <= j - 1 && board[i + k][j - 1] != 0) {
                            flag = 1;
                            break;
                        }
                        if (i + k <= 15 && 1 <= i + k && 1 <= j && board[i + k][j] != 0 && k != 0) {
                            flag = 1;
                            break;
                        }
                        if (i + k <= 15 && 1 <= i + k && j + 1 <= 15 && board[i + k][j + 1] != 0) {
                            flag = 1;
                            break;
                        }
                        if (i + k <= 15 && 1 <= i + k && j + 2 <= 15 && board[i + k][j + 2] != 0) {
                            flag = 1;
                            break;
                        }
                    }
                    //주변에 돌이 있을 경우에만 계산한다
                    if (flag == 1)
                        rst.push_back(point(j, i));
                }
            }
        return rst;
    }

    void bestMove() {
        //        printf("\n%d %d\n", rstXY.x, rstXY.y);
        move(rstXY.x, rstXY.y, AI);
    }

    bool gameOver() {
        if (winMan() == true || winAI() == true || possibleNode().empty() == true)
            return true;
        else
            return false;
    }

    bool winMan() {
        //행에 대한 탐색
        for (int i = 1; i <= 15; i++)
            for (int j = 1; j <= 15 - 4; j++) {
                int tmp = 0;
                for (int k = 0; k <= 4; k++) {
                    //행에 대한 탐색
                    if (board[i][j + k] == human)
                        tmp++;
                    else
                        break;
                }
                if (tmp == 5)
                    return true;
            }

        //열에 대한 탐색
        for (int i = 1; i <= 15; i++)
            for (int j = 1; j <= 15 - 4; j++) {
                int tmp = 0;
                for (int k = 0; k <= 4; k++) {
                    //열에 대한 탐색
                    if (board[j + k][i] == human)
                        tmp++;
                    else
                        break;
                }
                if (tmp == 5)
                    return true;
            }

        // 대각성분에 대한 탐색 (1) - 우하향
        for (int i = 1; i <= 15 - 4; i++)
            for (int j = 1; j <= 15 - 4; j++) {
                int tmp = 0;
                for (int k = 0; k <= 4; k++) {
                    if (board[i + k][j + k] == human)
                        tmp++;
                    else
                        break;
                }
                if (tmp == 5)
                    return true;
            }

        // 대각성분에 대한 탐색 (2) - 좌하향
        for (int i = 1; i <= 15 - 4; i++)
            for (int j = 4; j <= 15; j++) {
                int tmp = 0;
                for (int k = 0; k <= 4; k++) {
                    if (board[i + k][j - k] == human)
                        tmp++;
                    else
                        break;
                }
                if (tmp == 5)
                    return true;
            }

        return false;
    }

    bool winAI() {
        //행에 대한 탐색
        for (int i = 1; i <= 15; i++)
            for (int j = 1; j <= 15 - 4; j++) {
                int tmp = 0;
                for (int k = 0; k <= 4; k++) {
                    //행에 대한 탐색
                    if (board[i][j + k] == AI)
                        tmp++;
                    else
                        break;
                }
                if (tmp == 5)
                    return true;
            }

        //열에 대한 탐색
        for (int i = 1; i <= 15; i++)
            for (int j = 1; j <= 15 - 4; j++) {
                int tmp = 0;
                for (int k = 0; k <= 4; k++) {
                    //열에 대한 탐색
                    if (board[j + k][i] == AI)
                        tmp++;
                    else
                        break;
                }
                if (tmp == 5)
                    return true;

            }

        // 대각성분에 대한 탐색 (1) - 우하향
        for (int i = 1; i <= 15 - 4; i++)
            for (int j = 1; j <= 15 - 4; j++) {
                int tmp = 0;
                for (int k = 0; k <= 4; k++) {
                    if (board[i + k][j + k] == AI)
                        tmp++;
                    else
                        break;
                }
                if (tmp == 5)
                    return true;
            }

        // 대각성분에 대한 탐색 (2) - 좌하향
        for (int i = 1; i <= 15 - 4; i++)
            for (int j = 4; j <= 15; j++) {
                int tmp = 0;
                for (int k = 0; k <= 4; k++) {
                    if (board[i + k][j - k] == AI)
                        tmp++;
                    else
                        break;
                }
                if (tmp == 5)
                    return true;
            }

        return false;
    }

    void move(int x, int y, int player) {    //말을 옮겨주는 함수
        board[y][x] = player;
    }

    int evaluate() {    // 평가값을 반환        
        int score = 0;
        //방어 평가표
        /*2개짜리 평가*/
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                if (board[i][j] == 0) {    /*빈칸 시작일때*/
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == AI && board[i][j + 2] == AI) {
                        /*_XX_ _ _ : 200점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == 0 && board[i][j + 5] == 0) {
                            score += 200;
                        }
                        /*_XX_ _O : 100점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == 0 && board[i][j + 5] == human) {
                            score += 100;
                        }
                        /*_XX_ _X : 200점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == 0 && board[i][j + 5] == AI) {
                            score += 200;
                        }
                        /*_XX_O_ : 50점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == human && board[i][j + 5] == 0) {
                            score += 50;
                        }
                        /*_XX_OX : 50점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == human && board[i][j + 5] == AI) {
                            score += 50;
                        }
                        /*_XX_OO : 50점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == human && board[i][j + 5] == human) {
                            score += 50;
                        }
                        /*_XX_X_ : 350점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == AI && board[i][j + 5] == 0) {
                            score += 350;
                        }
                        /*_XX_XO : 300점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == AI && board[i][j + 5] == human) {
                            score += 300;
                        }
                        /*_XX_XX : 400점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == AI && board[i][j + 5] == AI) {
                            score += 400;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == AI && board[i + 2][j] == AI) {
                        /*_XX_ _ _ : 200점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == 0 && board[i + 5][j] == 0) {
                            score += 200;
                        }
                        /*_XX_ _O : 100점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == 0 && board[i + 5][j] == human) {
                            score += 100;
                        }
                        /*_XX_ _X : 200점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == 0 && board[i + 5][j] == AI) {
                            score += 200;
                        }
                        /*_XX_O_ : 50점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == human && board[i + 5][j] == 0) {
                            score += 50;
                        }
                        /*_XX_OX : 50점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == human && board[i + 5][j] == AI) {
                            score += 50;
                        }
                        /*_XX_OO : 50점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == human && board[i + 5][j] == human) {
                            score += 50;
                        }
                        /*_XX_X_ : 350점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == AI && board[i + 5][j] == 0) {
                            score += 350;
                        }
                        /*_XX_XO : 300점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == AI && board[i + 5][j] == human) {
                            score += 300;
                        }
                        /*_XX_XX : 400점*/
                        if (board[i + 3][j] == 0 && board[4][j] == AI && board[i + 5][j] == AI) {
                            score += 400;
                        }
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == AI && board[i + 2][j + 2] == AI) {
                        /*_XX_ _ _ : 200점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == 0) {
                            score += 200;
                        }
                        /*_XX_ _O : 100점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == human) {
                            score += 100;
                        }
                        /*_XX_ _X : 200점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == AI) {
                            score += 200;
                        }
                        /*_XX_O_ : 50점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == human && board[i + 5][j + 5] == 0) {
                            score += 50;
                        }
                        /*_XX_OX : 50점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == human && board[i + 5][j + 5] == AI) {
                            score += 50;
                        }
                        /*_XX_OO : 50점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == human && board[i + 5][j + 5] == human) {
                            score += 50;
                        }
                        /*_XX_X_ : 350점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == AI && board[i + 5][j + 5] == 0) {
                            score += 350;
                        }
                        /*_XX_XO : 300점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == AI && board[i + 5][j + 5] == human) {
                            score += 300;
                        }
                        /*_XX_XX : 400점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == AI && board[i + 5][j + 5] == AI) {
                            score += 400;
                        }
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == AI && board[i + 2][j - 2] == AI) {
                        /*_XX_ _ _ : 200점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == 0) {
                            score += 200;
                        }
                        /*_XX_ _O : 100점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == human) {
                            score += 100;
                        }
                        /*_XX_ _X : 200점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == AI) {
                            score += 200;
                        }
                        /*_XX_O_ : 50점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == human && board[i + 5][j - 5] == 0) {
                            score += 50;
                        }
                        /*_XX_OX : 50점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == human && board[i + 5][j - 5] == AI) {
                            score += 50;
                        }
                        /*_XX_OO : 50점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == human && board[i + 5][j - 5] == human) {
                            score += 50;
                        }
                        /*_XX_X_ : 350점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == AI && board[i + 5][j - 5] == 0) {
                            score += 350;
                        }
                        /*_XX_XO : 300점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == AI && board[i + 5][j - 5] == human) {
                            score += 300;
                        }
                        /*_XX_XX : 400점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == AI && board[i + 5][j - 5] == AI) {
                            score += 400;
                        }
                    }
                }
                else if (board[i][j] == human) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == AI && board[i][j + 2] == AI) {
                        /*OXX_ _ _ : 150점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == 0 && board[i][j + 5] == 0) {
                            score += 150;
                        }
                        /*OXX_ _ X : 200점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == 0 && board[i][j + 5] == AI) {
                            score += 200;
                        }
                        /*OXX_ X _ : 150점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == AI && board[i][j + 5] == 0) {
                            score += 150;
                        }
                        /*OXX_XX : 200점*/
                        if (board[i][j + 3] == 0 && board[i][j + 4] == AI && board[i][j + 5] == AI) {
                            score += 200;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == AI && board[i + 2][j] == AI) {
                        /*OXX_ _ _ : 150점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == 0 && board[i + 5][j] == 0) {
                            score += 150;
                        }
                        /*OXX_ _ X : 200점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == 0 && board[i + 5][j] == AI) {
                            score += 200;
                        }
                        /*OXX_ X _ : 150점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == AI && board[i + 5][j] == 0) {
                            score += 150;
                        }
                        /*OXX_XX : 200점*/
                        if (board[i + 3][j] == 0 && board[i + 4][j] == AI && board[i + 5][j] == AI) {
                            score += 200;
                        }
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == AI && board[i + 2][j + 2] == AI) {
                        /*OXX_ _ _ : 150점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == 0) {
                            score += 150;
                        }
                        /*OXX_ _ X : 200점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == AI) {
                            score += 200;
                        }
                        /*OXX_ X _ : 150점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == AI && board[i + 5][j + 5] == 0) {
                            score += 150;
                        }
                        /*OXX_XX : 200점*/
                        if (board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == AI && board[i + 5][j + 5] == AI) {
                            score += 200;
                        }
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == AI && board[i + 2][j - 2] == AI) {
                        /*OXX_ _ _ : 150점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == 0) {
                            score += 150;
                        }
                        /*OXX_ _ X : 200점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == AI) {
                            score += 200;
                        }
                        /*OXX_ X _ : 150점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == AI && board[i + 5][j - 5] == 0) {
                            score += 150;
                        }
                        /*OXX_XX : 200점*/
                        if (board[i + 3][j - 3] == 0 && board[i + 4][j - 4] == AI && board[i + 5][j - 5] == AI) {
                            score += 200;
                        }
                    }
                }
            }
        }

        /*3개 평가*/
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                /*빈칸으로 시작*/
                if (board[i][j] == 0) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == AI && board[i][j + 2] == AI && board[i][j + 3] == AI) {
                        /*_XXX_ _ : 2,000점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == 0) {
                            score += 2000;
                        }
                        /*_XXX_O : 1,500점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == human) {
                            score += 1500;
                        }
                        /*_XXX_X : 2,000점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == AI) {
                            score += 2000;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == AI && board[i + 2][j] == AI && board[i + 3][j] == AI) {
                        /*_XXX_ _ : 2,000점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == 0) {
                            score += 2000;
                        }
                        /*_XXX_O : 1,500점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == human) {
                            score += 1500;
                        }
                        /*_XXX_X : 2,000점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == AI) {
                            score += 2000;
                        }
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == AI && board[i + 2][j + 2] == AI && board[i + 3][j + 3] == AI) {
                        /*_XXX_ _ : 2,000점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == 0) {
                            score += 2000;
                        }
                        /*_XXX_O : 1,500점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == human) {
                            score += 1500;
                        }
                        /*_XXX_X : 2,000점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == AI) {
                            score += 2000;
                        }
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == AI && board[i + 2][j - 2] == AI && board[i + 3][j - 3] == AI) {
                        /*_XXX_ _ : 2,000점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == 0) {
                            score += 2000;
                        }
                        /*_XXX_O : 1,500점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == human) {
                            score += 1500;
                        }
                        /*_XXX_X : 2,000점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == AI) {
                            score += 2000;
                        }
                    }
                }
                /*O로 시작*/
                if (board[i][j] == human) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == AI && board[i][j + 2] == AI && board[i][j + 3] == AI) {
                        /*OXXX_ _ : 1,000점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == 0) {
                            score += 1000;
                        }
                        /*OXXX_X : 2,000점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == AI) {
                            score += 2000;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == AI && board[i + 2][j] == AI && board[i + 3][j] == AI) {
                        /*OXXX_ _ : 1,000점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == 0) {
                            score += 1000;
                        }
                        /*OXXX_X : 2,000점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == AI) {
                            score += 2000;
                        }
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == AI && board[i + 2][j + 2] == AI && board[i + 3][j + 3] == AI) {
                        /*OXXX_ _ : 1,000점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == 0) {
                            score += 1000;
                        }
                        /*OXXX_X : 2,000점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == AI) {
                            score += 2000;
                        }
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == AI && board[i + 2][j - 2] == AI && board[i + 3][j - 3] == AI) {
                        /*OXXX_ _ : 1,000점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == 0) {
                            score += 1000;
                        }
                        /*OXXX_X : 2,000점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == AI) {
                            score += 2000;
                        }
                    }
                }
            }
        }
        /*4개*/
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                /*빈칸으로 시작*/
                if (board[i][j] == 0) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == AI && board[i][j + 2] == AI && board[i][j + 3] == AI && board[i][j + 4] == AI) {
                        /*_XXXXO : 10,000점*/
                        if (board[i][j + 5] == human) {
                            score += 10000;
                        }
                        /*_XXXX_ : 15,000점*/
                        if (board[i][j + 5] == 0) {
                            score += 15000;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == AI && board[i + 2][j] == AI && board[i + 3][j] == AI && board[i + 4][j] == AI) {
                        /*_XXXXO : 10,000점*/
                        if (board[i + 5][j] == human) {
                            score += 10000;
                        }
                        /*_XXXX_ : 15,000점*/
                        if (board[i + 5][j] == 0) {
                            score += 15000;
                        }
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == AI && board[i + 2][j + 2] == AI && board[i + 3][j + 3] == AI && board[i + 4][j + 4] == AI) {
                        /*_XXXXO : 10,000점*/
                        if (board[i + 5][j + 5] == human) {
                            score += 10000;
                        }
                        /*_XXXX_ : 15,000점*/
                        if (board[i + 5][j + 5] == 0) {
                            score += 15000;
                        }
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == AI && board[i + 2][j - 2] == AI && board[i + 3][j - 3] == AI && board[i + 4][j - 4] == AI) {
                        /*_XXXXO : 10,000점*/
                        if (board[i + 5][j - 5] == human) {
                            score += 10000;
                        }
                        /*_XXXX_ : 15,000점*/
                        if (board[i + 5][j - 5] == 0) {
                            score += 15000;
                        }
                    }
                }
                /*O로 시작*/
                if (board[i][j] == human) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == AI && board[i][j + 2] == AI && board[i][j + 3] == AI && board[i][j + 4] == AI) {
                        /*OXXXX_ : 10,000점*/
                        if (board[i][j + 5] == 0) {
                            score += 10000;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == AI && board[i + 2][j] == AI && board[i + 3][j] == AI && board[i + 4][j] == AI) {
                        /*OXXXX_ : 10,000점*/
                        if (board[i + 5][j] == 0) {
                            score += 10000;
                        }
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == AI && board[i + 2][j + 2] == AI && board[i + 3][j + 3] == AI && board[i + 4][j + 4] == AI) {
                        /*OXXXX_ : 10,000점*/
                        if (board[i + 5][j + 5] == 0) {
                            score += 10000;
                        }
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == AI && board[i + 2][j - 2] == AI && board[i + 3][j - 3] == AI && board[i + 4][j - 4] == AI) {
                        /*OXXXX_ : 10,000점*/
                        if (board[i + 5][j - 5] == 0) {
                            score += 10000;
                        }
                    }
                }
            }
        }
        /*5개*/
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                /*빈칸으로 시작*/
                if (board[i][j] == 0) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == AI && board[i][j + 2] == AI && board[i][j + 3] == AI && board[i][j + 4] == AI && board[i][j + 5] == AI) {
                        /*_XXXXX : 100,000점*/
                        score += 100000;
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == AI && board[i + 2][j] == AI && board[i + 3][j] == AI && board[i + 4][j] == AI && board[i + 5][j] == AI) {
                        /*_XXXXX : 100,000점*/
                        score += 100000;
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == AI && board[i + 2][j + 2] == AI && board[i + 3][j + 3] == AI && board[i + 4][j + 4] == AI && board[i + 5][j + 5] == AI) {
                        /*_XXXXX : 100,000점*/
                        score += 100000;
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == AI && board[i + 2][j - 2] == AI && board[i + 3][j - 3] == AI && board[i + 4][j - 4] == AI && board[i + 5][j - 5] == AI) {
                        /*_XXXXX : 100,000점*/
                        score += 100000;
                    }
                }
                /*O로 시작*/
                if (board[i][j] == human) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == AI && board[i][j + 2] == AI && board[i][j + 3] == AI && board[i][j + 4] == AI && board[i][j + 5] == AI) {
                        /*_XXXXX : 100,000점*/
                        score += 100000;
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == AI && board[i + 2][j] == AI && board[i + 3][j] == AI && board[i + 4][j] == AI && board[i + 5][j] == AI) {
                        /*_XXXXX : 100,000점*/
                        score += 100000;
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == AI && board[i + 2][j + 2] == AI && board[i + 3][j + 3] == AI && board[i + 4][j + 4] == AI && board[i + 5][j + 5] == AI) {
                        /*_XXXXX : 100,000점*/
                        score += 100000;
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == AI && board[i + 2][j - 2] == AI && board[i + 3][j - 3] == AI && board[i + 4][j - 4] == AI && board[i + 5][j - 5] == AI) {
                        /*_XXXXX : 100,000점*/
                        score += 100000;
                    }
                }
            }
        }
        //공격 평가표
        /*3개*/
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                /*빈칸으로 시작*/
                if (board[i][j] == 0) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == human && board[i][j + 2] == human && board[i][j + 3] == human) {
                        /*_OOO_ _ : -2,000점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == 0) {
                            score -= 2000;
                        }
                        /*_OOO_X : -1,500점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == AI) {
                            score -= 1500;
                        }
                        /*_OOO_O : -2,000점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == human) {
                            score -= 2000;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == human && board[i + 2][j] == human && board[i + 3][j] == human) {
                        /*_OOO_ _ : -2,000점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == 0) {
                            score -= 2000;
                        }
                        /*_OOO_X : -1,500점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == AI) {
                            score -= 1500;
                        }
                        /*_OOO_O : -2,000점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == human) {
                            score -= 2000;
                        }
                    }
                    /*우대각*/
                    if (j + 5 <= 15 && i + 5 <= 15 && board[i + 1][j + 1] == human && board[i + 2][j + 2] == human && board[i + 3][j + 3] == human) {
                        /*_OOO_ _ : -2,000점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == 0) {
                            score -= 2000;
                        }
                        /*_OOO_X : -1,500점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == AI) {
                            score -= 1500;
                        }
                        /*_OOO_O : -2,000점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == human) {
                            score -= 2000;
                        }
                    }
                    /*좌대각*/
                    if (j - 5 >= 1 && i + 5 <= 15 && board[i + 1][j - 1] == human && board[i + 2][j - 2] == human && board[i + 3][j - 3] == human) {
                        /*_OOO_ _ : -2,000점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == 0) {
                            score -= 2000;
                        }
                        /*_OOO_X : -1,500점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == AI) {
                            score -= 1500;
                        }
                        /*_OOO_O : -2,000점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == human) {
                            score -= 2000;
                        }
                    }
                }
                /*X로 시작*/
                if (board[i][j] == AI) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == human && board[i][j + 2] == human && board[i][j + 3] == human) {
                        /*XOOO_ _ : -1,000점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == 0) {
                            score -= 1000;
                        }
                        /*XOOO_O : -2,000점*/
                        if (board[i][j + 4] == 0 && board[i][j + 5] == human) {
                            score -= 2000;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == human && board[i + 2][j] == human && board[i + 3][j] == human) {
                        /*XOOO_ _ : -1,000점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == 0) {
                            score -= 1000;
                        }
                        /*XOOO_O : -2,000점*/
                        if (board[i + 4][j] == 0 && board[i + 5][j] == human) {
                            score -= 2000;
                        }
                    }
                    /*우대각*/
                    if (j + 5 <= 15 && i + 5 <= 15 && board[i + 1][j + 1] == human && board[i + 2][j + 2] == human && board[i + 3][j + 3] == human) {
                        /*XOOO_ _ : -1,000점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == 0) {
                            score -= 1000;
                        }
                        /*XOOO_O : -2,000점*/
                        if (board[i + 4][j + 4] == 0 && board[i + 5][j + 5] == human) {
                            score -= 2000;
                        }
                    }
                    /*좌대각*/
                    if (j - 5 >= 1 && i + 5 <= 15 && board[i + 1][j - 1] == human && board[i + 2][j - 2] == human && board[i + 3][j - 3] == human) {
                        /*XOOO_ _ : -1,000점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == 0) {
                            score -= 1000;
                        }
                        /*XOOO_O : -2,000점*/
                        if (board[i + 4][j - 4] == 0 && board[i + 5][j - 5] == human) {
                            score -= 2000;
                        }
                    }
                }
            }
        }
        /*4개*/
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                /*빈칸으로 시작*/
                if (board[i][j] == 0) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == human && board[i][j + 2] == human && board[i][j + 3] == human && board[i][j + 4] == human) {
                        /*_OOOOX : -10,000점*/
                        if (board[i][j + 5] == AI) {
                            score -= 10000;
                        }
                        /*_OOOO_ : -15,000점*/
                        if (board[i][j + 5] == 0) {
                            score -= 15000;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == human && board[i + 2][j] == human && board[i + 3][j] == human && board[i + 4][j] == human) {
                        /*_OOOOX : -10,000점*/
                        if (board[i + 5][j] == AI) {
                            score -= 10000;
                        }
                        /*_OOOO_ : -15,000점*/
                        if (board[i + 5][j] == 0) {
                            score -= 15000;
                        }
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == human && board[i + 2][j + 2] == human && board[i + 3][j + 3] == human && board[i + 4][j + 4] == human) {
                        /*_OOOOX : -10,000점*/
                        if (board[i + 5][j + 5] == AI) {
                            score -= 10000;
                        }
                        /*_OOOO_ : -15,000점*/
                        if (board[i + 5][j + 5] == 0) {
                            score -= 15000;
                        }
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == human && board[i + 2][j - 2] == human && board[i + 3][j - 3] == human && board[i + 4][j - 4] == human) {
                        /*_OOOOX : -10,000점*/
                        if (board[i + 5][j - 5] == AI) {
                            score -= 10000;
                        }
                        /*_OOOO_ : -15,000점*/
                        if (board[i + 5][j - 5] == 0) {
                            score -= 15000;
                        }
                    }
                }
                /*X로 시작*/
                if (board[i][j] == AI) {
                    /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == human && board[i][j + 2] == human && board[i][j + 3] == human && board[i][j + 4] == human) {
                        /*XOOOO_ : -10,000점*/
                        if (board[i][j + 5] == 0) {
                            score -= 10000;
                        }
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == human && board[i + 2][j] == human && board[i + 3][j] == human && board[i + 4][j] == human) {
                        /*XOOOO_ : -10,000점*/
                        if (board[i + 5][j] == 0) {
                            score -= 10000;
                        }
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == human && board[i + 2][j + 2] == human && board[i + 3][j + 3] == human && board[i + 4][j + 4] == human) {
                        /*XOOOO_ : -10,000점*/
                        if (board[i + 5][j + 5] == 0) {
                            score -= 10000;
                        }
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == human && board[i + 2][j - 2] == human && board[i + 3][j - 3] == human && board[i + 4][j - 4] == human) {
                        /*XOOOO_ : -10,000점*/
                        if (board[i + 5][j - 5] == 0) {
                            score -= 10000;
                        }
                    }
                }
            }
        }
        /*5개*/
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                /*빈칸 또는 X로 시작*/
                if (board[i][j] == 0 || board[i][j] == AI) {
                    /*  _OOOOO : -100,000점
                        XOOOOO : -100,000점 */
                        /*가로*/
                    if (j + 5 <= 15 && board[i][j + 1] == human && board[i][j + 2] == human && board[i][j + 3] == human && board[i][j + 4] == human && board[i][j + 5] == human) {
                        score -= 100000;
                    }
                    /*세로*/
                    if (i + 5 <= 15 && board[i + 1][j] == human && board[i + 2][j] == human && board[i + 3][j] == human && board[i + 4][j] == human && board[i + 5][j] == human) {
                        score -= 100000;
                    }
                    /*우대각*/
                    if (i + 5 <= 15 && j + 5 <= 15 && board[i + 1][j + 1] == human && board[i + 2][j + 2] == human && board[i + 3][j + 3] == human && board[i + 4][j + 4] == human && board[i + 5][j + 5] == human) {
                        score -= 100000;
                    }
                    /*좌대각*/
                    if (i + 5 <= 15 && j - 5 >= 1 && board[i + 1][j - 1] == human && board[i + 2][j - 2] == human && board[i + 3][j - 3] == human && board[i + 4][j - 4] == human && board[i + 5][j - 5] == human) {
                        score -= 100000;
                    }
                }
            }
        }
        return score;
    }

    void display() {
        system("cls");

        for (int i = 0; i < 9; i++) printf(" ");
        printf("<대결 중>\n\n");

        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 15; j++) {
                switch (board[i][j])
                {
                case 0:    //None
                    printf("* ");
                    break;
                case 1:    //human
                    if (color != 1)
                        printf("O ");
                    else
                        printf("X ");
                    break;
                case 2:    //AI
                    if (color != 1)
                        printf("X ");
                    else
                        printf("O ");
                    break;
                }
            }
            printf("\n");
        }
    }
};

void preset() {
    while (1) {
        system("cls");

        printf("Black, White 중 어느 색을 선택하시겠습니까?\n");
        printf("Black 을 선택 시 ""X"" 로 표시되고, White 를 선택시 ""O"" 로 표시됩니다\n");
        printf("b (Black) / w (White)\n");

        char bh;
        scanf("%c", &bh);

        if (bh == 'b') {
            color = 1;
            break;
        }
        else if (bh == 'w') {
            color = 2;
            break;
        }
        else {
            printf("잘못 입력하셨습니다.\n다시 입력하세요");
            Sleep(1000);
        }
    }

    while (1) {
        system("cls");
        printf("난이도를 선택하세요 : (easy) 1 ~ 4 (hard)\n");
        scanf("%d", &is_easy);

        if (is_easy >= 4) {
            printf("난이도가 4 이상인 경우, 약 2분간 대기 시간이 있을 수 있습니다\n");
            printf("진행 하시겠습니까?   y / n\n");

            char yes_no;
            getchar();
            scanf("%c", &yes_no);

            if (yes_no == 'n') {
                Sleep(1000);
                continue;
            }
        }
        else
            printf("최대 소요시간 : 5초\n");

        break;
    }
}

int main() {
    preset();

    Board b = Board();

    b.display();
    int num;
    printf("\n먼저 돌을 놓으세요\n좌표를 입력하세요\n");

    int y, x;
    scanf("%d %d", &y, &x);

    b.move(x, y, human);
    b.display();
    printf("\n");

    while (b.gameOver() == false) {
        b.prun(-INF, INF, 0, AI);

        b.bestMove();
        b.display();

        if (b.gameOver() == true)
            break;

        printf("\n좌표를 입력하세요\n");
        scanf("%d %d", &y, &x);

        b.move(x, y, human);
        b.display();
        printf("\n");
    }

    if (b.winMan() == true)
        printf("You Win!! \n");
    else if (b.winAI() == true)
        printf("AI Win!! \n");
    else
        printf("Draw!!\n");
}