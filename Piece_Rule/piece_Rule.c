#include <stdio.h>
#include <stdlib.h>
#include "Rule_header.h"
#include <SDL2/SDL.h>
#include <ctype.h>


//모든 코드를 합한 최종 이동 판별 코드
int move_piece(int currentCol, int currentRow, int targetCol, int targetRow) {

	if ((currentRow < 0 || currentRow >= 8) ||
		currentCol < 0 || currentCol >= 8 ||
		targetRow < 0 || targetRow >= 8 ||
		targetCol < 0 || targetCol >= 8) {return 0;}
	//좌표가 8x8을 벗어나는지 검사 


char start_location = board[currentRow][currentCol];
char target_location = board[targetRow][targetCol];
if (start_location == '.') { return 0; }
//좌표 검사에서 문제가 없다면 현재좌표/도착지좌표를 각각 start/target에 저장 후, 만약 '.' 
// 즉, 빈칸이라면 0 반환



	
	

	if (can_move_piece(currentRow, currentCol, targetRow, targetCol))
	{
		board[currentRow][currentCol] = '.';
		board[targetRow][targetCol] = start_location;
	}
	
	if (target_location != '.') {
		if (isupper(start_location) == isupper(target_location)) {return 0;}
			

	}	
}




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

//임시로 작성 중이시던 코드 좀 베껴옴(원할한 테스트를 위해)


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

//폰의 타겟 칸이 프로모션을 실행하기 위한 칸에 도착하는지/했는지 판별하는 함수
int Pawn_promotion(int currentRow, int currentCol, int targetRow, int targetCol) {
	if ((board[currentCol][currentRow] == 'P' || 
		board[currentCol][currentRow] == 'p')
		&&
		(targetRow == 7 || targetRow == 0)) {return 1;}
	else { return 0; }
}

//폰의 실제 프로모션 구현 함수
//void promote_Pawn(int targetCol, int targetRow, char Select_piece) {
	//board[targetRow][targetCol] = Select_piece;


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

int can_Move_Pawn(int currentRow, int currentCol, int targetRow, int targetCol, int white_piece, int My_piece, int other_piece) { //white부터 3개의 함수는 테스트용 임의의 함수
	if (white_piece == 1 && targetRow == currentRow - 1 && targetCol == currentCol) { return 1; }
	//백 기물 턴이면 1칸 전진(col이 같아야 return 되기에 전진만 가능) 로직
	else if (white_piece == 0 && targetRow == currentRow + 1 && targetCol == currentCol) { return 1; }
	//흑 기물 턴이면 1칸 전진(col이 같아야 return 되기에 전진만 가능) 로직
	else if (white_piece == 1 && currentRow == 6 && targetRow == currentRow - 2 && targetCol == currentCol) { return 1; }
	//백 기물 턴이면서, 시작위치에 있다면 2칸 이동(첫 턴 2칸 이동 로직)
	else if (white_piece == 0 && currentRow == 1 && targetRow == currentRow + 2 && targetCol == currentCol) { return 1; }
	//흑 기물 턴이면서, 시작위치에 있다면 2칸 이동(첫 턴 2칸 이동 로직)
	else if (white_piece == 1 && targetRow == currentRow - 1 &&
		(abs(targetRow - currentRow) == 1 &&
			abs(targetCol - currentCol) == 1 &&
			other_piece == 1))
	{
		return 1;
	} //백 기준 대각잡 판정 로직

	else if (white_piece == 0 && targetRow == currentRow + 1 &&
		(abs(targetRow - currentRow) == 1 &&
			abs(targetCol - currentCol) == 1 &&
			other_piece == 1))
	{
		return 1;
	} //흑 기준 대각잡 판정 로직



		else { return 0; }

}


 //여기부터 테스트용 출력 코드입니다.
