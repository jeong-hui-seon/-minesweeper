#include "ofApp.h"

//게임을 초기 상태로 설정한다.
void ofApp::setup() {
    rows = 10;
    cols = 10;
    resetGame(); //게임 리셋 함수 호출
}

//게임의 모든 변수를 초기화한다.
void ofApp::resetGame() {
    mines.assign(rows, vector<bool>(cols, false)); //모든 칸에 지뢰가 없는 상태로 초기화한다.
    revealed.assign(rows, vector<bool>(cols, false)); //모든 칸이 공개되지 않은 상태로 초기화한다.
    flagged.assign(rows, vector<bool>(cols, false)); //모든 깃발을 제거한다.
    adjacentMines.assign(rows, vector<int>(cols, 0)); //인접한 지뢰수를 0으로 초기화한다.
    flagsLeft = 20; //깃발 수 초기화
    gameOver = false;
    gameWon = false;
    isFirstClick = true;  //첫 번째 클릭을 감지하기 위한 초기화
}

void ofApp::update() {
}

//게임 상태(게임 오버, 게임 승리, 진행 중)에 따라 다른 화면을 그린다.
void ofApp::draw() {
    if (gameOver) { //게임 오버 상태인 경우
        ofBackground(0, 0, 0);
        ofSetColor(255, 0, 0);
        ofDrawBitmapString("Game Over", ofGetWidth() / 2 - 40, ofGetHeight() / 2 - 10);
        ofDrawBitmapString("Try again? (y/n)", ofGetWidth() / 2 - 60, ofGetHeight() / 2 + 10);
    }
    else if (gameWon) { //게임 승리 상태인 경우
        ofBackground(0, 0, 0);
        ofSetColor(0, 255, 0);
        ofDrawBitmapString("You Win!", ofGetWidth() / 2 - 30, ofGetHeight() / 2 - 10);
        ofDrawBitmapString("Try again? (y/n)", ofGetWidth() / 2 - 60, ofGetHeight() / 2 + 10);
    }
    else { //게임 진행 중인 경우
        drawTiles(); //게임 타일을 그리는 함수
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Flags Left: " + std::to_string(flagsLeft), ofGetWidth() - 150, 30); //남은 깃발 수 오른쪽 상단에 표시
    }
}

//키보드 입력을 처리하는 함수
void ofApp::keyPressed(int key) {
    if (gameOver || gameWon) { //게임 오버 or 게임 승리 상태에서 키 입력을 처리한다.
        if (key == 'y' || key == 'Y') {
            resetGame(); //'y'를 입력받은 경우, 게임 리셋
        }
        else if (key == 'n' || key == 'N') {
            ofExit();  //'n'를 입력받은 경우, app 종료
        }
    }
}

//마우스 입력을 처리하는 함수
void ofApp::mousePressed(int x, int y, int button) {
    if (gameOver || gameWon) return;  //게임이 끝났다면 입력을 무시한다.

    //마우스 클릭 위치에서 타일의 행,열 계산
    int row = (y - 50) / (768 / rows);
    int col = x / (1024 / cols);
    if (!isValid(row, col)) return; //유효하지 않은 위치인 경우, 무시한다.

    if (button == OF_MOUSE_BUTTON_LEFT && !flagged[row][col]) { //좌클릭이면서, 깃발이 없는 경우
        if (isFirstClick) { //첫 클릭인 경우
            isFirstClick = false; //첫 클릭 상태 업데이트
            generateMines(20, row, col); //첫 클릭 위치를 제외하고 지뢰를 생성한다.
            calculateAdjacentMines(); //인접한 지뢰 수 계산
            revealTile(row, col); //클릭한 타일 공개
        }
        else {
            if (mines[row][col]) {
                gameOver = true; //지뢰 클릭 시, 게임 오버
            }
            else {
                revealTile(row, col); //지뢰가 아닌 타일 공개
            }
        }
    }
    else if (button == OF_MOUSE_BUTTON_RIGHT && !revealed[row][col]) { //우클릭이면서 타일이 공개되지 않은 경우
        if (flagged[row][col]) { //이미 깃발이 존재한다면
            flagged[row][col] = false; //깃발 제거
            flagsLeft++; //남은 깃발 수 증가
        }
        else if (flagsLeft > 0) { //클릭한 타일에 깃발이 없으며, 깃발이 남아있는 경우
            flagged[row][col] = true; //깃발 설치
            flagsLeft--; //남은 깃발 수 감소
        }
    }

    if (!gameOver) {
        checkWinCondition(); //승리 조건을 검사한다.
    }
}

