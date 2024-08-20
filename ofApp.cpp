#include "ofApp.h"

//������ �ʱ� ���·� �����Ѵ�.
void ofApp::setup() {
    rows = 10;
    cols = 10;
    resetGame(); //���� ���� �Լ� ȣ��
}

//������ ��� ������ �ʱ�ȭ�Ѵ�.
void ofApp::resetGame() {
    mines.assign(rows, vector<bool>(cols, false)); //��� ĭ�� ���ڰ� ���� ���·� �ʱ�ȭ�Ѵ�.
    revealed.assign(rows, vector<bool>(cols, false)); //��� ĭ�� �������� ���� ���·� �ʱ�ȭ�Ѵ�.
    flagged.assign(rows, vector<bool>(cols, false)); //��� ����� �����Ѵ�.
    adjacentMines.assign(rows, vector<int>(cols, 0)); //������ ���ڼ��� 0���� �ʱ�ȭ�Ѵ�.
    flagsLeft = 20; //��� �� �ʱ�ȭ
    gameOver = false;
    gameWon = false;
    isFirstClick = true;  //ù ��° Ŭ���� �����ϱ� ���� �ʱ�ȭ
}

void ofApp::update() {
}

//���� ����(���� ����, ���� �¸�, ���� ��)�� ���� �ٸ� ȭ���� �׸���.
void ofApp::draw() {
    if (gameOver) { //���� ���� ������ ���
        ofBackground(0, 0, 0);
        ofSetColor(255, 0, 0);
        ofDrawBitmapString("Game Over", ofGetWidth() / 2 - 40, ofGetHeight() / 2 - 10);
        ofDrawBitmapString("Try again? (y/n)", ofGetWidth() / 2 - 60, ofGetHeight() / 2 + 10);
    }
    else if (gameWon) { //���� �¸� ������ ���
        ofBackground(0, 0, 0);
        ofSetColor(0, 255, 0);
        ofDrawBitmapString("You Win!", ofGetWidth() / 2 - 30, ofGetHeight() / 2 - 10);
        ofDrawBitmapString("Try again? (y/n)", ofGetWidth() / 2 - 60, ofGetHeight() / 2 + 10);
    }
    else { //���� ���� ���� ���
        drawTiles(); //���� Ÿ���� �׸��� �Լ�
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Flags Left: " + std::to_string(flagsLeft), ofGetWidth() - 150, 30); //���� ��� �� ������ ��ܿ� ǥ��
    }
}

//Ű���� �Է��� ó���ϴ� �Լ�
void ofApp::keyPressed(int key) {
    if (gameOver || gameWon) { //���� ���� or ���� �¸� ���¿��� Ű �Է��� ó���Ѵ�.
        if (key == 'y' || key == 'Y') {
            resetGame(); //'y'�� �Է¹��� ���, ���� ����
        }
        else if (key == 'n' || key == 'N') {
            ofExit();  //'n'�� �Է¹��� ���, app ����
        }
    }
}

//���콺 �Է��� ó���ϴ� �Լ�
void ofApp::mousePressed(int x, int y, int button) {
    if (gameOver || gameWon) return;  //������ �����ٸ� �Է��� �����Ѵ�.

    //���콺 Ŭ�� ��ġ���� Ÿ���� ��,�� ���
    int row = (y - 50) / (768 / rows);
    int col = x / (1024 / cols);
    if (!isValid(row, col)) return; //��ȿ���� ���� ��ġ�� ���, �����Ѵ�.

    if (button == OF_MOUSE_BUTTON_LEFT && !flagged[row][col]) { //��Ŭ���̸鼭, ����� ���� ���
        if (isFirstClick) { //ù Ŭ���� ���
            isFirstClick = false; //ù Ŭ�� ���� ������Ʈ
            generateMines(20, row, col); //ù Ŭ�� ��ġ�� �����ϰ� ���ڸ� �����Ѵ�.
            calculateAdjacentMines(); //������ ���� �� ���
            revealTile(row, col); //Ŭ���� Ÿ�� ����
        }
        else {
            if (mines[row][col]) {
                gameOver = true; //���� Ŭ�� ��, ���� ����
            }
            else {
                revealTile(row, col); //���ڰ� �ƴ� Ÿ�� ����
            }
        }
    }
    else if (button == OF_MOUSE_BUTTON_RIGHT && !revealed[row][col]) { //��Ŭ���̸鼭 Ÿ���� �������� ���� ���
        if (flagged[row][col]) { //�̹� ����� �����Ѵٸ�
            flagged[row][col] = false; //��� ����
            flagsLeft++; //���� ��� �� ����
        }
        else if (flagsLeft > 0) { //Ŭ���� Ÿ�Ͽ� ����� ������, ����� �����ִ� ���
            flagged[row][col] = true; //��� ��ġ
            flagsLeft--; //���� ��� �� ����
        }
    }

    if (!gameOver) {
        checkWinCondition(); //�¸� ������ �˻��Ѵ�.
    }
}

