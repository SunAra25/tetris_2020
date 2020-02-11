#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define e 101
#define s 115

void gotoxy(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void main() {

	srand((unsigned)time(NULL));
	int i, j, p, q, k, t;  // 반복문을 위한 변수
	int absx = 8, absy = 0;  //블럭의 절대 좌표 == 블럭이 나타나는 처음 좌표 
	int curx = 8, cury = 0;  //블럭의 현재 좌표 == 계속 바뀜
	int form = rand() % 7;  //블럭의 모양을 랜덤으로 구함
	int left = 0;  //blocks가 옆 벽에 닿을 경우 사용
	int right = 0;  //blocks가 옆 벽에 닿을 경우 사용
	int count = 0;  //블록이 한 줄 다 찼을 경우 1씩 증가시키기 위해 사용
	int stop = 0;  //블록이 바닥, 혹은 아래에 있는 블록에 닿을 경우 사용
	int die = 0;  //게임이 끝날 경우 사용
	int input = 0;  //키보드를 입력받을 때 사용
	int time = 100;

	int map[32][22];  //테트리스 벽

	int blocks[7][4][4] = { 0,0,0,0,
		0,1,1,0,
		0,1,1,0,
		0,0,0,0,

		0,1,0,0,
		0,1,0,0,
		0,1,0,0,
		0,1,0,0,

		0,0,1,0,
		0,1,1,0,
		0,1,0,0,
		0,0,0,0,

		0,0,0,0,
		0,1,0,0,
		1,1,1,0,
		0,0,0,0,

		0,0,0,0,
		0,0,1,0,
		1,1,1,0,
		0,0,0,0,

		0,0,1,0,
		0,0,1,0,
		0,1,1,0,
		0,0,0,0,

		0,0,0,0,
		0,1,1,0,
		1,1,0,0,
		0,0,0,0 };   //7가지 모양의 블록들

	int subblock[7][4][4] = { 0, }; // blocks 모양을 돌린 걸 저장하는 배열


start:

	while (1) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
		gotoxy(31, 10);
		printf("← : LEFT    → : RIGHT    ↑ : ROTATION    ↓ : DOWN");
		gotoxy(26, 15);
		printf(" =========================== TETRIS =========================== \n");
		gotoxy(48, 20);
		printf("PRESS 'S' TO START"); {

			if (kbhit()) {
				input = getch();

				switch (input)
				{
				case s: //'s'를 누르면

					system("cls");  //출력했던 걸 다 지우고

					goto end;//end로 간다 == 게임 시작

					break;
				}
			}
		}
	}
end:

	while (1) {

		gotoxy(0, 0); //(0,0)으로 가서 
		for (i = 0; i < 30; i++)
			for (j = 0; j < 20; j++)
				if (i != 29 && j != 0 && j != 19)
					map[i][j] = 0;
				else
					map[i][j] = 1; //i==29 이고 j==0 이거나 i==29 이고 j==19 이면 map에 1 대입 -> 벽
		while (1) {

			form = rand() % 7; //while문에 들어 올 때마다 모양 랜덤으로 출력
			stop = 0;  //바뀐 값 초기화

			gotoxy(0, 0);  //(0,0)으로 가서
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			for (i = 0; i < 30; i++) {
				for (j = 0; j < 20; j++)
					if (map[i][j] == 1)  //map==1이면
						printf("#");  //#을 출력
					else
						printf(".");  // 아니라면 . 출력  -->  블록 위치를 잘 보기 위함!

				printf("\n");
			}

			for (k = 1; k <= 28; k++) {
				count = 0;  //줄이 바뀔때마다 초기화
				for (t = 1; t <= 18; t++) {
					if (map[k][t] == 0)  //가로로 한줄씩 1인지 0인지 확인
						break;
					count += 1; //만약 1이면 count를 1씩 증가
				}
				if (count == 18) {  //그러다가 count가 18이 되면
					for (p = k; p >= 1; p--) //꽉 찬 줄을 기준으로
						for (q = 1; q < 18; q++)
							map[p][q] = map[p - 1][q]; //그 위의 줄을 한줄 아래로 옮기기
					for (q = 1; q < 18; q++)
						map[0][q] = 0; // map의 맨 윗줄을 0으로

					Sleep(time); // 0.1초 후
					system("cls");  // 출력한 걸 다 지우고 

					for (i = 0; i < 30; i++) {
						for (j = 0; j < 20; j++)
							if (map[i][j] == 1) // 지금까지 map에 넣은 값에 따라 
								printf("#");
							else
								printf(".");  //다시 map 출력

						printf("\n");

					}
				}
			}




			curx = 8;
			cury = 0;  //현재 좌표를 처음 좌표로

			gotoxy(curx, cury); //처음 좌포에서 # 그려주기
			while (1) { //무한반복
				if (stop == 1)
					break;

				switch (input) //눌린 버튼에 따른 좌표 변경
				{
				case LEFT:
					for (i = 0; i < 4; i++) {
						for (j = 0; j < 4; j++) {
							if ((blocks[form][i][j] == 1) && (map[cury + i][curx + j - 1] == 1))
								//blocks 값이 1인 동시에 블럭의 한칸 왼쪽의 map의 값이 1 이면 
								left = 1; //left를 1로 변경
						}
					}
					if (left == 0)  //만약 left가 0이라면
						curx -= 1;  // 블럭 왼쪽으로 한 칸 이동

					left = 0; //다시 left를 0으로 초기화 -> 또 확인을 해야하기 때문!

					cury++; // 방향키를 눌러도 계속 아래로 내려감

					break;

				case RIGHT:
					for (i = 0; i < 4; i++) {
						for (j = 0; j < 4; j++) {
							if ((blocks[form][i][j] == 1) && (map[cury + i][curx + j + 1] == 1))
								//만약 blocks가 1인 동시에 블럭의 오른쪽의 map이 1이라면
								right = 1;  //right 1로 변경
						}
					}
					if (right == 0)  //right가 0이라면
						curx += 1; // 블럭 오른쪽으로 한 칸 이동

					right = 0;  //right를 다시 0으로 변경

					cury++;

					break;

				case UP:    // 블럭 오른쪽으로 90도 회전하기 위함
					for (j = 0; j < 4; j++)
						for (i = 3; i >= 0; i--)
							subblock[form][j][(3 - i)] = blocks[form][i][j];  //blocks를 오른쪽으로 돌린 것을 subblock에 넣음
					for (i = 0; i < 4; i++)
						for (j = 0; j < 4; j++)
							blocks[form][i][j] = subblock[form][i][j];  //회전시킨 블럭을 다시 blocks에 넣음

					for (i = 0; i < 4; i++) {
						if ((blocks[form][i][0] == 1) && (map[cury + i][curx + 0] == 1))
							//만약 blocks의 왼쪽이 1인 동시에 blocks의 좌표가 (curx,cury)일때 map의 왼쪽 벽이 1이라면
							curx += 1; //curx를 1씩 증가
					}

					for (i = 0; i < 4; i++) {
						if ((blocks[form][i][3] == 1) && (map[cury + i][curx + 3] == 1))
							//만약 blocks의 오른쪽이 1인 동시에 blocks의 좌표가 (curx,cury)일때 map의 오른쪽 벽이 1이라면
							curx -= 1; //curx를 1씩 감소
					}

					cury += 1; //방향을 바꾸는 동시에 계속 내려감

					break;

				case DOWN:
					while (1) {
						for (i = 0; i < 4; i++) {
							for (j = 0; j < 4; j++) {
								if ((blocks[form][i][j] == 1) && (map[cury + i + 1][curx + j] == 1)) {
									//만약 blocks가 1인 동시에 map 위의 blocks의 현재 y좌표의 한칸 아래인 곳이 1이면
									stop = 1;  //stop에 1 대입
									break;
								}
							}if (stop)
								break;
						}
						if (stop == 1)
							break;
						else
							cury += 1;
					}

					break;

				}
				input = 0;
				while (1) {

					gotoxy(curx, cury); //현재 좌표로 가서 blocks 그리기
					for (i = 0; i < 4; i++) {
						for (j = 0; j < 4; j++) {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
							if ((blocks[form][i][j] == 1) || (map[cury + i][curx + j] == 1)) {
								//만약 blocks가 1이거나 map위의 blocks의 현재 좌표의 값이 1이라면
								printf("#");
							}
							else {
								SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
								printf(".");
							}
							if ((blocks[form][i][j] == 1) && (map[cury + i + 1][curx + j] == 1))
								//만약 blocks가 1인 동시에 map 위의 blocks의 현재 y좌표의 한칸 아래인 곳이 1이면
								stop = 1;  //stop에 1 대입

						}gotoxy(curx, cury + 1 + i);  //현재의 y좌표보다 한칸 아래로 이동

					}

					Sleep(time);  //0.1초 후에
					gotoxy(curx, cury);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
					for (i = 0; i < 4; i++) {
						for (j = 0; j < 4; j++) {
							if (map[cury + i][curx + j] == 1)  //map의 현재 좌표의 값이 1이라면
								printf("#");
							else
								printf(".");
						}
						gotoxy(curx, cury + 1 + i);  //현재 y좌표보다 하나 아래로 이동
					}

					if (stop == 1) {  //만약 stop이 1이라면

						for (i = 0; i < 4; i++) {
							for (j = 0; j < 4; j++) {
								if (blocks[form][i][j] == 1)  //blocks의 값이 1이라면
									map[cury + i][curx + j] = 1;  //map의 현재 좌표에 1을 대입 -> blocks을 map에 대입
							}
						}
						break;
					}


					if (kbhit()) { //kbhit는 키보드가 눌렸는지의 여부 확인
						input = getch();  //input에 키보드에서 눌린것을  대입
						gotoxy(curx, cury);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
						for (i = 0; i < 4; i++) {
							for (j = 0; j < 4; j++) {
								if (map[cury + i][curx + j] == 1)  //map의 현재 좌표의 값이 1이라면
									printf("#");
								else
									printf(".");
							}
							gotoxy(curx, cury + 1 + i);  //현재 y좌표보다 하나 아래로 이동
						}
						break;
					}
					cury += 1; //y좌표를 하나씩 늘림
				}


				if ((map[1][9] == 1) || (map[1][10] == 1) || (map[1][11] == 1))
					//map의 맨 윗줄의 가운데, 즉 blocks가 처음 출력되는 부분이 1이라면
					die = 1;  //die에 1 대입

				if (die == 1) {  //die에 1을 대입하면
					for (i = 0; i < 30; i++) {
						for (j = 0; j < 20; j++) {
							map[i][j] = 0;  //map을 다 0으로 만들고
							system("cls");  //출력한 것을 다 지우고
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
							gotoxy(55, 13);
							printf("DIE");

							gotoxy(46, 15);

							printf("PRESS 'S' TO RESTART");

							gotoxy(48, 17);
							printf("PRESS 'E' TO END ");

							gotoxy(40, 18);
							printf("\n\n");



							if (kbhit) {
								input = getch();
								switch (input) {
								case (s):

									system("cls");  //다 지우고

									goto start;  //start로 다시 가기 -> 맨 처음 시작 화면으로

									break;

								case (e):

									return 0;





								}
							}
						}
					}
				}

			}
		}


	}
	system("pause");
}