int main() {

	printf("Rook   (3,2) -> (3,7): %d\n", can_Move_Rook(3, 2, 3, 7));
	printf("Rook   (3,2) -> (7,2): %d\n", can_Move_Rook(3, 2, 7, 2));
	printf("Rook   (3,2) -> (7,5): %d\n", can_Move_Rook(3, 2, 7, 5));
	printf("Rook   (3,2) -> (3,2): %d\n", can_Move_Rook(3, 2, 3, 2));

	printf("Knight (3,2) -> (5,3): %d\n", can_Move_Knight(3, 2, 5, 3));
	printf("Knight (3,2) -> (4,4): %d\n", can_Move_Knight(3, 2, 4, 4));
	printf("Knight (3,2) -> (5,4): %d\n", can_Move_Knight(3, 2, 5, 4));
	printf("Knight (3,2) -> (3,2): %d\n", can_Move_Knight(3, 2, 3, 2));

	printf("Bishop (3,2) -> (4,3): %d\n", can_Move_Bishop(3, 2, 4, 3));
	printf("Bishop (3,2) -> (5,4): %d\n", can_Move_Bishop(3, 2, 5, 4));
	printf("Bishop (3,2) -> (4,4): %d\n", can_Move_Bishop(3, 2, 4, 4));
	printf("Bishop (3,2) -> (3,2): %d\n", can_Move_Bishop(3, 2, 3, 2));

	printf("Queen  (3,2) -> (3,7): %d\n", can_Move_Queen(3, 2, 3, 7));
	printf("Queen  (3,2) -> (7,2): %d\n", can_Move_Queen(3, 2, 7, 2));
	printf("Queen  (3,2) -> (5,4): %d\n", can_Move_Queen(3, 2, 5, 4));
	printf("Queen  (3,2) -> (5,5): %d\n", can_Move_Queen(3, 2, 5, 5));

	printf("King   (3,2) -> (3,3): %d\n", can_Move_King(3, 2, 3, 3));
	printf("King   (3,2) -> (4,3): %d\n", can_Move_King(3, 2, 4, 3));
	printf("King   (3,2) -> (5,2): %d\n", can_Move_King(3, 2, 5, 2));
	printf("King   (3,2) -> (3,2): %d\n", can_Move_King(3, 2, 3, 2));

	// 백색 1칸 전진
	printf("Pawn W  (6,3) -> (5,3): %d\n",
		can_Move_Pawn(6, 3, 5, 3, 1, 1, 0));

	// 흑색 1칸 전진
	printf("Pawn B  (1,3) -> (2,3): %d\n",
		can_Move_Pawn(1, 3, 2, 3, 0, 1, 0));

	// 백색 첫 2칸
	printf("Pawn W  (6,3) -> (4,3): %d\n",
		can_Move_Pawn(6, 3, 4, 3, 1, 1, 0));

	// 흑색 첫 2칸
	printf("Pawn B  (1,3) -> (3,3): %d\n",
		can_Move_Pawn(1, 3, 3, 3, 0, 1, 0));

	// 백색 대각선 잡기
	printf("Pawn W  (5,3) -> (4,2): %d\n",
		can_Move_Pawn(5, 3, 4, 2, 1, 1, 1));

	// 백색 반대쪽 대각선 잡기
	printf("Pawn W  (5,3) -> (4,4): %d\n",
		can_Move_Pawn(5, 3, 4, 4, 1, 1, 1));

	// 흑색 대각선 잡기
	printf("Pawn B  (2,3) -> (3,2): %d\n",
		can_Move_Pawn(2, 3, 3, 2, 0, 1, 1));

	// 흑색 반대쪽 대각선 잡기
	printf("Pawn B  (2,3) -> (3,4): %d\n",
		can_Move_Pawn(2, 3, 3, 4, 0, 1, 1));

	// 옆으로 이동 → 실패
	printf("Pawn W  (5,3) -> (5,4): %d\n",
		can_Move_Pawn(5, 3, 5, 4, 1, 1, 0));

	// 대각선인데 상대 말 없음 → 실패
	printf("Pawn W  (5,3) -> (4,4): %d\n",
		can_Move_Pawn(5, 3, 4, 4, 1, 1, 0));

	// 백색 폰이 시작 위치가 아닌 곳에서 2칸 → 실패
	printf("Pawn W  (5,3) -> (3,3): %d\n",
		can_Move_Pawn(5, 3, 3, 3, 1, 1, 0));

	printf("1. 가로 이동: %d\n",
		Is_path_Clear(3, 1, 3, 4));

	printf("2. 세로 이동: %d\n",
		Is_path_Clear(2, 3, 5, 3));

	printf("3. 대각선 오른쪽 아래: %d\n",
		Is_path_Clear(2, 2, 5, 5));

	printf("4. 대각선 오른쪽 위: %d\n",
		Is_path_Clear(5, 2, 2, 5));

	printf("5. 대각선 왼쪽 아래: %d\n",
		Is_path_Clear(2, 5, 5, 2));

	printf("6. 대각선 왼쪽 위: %d\n",
		Is_path_Clear(5, 5, 2, 2));

	printf("7. 같은 칸 이동: %d\n",
		Is_path_Clear(3, 3, 3, 3));


	return 0;
}





