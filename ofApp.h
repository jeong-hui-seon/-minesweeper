#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:

    int rows; //게임 그리드 행
    int cols; //게임 그리드 열
    vector<vector<bool>> mines; //셀에 지뢰가 존재하는지 여부를 저장하는 2차원 벡터
    vector<vector<bool>> revealed; //셀이 사용자에 의해 공개되었는지 여부 저장하는 2차원 벡터
    vector<vector<bool>> flagged; //셀에 깃발이 표시되었는지 여부를 저장하는 2차원 벡터
    vector<vector<int>> adjacentMines; //셀의 주변 지뢰 수를 저장하는 2차원 벡터
    bool gameOver; //게임 오버 플래그
    bool gameWon; //게임 승리 플래그
    bool isFirstClick; //사용자의 첫 클릭 여부 플래그
    int flagsLeft; //사용 가능한 깃발의 수

    void setup(); //초기 설정을 위한 함수
    void update(); //프레임 상태 업데이트 함수
    void draw(); //그래픽을 화면에 그리는 함수
    void keyPressed(int key); //키 입력 처리 함수
    void mousePressed(int x, int y, int button); //마우스 입력 처리 함수
    void generateMines(int mineCount, int firstClickRow, int firstClickCol); //지뢰 랜덤 생성 함수
    void calculateAdjacentMines(); //셀 주변 지뢰 수 계산 함수
    bool isValid(int row, int col); //셀 위치가 유효한지 확인하는 함수
    void revealTile(int row, int col); //셀을 공개하는 함수, 경우에 따라 주변 셀도 공개
    void checkWinCondition(); //게임 승리 확인 함수
    void drawTiles(); //게임 타일을 그리는 함수
    void resetGame(); //게임 초기 리셋 함수

};
