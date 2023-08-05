﻿#include <iostream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <string.h>
#define PLAYER_START_POSX 0
#define PLAYER_START_POSY 270
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540
#define PLAYER_SIZE 30
#define ENEMY_SIZE 30
#define HITBOX_SIZE 15

#define ENEMY_GENERATE_MINX 150
#define ENEMY_GENERATE_MAXX 900
#define ENEMY_GENERATE_MINY 0
#define ENEMY_GENERATE_MAXY 500

#define GOAL_POSX 950
#define GOAL_POSY 270
#define GOAL_SIZEX 10
#define GOAL_SIZEY 100


using namespace std;

int life = 3;		//プレイヤー残機
int score = 0;		//スコア。飛距離
bool gameover_flag = false;		//trueでゲームオーバー
bool gameclear_flag = false;	//trueでゲームクリアー
float player_move_speed = 10.0;	//プレイヤーの移動速度
float player_posX;
float player_posY;
float player_moveX_degree = 0.0f;
float player_moveY_degree = 0.0f;

float enemy1_posX = 100;
float enemy1_posY = 100;
float enemy1_speed = 0.05;
bool enemy1_up = false;
bool enemy1_goLeft = false;

float enemy2_posX = 200;
float enemy2_posY = 200;
float enemy2_speed = 0.1;
bool enemy2_up = true;
bool enemy2_goLeft = true;

float enemy3_posX = 300;
float enemy3_posY = 300;
float enemy3_speed = 0.08;
bool enemy3_up = true;
bool enemy3_goLeft = false;

class player {
private:

public:
	float player_posX;
	float player_posY;

	void player_draw() {
		glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(100, 0);
		glVertex2i(100, 100);
		glVertex2i(0, 100);
		glEnd();

	}

	void player_move_up(int up_degree) {

	}
};

void printString(float x, float y, const char* str, int length) {
	float z = -1.0f;
	glRasterPos3f(x, y, z);

	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}


void enemy_move(float enemy_posX, float enemy_posY, bool enemy_up, bool enemy_goLeft, float enemy_speed) {
	if (enemy_posY >= ENEMY_GENERATE_MAXY && !enemy_up) enemy_up = true;
	else if (enemy_posY <= ENEMY_GENERATE_MINY && enemy_up) enemy_up = false;
	if (enemy_posX >= ENEMY_GENERATE_MAXX && !enemy_goLeft) enemy_goLeft = true;
	else if (enemy_posX <= ENEMY_GENERATE_MINX && enemy_goLeft) enemy_goLeft = false;

	if (enemy_up) enemy_posY -= enemy_speed;
	else if (!enemy_up) enemy_posY += enemy_speed;

	if (enemy_goLeft) enemy_posX -= enemy_speed;
	else if (!enemy_goLeft) enemy_posX += enemy_speed;
}

float enemy_move_up(float enemy_posY, bool enemy_up, float enemy_speed) {
	//if (enemy_posY >= ENEMY_GENERATE_MAXY && !enemy_up) enemy_up = true;
	//else if (enemy_posY <= ENEMY_GENERATE_MINY && enemy_up) enemy_up = false;

	if (enemy_up) enemy_posY -= enemy_speed;
	else if (!enemy_up) enemy_posY += enemy_speed;

	return enemy_posY;
}

float enemy_move_side(float enemy_posX, bool enemy_goLeft, float enemy_speed) {
	//if (enemy_posX >= ENEMY_GENERATE_MAXX && !enemy_goLeft) enemy_goLeft = true;
	//else if (enemy_posX <= ENEMY_GENERATE_MINX && enemy_goLeft) enemy_goLeft = false;

	if (enemy_goLeft) enemy_posX -= enemy_speed;
	else if (!enemy_goLeft) enemy_posX += enemy_speed;

	return enemy_posX;
}

