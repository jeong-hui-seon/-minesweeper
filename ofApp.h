#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:

    int rows; //���� �׸��� ��
    int cols; //���� �׸��� ��
    vector<vector<bool>> mines; //���� ���ڰ� �����ϴ��� ���θ� �����ϴ� 2���� ����
    vector<vector<bool>> revealed; //���� ����ڿ� ���� �����Ǿ����� ���� �����ϴ� 2���� ����
    vector<vector<bool>> flagged; //���� ����� ǥ�õǾ����� ���θ� �����ϴ� 2���� ����
    vector<vector<int>> adjacentMines; //���� �ֺ� ���� ���� �����ϴ� 2���� ����
    bool gameOver; //���� ���� �÷���
    bool gameWon; //���� �¸� �÷���
    bool isFirstClick; //������� ù Ŭ�� ���� �÷���
    int flagsLeft; //��� ������ ����� ��

    void setup(); //�ʱ� ������ ���� �Լ�
    void update(); //������ ���� ������Ʈ �Լ�
    void draw(); //�׷����� ȭ�鿡 �׸��� �Լ�
    void keyPressed(int key); //Ű �Է� ó�� �Լ�
    void mousePressed(int x, int y, int button); //���콺 �Է� ó�� �Լ�
    void generateMines(int mineCount, int firstClickRow, int firstClickCol); //���� ���� ���� �Լ�
    void calculateAdjacentMines(); //�� �ֺ� ���� �� ��� �Լ�
    bool isValid(int row, int col); //�� ��ġ�� ��ȿ���� Ȯ���ϴ� �Լ�
    void revealTile(int row, int col); //���� �����ϴ� �Լ�, ��쿡 ���� �ֺ� ���� ����
    void checkWinCondition(); //���� �¸� Ȯ�� �Լ�
    void drawTiles(); //���� Ÿ���� �׸��� �Լ�
    void resetGame(); //���� �ʱ� ���� �Լ�

};
