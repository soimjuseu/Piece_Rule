#include <stdio.h>
#include <stdlib.h>
#include "Rule_header.h"
#include <SDL2/SDL.h>
#include <ctype.h>


int promotion_mode = 0;
int promotionCol = -1;
int promotionRow = -1;

char board[BOARD_SIZE][BOARD_SIZE] = {
{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
{'.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.'},
{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};


int move_piece(int currentRow, int currentCol, int targetRow, int targetCol) {

	if (promotion_mode == 1) {
		return 0;
	}
	//폰 승급 진행 중에는 다른 기물 이동 제한

	if ((currentRow < 0 || currentRow >= 8) ||
		currentCol < 0 || currentCol >= 8 ||
		targetRow < 0 || targetRow >= 8 ||
		targetCol < 0 || targetCol >= 8) {
		return 0;
	}
	// 1.기물의 좌표가 8x8(보드칸 크기) 을 벗어나는지 검사 

	char start_location = board[currentRow][currentCol];
	char target_location = board[targetRow][targetCol];
	if (start_location == '.') { return 0; }
	// 2.출발 칸과 목표 칸의 기물 정보를 저장

	if (target_location != '.') {
		if (isupper(start_location) == isupper(target_location)) { return 0; }
	}
	// 3.이동하려는 목표 칸에 같은 편 기물이 있는 지 검사 (보드 판상 알파벳으로 되어있기에 두개의 기물을 
	// 모두 대문자로 올리고 P=P 이런식으로 같은 편인지 체크.)

	if (can_move_piece(currentRow, currentCol, targetRow, targetCol))
	{
		board[currentRow][currentCol] = '.';
		board[targetRow][targetCol] = start_location;
		if (Pawn_promotion(targetRow, targetCol)) {
			promotionCol = targetCol;
			promotionRow = targetRow;
			promotion_mode = 1;
			//폰 승급 연결
		}
		return 1;
		// 4. 3번까지 문제가 없었다면 can_move_piece 함수를 호출해 각 기물별 규칙 호출
	}
	else {
		return 0;
	}
}

//현재 위치의 기물에 맞는 기물 이동 판별로 넘기는 코드
//ex) current_col/row를 보니 'R'(RooK)이라면 can_Move_Rook으로 넘김
int can_move_piece(int currentRow, int currentCol, int targetRow, int targetCol) {
	if (Rook) {
		return can_Move_Rook(currentRow, currentCol, targetRow, targetCol);
		}

	if (Pawn) {
		return can_Move_Pawn(currentRow, currentCol, targetRow, targetCol);
	}

	if (Bishop) {
		return can_Move_Bishop(currentRow, currentCol, targetRow, targetCol);
	}
	
	if (Knight) {
		return can_Move_Knight(currentRow, currentCol, targetRow, targetCol);
	}

	if (Queen) {
		return can_Move_Queen(currentRow, currentCol, targetRow, targetCol);
	}

	if (King) {
		return can_Move_King(currentRow, currentCol, targetRow, targetCol);
	}
	return 0;
}

//폰의 타겟 칸이 승급을 실행하기 위한 칸에 도착하는지/했는지 판별하는 함수
int Pawn_promotion(int targetRow, int targetCol) {
	if ((board[targetRow][targetCol] == 'P' || 
		board[targetRow][targetCol] == 'p')
		&&
		(targetRow == 7 || targetRow == 0)) {return 1;}
	
	else { return 0; }
}





//폰의 실제 승급 구현 함수(SDL - 마우스 구현 코드 필요)
int promote_Pawn(int targetCol, int targetRow, char Select_piece)  {
	//Select_piece는 SDL-마우스로 누른 값을 저장하는 변수
	//여기쯤에 SDL을 넣어야 하는데
	if (board[targetRow][targetCol] == 'P') {
	   if   (Select_piece == 'Q' ||
			Select_piece == 'R' ||
			Select_piece == 'B' ||
			Select_piece == 'N')
		   //마우스로 누른값 확인(실제 게임상에선 선택 창 UI가 뜰꺼임)

	   {
		   board[targetRow][targetCol] = Select_piece; 
		   promotion_mode = 0;
		   return 1;
	   }
		else {return 0;}

	}
	else if (board[targetRow][targetCol] == 'p') {
		if
			   (Select_piece == 'q' ||
				Select_piece == 'r' ||
				Select_piece == 'b' ||
				Select_piece == 'n') {
			board[targetRow][targetCol] = Select_piece;
			promotion_mode = 0;
			return 1;
		}
		else { return 0; }
			
	}	
	return 0;

	board[targetRow][targetCol] = Select_piece;
}	


int can_Move_Rook(int currentRow, int currentCol, int targetRow, int targetCol) //
{

	if (currentRow == targetRow && currentCol == targetCol) { return 0; }

	else if (currentCol == targetCol || currentRow == targetRow)


	{
		return Is_path_Clear(currentRow, currentCol, targetRow, targetCol);
	}
	else { return 0; }
	//룩은 거리제한은 없지만 방향제한(열,행)은 있음 따라서 행=행 혹은 열=열이 만족될 때 이동 허가 로직


}

int Is_path_Clear(int currentRow, int currentCol, int targetRow, int targetCol) {
	

			//세로
	if (currentCol == targetCol && currentRow == targetRow) {return 0;}
	if (currentCol < targetCol && currentRow == targetRow || (currentCol > targetCol && currentRow == targetRow)) {

		for (currentCol; currentCol != targetCol;) {
			if (currentCol < targetCol) {
				currentCol++;
			}
			else if (currentCol > targetCol) {
				currentCol--;
			}

			if (currentCol == targetCol) { return 1; }

			else {
				if (board[currentRow][currentCol] != '.') { return 0; }
			}



			
		}
			return 1;
	}	
	   //가로
	if (currentRow < targetRow && currentCol == targetCol || (currentRow > targetRow && currentCol == targetCol)) {
		for (currentRow; currentRow != targetRow;) {
			if (currentRow < targetRow) {
				currentRow++;
			}
			else if (currentRow > targetRow) {
				currentRow--;
			}

			if (currentRow == targetRow) { return 1; }

			else {
				if (board[currentRow][currentCol] != '.') { return 0; }
			}

		}

		
	}
	//대각선
	if (abs(targetCol - currentCol) == (abs(targetRow - currentRow)))
	{
			for (; currentCol != targetCol && currentRow != targetRow;) {
				if (currentCol < targetCol && currentRow < targetRow) {
						currentCol++, currentRow++;
				}
						
				else if (currentCol > targetCol && currentRow > targetRow) {
					currentCol--, currentRow--;
				}

				else if (currentCol > targetCol && currentRow < targetRow) {
					currentCol--, currentRow++;
				}

				else if (currentCol < targetCol && currentRow > targetRow) {
					currentCol++, currentRow--;
				}
				if (currentCol == targetCol && currentRow == targetRow) { return 1; }

				else {
						if (board[currentRow][currentCol] != '.') { return 0; }
				}
						
					
				}
			
			return 1;
		}
	return 0;
}
int can_Move_Knight(int currentRow, int currentCol, int targetRow, int targetCol) {
	if (currentRow == targetRow && currentCol == targetCol) { return 0; }
	else if (abs(currentRow - targetRow) == 2 && (abs(currentCol - targetCol) == 1)
		||
		(abs(currentRow - targetRow) == 1 && (abs(currentCol - targetCol) == 2))) {
		return 1;
	}
	else { return 0; }

	//나이트는 직진 2 옆 1 이라는 특이한 로직이 있음. 따라서 이론상 8방향 모두 이동이 가능함 그렇기에 abs(절대값)을 통해 행2열1 또는 행1열2 이라는 조건이 만족될 때 이동 허가 하는 로직


}

int can_Move_Bishop(int currentRow, int currentCol, int targetRow, int targetCol) {
	if (currentRow == targetRow && currentCol == targetCol) { return 0; }
	else if (abs(targetRow - currentRow) == (abs(targetCol - currentCol))) { return Is_path_Clear(currentRow, currentCol, targetRow, targetCol); }
	else { return 0; }

	//비숍은 4방향으로 "대각선"만 충족된다면 거리제한 없이 이동이 가능함. 여기서 "대각선" 이기에 target에서 current를 뺀 값이 col/row 모두 동일하다면 대각선 판정이 나옴
	//따라서 coㅣ,row값이 같은지 확인하면서 제자리가 아니라는 것이 확인되면 이동을 허가하는 로직


}

int can_Move_Queen(int currentRow, int currentCol, int targetRow, int targetCol) {

	if (currentRow == targetRow && currentCol == targetCol) { return 0; }
	else if ((currentCol == targetCol
		||
		currentRow == targetRow //1.상하좌우 
		||
		abs(targetRow - currentRow) == abs(targetCol - currentCol) //2.대각선
		)) {
		return Is_path_Clear(currentRow, currentCol, targetRow, targetCol);
	}

	else { return 0; }
}

//나름 복잡해보이지만 비숍 + 룩을 합친거와 같다. 다음 조건 2가지중 하나를 만족하면 이동를 허가하는 로직.
//제자리가 아니면서 -> 1.row=row 혹은 col=col을 만족하는가(상하좌우,룩) 2.target에서 current를 뺀 row/col의 값이 동일한가(대각선,비숍)

int can_Move_King(int currentRow, int currentCol, int targetRow, int targetCol) {

	if (currentRow == targetRow && currentCol == targetCol) { return 0; }
	else if ((abs(currentCol - targetCol) <= 1
		&&
		(abs(currentRow - targetRow) <= 1)))
	{
		return 1;
	}


	else { return 0; }




}

int can_Move_Pawn(int currentRow, int currentCol, int targetRow, int targetCol) { //white부터 3개의 함수는 테스트용 임의의 함수

int direction = 0;

	if (board[currentRow][currentCol] == 'P') { direction = -1; }
	if (board[currentRow][currentCol] == 'p') { direction = +1; }
	//중간칸 검사를 위한 별도 변수

	if (board[currentRow][currentCol] == 'P' 
		&& targetRow == currentRow - 1 
		&& targetCol == currentCol
		&& board[targetRow][targetCol] == '.' ) { return 1; }
	//백 기물 턴이면 1칸 전진(col이 같아야 return 되기에 전진만 가능) 로직 + white_piece 없애고 보드판을 통해 직접 판별하게 개선

	else if (board[currentRow][currentCol] == 'p'
		&& targetRow == currentRow + 1 
		&& targetCol == currentCol
		&& board[targetRow][targetCol] == '.') { return 1; }
	
	//흑 기물 턴이면 1칸 전진(col이 같아야 return 되기에 전진만 가능) 로직 + 이하 같음

else if (board[currentRow][currentCol] == 'P' &&
		currentRow == 6 &&
		targetRow == currentRow - 2 &&
	    board[currentRow + direction][currentCol] == '.' &&
		//중간 칸 검사
		board[targetRow][targetCol] == '.' &&
		targetCol == currentCol) { return 1; }

	//백 기물 턴이면서, 시작위치에 있다면 2칸 이동(첫 턴 2칸 이동 로직)

else if (board[currentRow][currentCol] == 'p'
		&& currentRow == 1 &&
		targetRow == currentRow + 2 &&
	board[currentRow + direction][currentCol] == '.' &&
	board[targetRow][targetCol] == '.' &&
		targetCol == currentCol) { return 1; }
	//흑 기물 턴이면서, 시작위치에 있다면 2칸 이동(첫 턴 2칸 이동 로직)

else if (board[currentRow][currentCol] == 'P' &&
		targetRow == currentRow - 1 &&
		board[targetRow][targetCol] != '.' &&
		abs(targetCol - currentCol) == 1 &&
			isupper(board[currentRow][currentCol])
			!= isupper(board[targetRow][targetCol]))
	{
		return 1;
	} //백 기준 대각잡 판정 로직

	else if (board[currentRow][currentCol] == 'p' &&
		     targetRow == currentRow + 1 &&
			board[targetRow][targetCol] != '.' &&
			abs(targetCol - currentCol) == 1 &&
			isupper(board[currentRow][currentCol]) 
			!= isupper(board[targetRow][targetCol]))
	{
		return 1;
	} //흑 기준 대각잡 판정 로직



		else { return 0; }

}
int main() {
	int running = 1;
	SDL_Event e;
	while (running) {
		//여기부분 부터가 따로 폰승급용 UI 창 + 마우스
		SDL_Window* promotionWindow = SDL_CreateWindow(
			"폰 승격",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			300,
			180,
			SDL_WINDOW_HIDDEN
		);
		if (promotion_mode == 1) {
			SDL_ShowWindow(promotionWindow);
		}
		else {
			SDL_HideWindow(promotionWindow);
		}
		SDL_Renderer* promotionRenderer =
			SDL_CreateRenderer(promotionWindow, -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(promotionRenderer, 240, 240, 240, 255);
		SDL_RenderClear(promotionRenderer);
		SDL_RenderPresent(promotionRenderer);
		int optionwidth = 300 / 4;
		(e.button.windowID == SDL_GetWindowID(promotionWindow));


		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = 0;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					//여기 아래 부분 코드부터만 따로 합치면 됨(위에는 이미 겹치는 코드단이라)
					if (e.button.windowID == SDL_GetWindowID(promotionWindow)) {
						int mouseX = e.button.x;
						int mouseY = e.button.y;

						int selectedOption = mouseX / optionwidth;
						char Select_piece;

						if (isupper(board[promotionRow][promotionCol])) {
							switch (selectedOption) {
							case 0:
								Select_piece = 'Q';
								break;
							case 1:
								Select_piece = 'R';
								break;
							case 2:
								Select_piece = 'B';
								break;
							case 3:
								Select_piece = 'N';
								break;
							default:
								return 0;
							}
						}
						else {
							switch (selectedOption) {
							case 0:
								Select_piece = 'q';
								break;
							case 1:
								Select_piece = 'r';
								break;
							case 2:
								Select_piece = 'b';
								break;
							case 3:
								Select_piece = 'n';
								break;
							default:
								return 0;
							}
							
							}
								if (promote_Pawn(promotionCol, promotionRow, Select_piece)) {
								SDL_HideWindow(promotionWindow);
						}
					}

				}
			}

		}


	}