//ù Ŭ���� ������ ��ġ�� ���ڸ� �����Ѵ�.
void ofApp::generateMines(int mineCount, int firstClickRow, int firstClickCol) {
    int placedMines = 0;
    while (placedMines < mineCount) {
        int row = floor(ofRandom(rows));
        int col = floor(ofRandom(cols));
        //ù Ŭ�� �ֺ��� �����ϰ� ���ڸ� ��ġ�Ѵ�.
        if (!(row >= firstClickRow - 1 && row <= firstClickRow + 1 &&
            col >= firstClickCol - 1 && col <= firstClickCol + 1) &&
            !mines[row][col]) {
            mines[row][col] = true;
            placedMines++;
        }
    }
}

//�� ĭ�� ���� ���� ���� ���� ����ϴ� �Լ�
void ofApp::calculateAdjacentMines() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (mines[r][c]) continue; //���ڰ� �ִ� ĭ�� ���, �ǳʶڴ�.
            int count = 0;
            //������ 8ĭ�� �˻� �� ������ �� ���
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

//Ư�� ��ġ�� ��ȿ�� �˻� �Լ�
bool ofApp::isValid(int row, int col) {
    return row >= 0 && row < rows && col >= 0 && col < cols; //��,���� ��ȿ���� ���� �ִ��� Ȯ���Ѵ�.
}

//Ư�� ĭ�� �����ϰ�, �ʿ��� ��� �ֺ� ĭ�� �����Ѵ�.
void ofApp::revealTile(int row, int col) {
    if (!isValid(row, col) || revealed[row][col] || mines[row][col]) return; //��ȿ���� �ʰų�, �̹� ���� ĭ�� �����Ѵ�.

    revealed[row][col] = true; //ĭ�� �����Ѵ�.

    if (adjacentMines[row][col] == 0) { //���� ���ڰ� ���� ���, �ֺ� ĭ�� �����Ѵ�.
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = row + dr;
                int nc = col + dc;
                revealTile(nr, nc); //��������� �ֺ� ĭ�� �����Ѵ�.
            }
        }
    }
}

//���� �¸� ���� �˻� (���ڰ� �ƴ� ��� Ÿ���� ���� ���)
void ofApp::checkWinCondition() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!mines[r][c] && !revealed[r][c]) {
                return;  //���ڰ� �ƴ� ���� ������ ���� ĭ ���� ��, �¸����� �ʴ´�.
            }
        }
    }
    gameWon = true; //���ڰ� �ƴ� ��� Ÿ���� ���� ���, ���� �¸�
}

//���� �׸����� Ÿ���� �׸���.
void ofApp::drawTiles() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            ofColor color = ofColor::darkGray;  //�׵θ� �� ����
            ofSetColor(color);
            ofDrawRectangle(c * (1024 / cols), r * (768 / rows) + 50, 1024 / cols, 768 / rows); // ����� �׸���.

            if (revealed[r][c]) { //ĭ�� ������ ���
                color = (mines[r][c] ? ofColor::red : ofColor::white); //���ڸ� ������, �ƴϸ� ������� �����Ѵ�.
            }
            else {
                color = ofColor::gray; //�湮���� ���� ĭ�� ȸ������ ����
            }
            if (flagged[r][c]) {
                color = ofColor::red; //����� ���� ������ ǥ���Ѵ�.
            }

            ofSetColor(color);
            ofFill();
            ofDrawRectangle(c * (1024 / cols) + 1, r * (768 / rows) + 51, 1024 / cols - 2, 768 / rows - 2); //���� ������ ä���.

            if (revealed[r][c] && !mines[r][c] && adjacentMines[r][c] > 0) {
                ofSetColor(ofColor::black);
                string numMines = to_string(adjacentMines[r][c]); //���� ���� ���� ���ڿ��� ��ȯ
                ofDrawBitmapString(numMines, c * (1024 / cols) + 1024 / cols / 2 - 5, r * (768 / rows) + 768 / rows / 2 + 55); //���� ���� �� ���
            }
        }
    }
}