bool enemy_move_left_or_right(float enemy_posX, bool _enemy_goLeft) {
	if (enemy_posX >= ENEMY_GENERATE_MAXX && !_enemy_goLeft) return true;
	else if (enemy_posX <= ENEMY_GENERATE_MINX && _enemy_goLeft) return false;
	else return _enemy_goLeft;
}

bool enemy_up_or_down(float enemy_posY, bool _enemy_up) {
	if (enemy_posY >= ENEMY_GENERATE_MAXY && !_enemy_up) return true;
	else if (enemy_posY <= ENEMY_GENERATE_MINY && _enemy_up) return false;
	else return _enemy_up;
}

//float calculate_distance_player_to_enemy() {
//
//}

void collision_enemy() {

}

void key_press(unsigned char key, int x, int y) {
	if (!gameover_flag && !gameclear_flag) {
		switch (key) {
		case 'w':
			//上移動
			if (player_posY > 0) player_moveY_degree -= player_move_speed;
			break;
		case 'a':
			//左移動
			if (player_posX > 0) player_moveX_degree -= player_move_speed;
			break;
		case 's':
			//下移動
			if (player_posY < WINDOW_HEIGHT - (PLAYER_SIZE / 2)) player_moveY_degree += player_move_speed;
			break;
		case 'd':
			//右移動
			if (player_posX < WINDOW_WIDTH - (PLAYER_SIZE / 2)) player_moveX_degree += player_move_speed;
			break;
		case 'q':	//ゲーム終了
			exit(0);
			break;
		default:
			break;
		}
	}
	else {
		switch (key) {
		case 'q':	//ゲーム終了
			exit(0);
			break;
		default:
			break;
		}
	}
}