//첫 클릭을 제외한 위치에 지뢰를 생성한다.
void ofApp::generateMines(int mineCount, int firstClickRow, int firstClickCol) {
    int placedMines = 0;
    while (placedMines < mineCount) {
        int row = floor(ofRandom(rows));
        int col = floor(ofRandom(cols));
        //첫 클릭 주변을 제외하고 지뢰를 배치한다.
        if (!(row >= firstClickRow - 1 && row <= firstClickRow + 1 &&
            col >= firstClickCol - 1 && col <= firstClickCol + 1) &&
            !mines[row][col]) {
            mines[row][col] = true;
            placedMines++;
        }
    }
}

//각 칸에 대해 인접 지뢰 수를 계산하는 함수
void ofApp::calculateAdjacentMines() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (mines[r][c]) continue; //지뢰가 있는 칸인 경우, 건너뛴다.
            int count = 0;
            //인접한 8칸을 검사 후 지뢰의 수 기록
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr;
                    int nc = c + dc;
                    if (isValid(nr, nc) && mines[nr][nc]) count++;
                }
            }
            adjacentMines[r][c] = count;
        }
    }
}

//특정 위치의 유효성 검사 함수
bool ofApp::isValid(int row, int col) {
    return row >= 0 && row < rows && col >= 0 && col < cols; //행,열이 유효범위 내에 있는지 확인한다.
}

//특정 칸을 공개하고, 필요한 경우 주변 칸도 공개한다.
void ofApp::revealTile(int row, int col) {
    if (!isValid(row, col) || revealed[row][col] || mines[row][col]) return; //유효하지 않거나, 이미 열린 칸은 무시한다.

    revealed[row][col] = true; //칸을 공개한다.

    if (adjacentMines[row][col] == 0) { //인접 지뢰가 없는 경우, 주변 칸도 공개한다.
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = row + dr;
                int nc = col + dc;
                revealTile(nr, nc); //재귀적으로 주변 칸도 공개한다.
            }
        }
    }
}

//게임 승리 조건 검사 (지뢰가 아닌 모든 타일이 열린 경우)
void ofApp::checkWinCondition() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!mines[r][c] && !revealed[r][c]) {
                return;  //지뢰가 아닌 아직 열리지 않은 칸 존재 시, 승리하지 않는다.
            }
        }
    }
    gameWon = true; //지뢰가 아닌 모든 타일이 열린 경우, 게임 승리
}

//게임 그리드의 타일을 그린다.
void ofApp::drawTiles() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            ofColor color = ofColor::darkGray;  //테두리 색 설정
            ofSetColor(color);
            ofDrawRectangle(c * (1024 / cols), r * (768 / rows) + 50, 1024 / cols, 768 / rows); // 블록을 그린다.

            if (revealed[r][c]) { //칸이 공개된 경우
                color = (mines[r][c] ? ofColor::red : ofColor::white); //지뢰면 빨간색, 아니면 흰색으로 설정한다.
            }
            else {
                color = ofColor::gray; //방문하지 않은 칸은 회색으로 설정
            }
            if (flagged[r][c]) {
                color = ofColor::red; //깃발은 빨간 색으로 표시한다.
            }

            ofSetColor(color);
            ofFill();
            ofDrawRectangle(c * (1024 / cols) + 1, r * (768 / rows) + 51, 1024 / cols - 2, 768 / rows - 2); //내부 색상을 채운다.

            if (revealed[r][c] && !mines[r][c] && adjacentMines[r][c] > 0) {
                ofSetColor(ofColor::black);
                string numMines = to_string(adjacentMines[r][c]); //인접 지뢰 수를 문자열로 변환
                ofDrawBitmapString(numMines, c * (1024 / cols) + 1024 / cols / 2 - 5, r * (768 / rows) + 768 / rows / 2 + 55); //인접 지뢰 수 출력
            }
        }
    }
}