//敵の描画
void draw_enemy(float enemy_posX, float enemy_posY) {
	glColor4f(1.0f, 0, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2i(enemy_posX, enemy_posY);
	glVertex2i(enemy_posX - ENEMY_SIZE / 2, enemy_posY + ENEMY_SIZE);
	glVertex2i(enemy_posX + ENEMY_SIZE / 2, enemy_posY + ENEMY_SIZE);
	glEnd();
}

void initialize() {

	//player_posX = PLAYER_START_POSX;
	//player_posY = PLAYER_START_POSY;

	srand(time(NULL));

	//第1の敵の初期座標生成
	enemy1_posX = ENEMY_GENERATE_MINX + rand() % (ENEMY_GENERATE_MAXX - ENEMY_GENERATE_MINX + 1);
	enemy1_posY = ENEMY_GENERATE_MINY + rand() % (ENEMY_GENERATE_MAXY - ENEMY_GENERATE_MINY + 1);

	//第2の敵の初期座標生成
	enemy2_posX = ENEMY_GENERATE_MINX + rand() % (ENEMY_GENERATE_MAXX - ENEMY_GENERATE_MINX + 1);
	enemy2_posY = ENEMY_GENERATE_MINY + rand() % (ENEMY_GENERATE_MAXY - ENEMY_GENERATE_MINY + 1);


	//第3の敵の初期座標生成
	enemy3_posX = ENEMY_GENERATE_MINX + rand() % (ENEMY_GENERATE_MAXX - ENEMY_GENERATE_MINX + 1);
	enemy3_posY = ENEMY_GENERATE_MINY + rand() % (ENEMY_GENERATE_MAXY - ENEMY_GENERATE_MINY + 1);
}


void reshape(int x, int y) {
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, x, y, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}


//ゲームに映る画面の詳細
void display() {
	// 画面全体を指定した色で塗りつぶす 
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//テキストUI表示
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);	//黒
	printString(0,20,"q:End",5);

	//プレイヤーの位置取得
	player_posX = PLAYER_START_POSX + player_moveX_degree;
	player_posY = PLAYER_START_POSY + player_moveY_degree;

	//敵1の移動処理
	//if (enemy1_posY >= ENEMY_GENERATE_MAXY && !enemy1_up) enemy1_up = true;
	//else if(enemy1_posY <= ENEMY_GENERATE_MINY && enemy1_up) enemy1_up = false;
	//if (enemy1_posX >= ENEMY_GENERATE_MAXX && !enemy1_goLeft) enemy1_goLeft = true;
	//else if (enemy1_posX <= ENEMY_GENERATE_MINX && enemy1_goLeft) enemy1_goLeft = false;

	//if (enemy1_up) enemy1_posY -= enemy1_speed;
	//else if (!enemy1_up) enemy1_posY += enemy1_speed;

	//if (enemy1_goLeft) enemy1_posX -= enemy1_speed;
	//else if (!enemy1_goLeft) enemy1_posX += enemy1_speed;

	//enemy_move(enemy1_posX, enemy1_posY, enemy1_up, enemy1_goLeft, enemy1_speed);
	enemy1_up = enemy_up_or_down(enemy1_posY, enemy1_up);
	enemy1_goLeft = enemy_move_left_or_right(enemy1_posX,enemy1_goLeft);
	enemy1_posX = enemy_move_side(enemy1_posX, enemy1_goLeft, enemy1_speed);
	enemy1_posY = enemy_move_up(enemy1_posY, enemy1_up, enemy1_speed);

	//敵2の移動処理
	//if (enemy2_posY >= ENEMY_GENERATE_MAXY && !enemy2_up) enemy2_up = true;
	//else if (enemy2_posY <= ENEMY_GENERATE_MINY && enemy2_up) enemy2_up = false;
	//if (enemy2_posX >= ENEMY_GENERATE_MAXX && !enemy2_goLeft) enemy2_goLeft = true;
	//else if (enemy2_posX <= ENEMY_GENERATE_MINX && enemy2_goLeft) enemy2_goLeft = false;

	//if (enemy2_up) enemy2_posY -= enemy2_speed;
	//else if (!enemy2_up) enemy2_posY += enemy2_speed;

	//if (enemy2_goLeft) enemy2_posX -= enemy2_speed;
	//else if (!enemy2_goLeft) enemy2_posX += enemy2_speed;

	//enemy_move(enemy2_posX, enemy2_posY, enemy2_up, enemy2_goLeft, enemy2_speed);
	enemy2_up = enemy_up_or_down(enemy2_posY, enemy2_up);
	enemy2_goLeft = enemy_move_left_or_right(enemy2_posX, enemy2_goLeft);
	enemy2_posX = enemy_move_side(enemy2_posX, enemy2_goLeft, enemy2_speed);
	enemy2_posY = enemy_move_up(enemy2_posY, enemy2_up, enemy2_speed);

	//敵3の移動処理
	//if (enemy3_posY >= ENEMY_GENERATE_MAXY && !enemy3_up) enemy3_up = true;
	//else if (enemy3_posY <= ENEMY_GENERATE_MINY && enemy3_up) enemy3_up = false;
	//if (enemy3_posX >= ENEMY_GENERATE_MAXX && !enemy3_goLeft) enemy3_goLeft = true;
	//else if (enemy3_posX <= ENEMY_GENERATE_MINX && enemy3_goLeft) enemy3_goLeft = false;

	//if (enemy3_up) enemy3_posY -= enemy3_speed;
	//else if (!enemy3_up) enemy3_posY += enemy3_speed;

	//if (enemy3_goLeft) enemy3_posX -= enemy3_speed;
	//else if (!enemy3_goLeft) enemy3_posX += enemy3_speed;

	//enemy_move(enemy3_posX, enemy3_posY, enemy3_up, enemy3_goLeft, enemy3_speed);
	enemy3_up = enemy_up_or_down(enemy3_posY, enemy3_up);
	enemy3_goLeft = enemy_move_left_or_right(enemy3_posX, enemy3_goLeft);
	enemy3_posX = enemy_move_side(enemy3_posX, enemy3_goLeft, enemy3_speed);
	enemy3_posY = enemy_move_up(enemy3_posY, enemy3_up, enemy3_speed);

	//敵1の描画
	draw_enemy(enemy1_posX,enemy1_posY);
	//敵2の描画
	draw_enemy(enemy2_posX, enemy2_posY);
	//敵3の描画
	draw_enemy(enemy3_posX, enemy3_posY);


	//ゴール描画
	glColor4f(1.0f, 0.5f, 0.0f, 1.0f);	//オレンジ
	glBegin(GL_QUADS);
	glVertex2i(GOAL_POSX, GOAL_POSY);
	glVertex2i(GOAL_POSX + GOAL_SIZEX, GOAL_POSY);
	glVertex2i(GOAL_POSX + GOAL_SIZEX, GOAL_POSY + GOAL_SIZEY);
	glVertex2i(GOAL_POSX, GOAL_POSY + GOAL_SIZEY);
	glEnd();

	//衝突判定用デバッグ
	//cout << abs((player_posX + PLAYER_SIZE / 2) - enemy1_posX) << "X to " << abs((player_posY - PLAYER_SIZE / 2) - (enemy1_posY + ENEMY_SIZE / 2)) << "Y" << endl;

	//player(四角形)描画
	glColor4f(0.0f, 0.5f, 0.0f, 1.0f);	//緑
	glBegin(GL_QUADS);
	glVertex2i(player_posX, player_posY);
	glVertex2i(player_posX + PLAYER_SIZE, player_posY);
	glVertex2i(player_posX + PLAYER_SIZE, player_posY + PLAYER_SIZE);
	glVertex2i(player_posX, player_posY + PLAYER_SIZE);
	glEnd();

	//enemy1との衝突判定
	//工夫したところ
	if (abs((player_posX + PLAYER_SIZE / 2) - enemy1_posX) < HITBOX_SIZE && abs((player_posY + PLAYER_SIZE / 2) - (enemy1_posY + ENEMY_SIZE / 2)) < HITBOX_SIZE) {
		gameover_flag = true;
		//exit(0);
	}

	//enemy2との衝突判定
	//工夫したところ
	if (abs((player_posX + PLAYER_SIZE / 2) - enemy2_posX) < HITBOX_SIZE && abs((player_posY + PLAYER_SIZE / 2) - (enemy2_posY + ENEMY_SIZE / 2)) < HITBOX_SIZE) {
		gameover_flag = true;
		//exit(0);
	}

	//enemy3との衝突判定
	if (abs((player_posX + PLAYER_SIZE / 2) - enemy3_posX) < HITBOX_SIZE && abs((player_posY + PLAYER_SIZE / 2) - (enemy3_posY + ENEMY_SIZE / 2)) < HITBOX_SIZE) {
		gameover_flag = true;
		//exit(0);
	}

	//Goalとの衝突判定。つまりゲームクリア
	if (abs(player_posX - GOAL_POSX) < GOAL_SIZEX && abs(player_posY - GOAL_POSY) < GOAL_SIZEY) {
		gameclear_flag = true;
		//exit(0);
	}

	//ゲームオーバー時の処理
	if (gameclear_flag) {
		glColor4f(1.0f, 0.6f, 0.0f, 1.0f);	//オレンジ
		printString(250, 450, "GAME CLEAR!", 11);
		printString(250, 480, "CONGRATULATION!!", 16);
		printString(250, 510, "YOU CAN QUIT GAME BY PUSHING Q", 30);
	}
	//ゲームクリア時の処理
	if (gameover_flag) {
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);	//黒
		printString(250, 450, "GAME OVER", 9);
		printString(250, 480, "I LOOK FORWARD TO YOUR RETRY!", 29);
		printString(250, 510, "YOU CAN QUIT GAME BY PUSHING Q", 30);
	}

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	//ゲーム開始前にコマンドプロンプトでゲームの難易度やプレイヤーの速度を変更できるようにしたい

	initialize();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("YokeGame");
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_press);
	glutDisplayFunc(display);

	//背景白色に
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glutIdleFunc(display);

	//ゲームループ
	glutMainLoop();

	if (gameover_flag) printf("Dont mind! You can do it!\n");
	else if (gameclear_flag) printf("Congratulation!\n");
}