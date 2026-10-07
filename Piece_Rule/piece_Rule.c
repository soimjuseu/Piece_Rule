#include <stdio.h>
#include <stdlib.h>
#include "Rule_header.h"

int can_Move_Rook(int currentRow, int currentCol, int targetRow, int targetCol) //
{

	if (currentRow == targetRow && currentCol == targetCol) { return 0; } 

	else if (currentCol == targetCol || currentRow == targetRow) 
	

	{return 1;}
		else {return 0;}
	//룩은 거리제한은 없지만 방향제한(열,행)은 있음 따라서 행=행 혹은 열=열이 만족될 때 이동 허가 로직


}

int isPathClear_Rook(int currentRow, int currentCol, int targetRow, int targetCol) {
	if (currentCol < targetCol) {
		for (currentCol; currentCol < targetCol; currentCol++) {
			if Empty (currentCol == 3, currentRow == 5) = {
							return 1;
			} //일단 여기부터  
				
               
			else { return 0; }
			
		}

	}
	else if (currentCol > targetCol) {
		for (currentCol; currentCol > targetCol; currentCol--) {
		}
	}
	 


}



int can_Move_Knight(int currentRow, int currentCol, int targetRow, int targetCol) {
	if (currentRow == targetRow && currentCol == targetCol) { return 0; }
	else if (abs(currentRow - targetRow) == 2 && (abs(currentCol - targetCol) == 1)
		|| 
	   (abs(currentRow - targetRow) == 1 && (abs(currentCol - targetCol) == 2))) {return 1;}
	else { return 0; }

	//나이트는 직진 2 옆 1 이라는 특이한 로직이 있음. 따라서 이론상 8방향 모두 이동이 가능함 그렇기에 abs(절대값)을 통해 행2열1 또는 행1열2 이라는 조건이 만족될 때 이동 허가 하는 로직


}

int can_Move_Bishop(int currentRow, int currentCol, int targetRow, int targetCol) {
	if (currentRow == targetRow && currentCol == targetCol) { return 0; }
	else if (abs(targetRow - currentRow) == (abs(targetCol - currentCol))) { return 1; }
	else { return 0; }

	//비숍은 4방향으로 "대각선"만 충족된다면 거리제한 없이 이동이 가능함. 여기서 "대각선" 이기에 target에서 current를 뺀 값이 col/row 모두 동일하다면 대각선 판정이 나옴
	//따라서 coㅣ,row값이 같은지 확인하면서 제자리가 아니라는 것이 확인되면 이동을 허가하는 로직


}

int can_Move_Queen(int currentRow, int currentCol, int targetRow, int targetCol) {

	  if(currentRow == targetRow && currentCol == targetCol) { return 0; }
 else if((currentCol == targetCol 
		||
		currentRow == targetRow //1.상하좌우 
		||
		abs(targetRow - currentRow) == abs(targetCol - currentCol) //2.대각선
		)) {return 1;}

	else { return 0; }
}

//나름 복잡해보이지만 비숍 + 룩을 합친거와 같다. 다음 조건 2가지중 하나를 만족하면 이동를 허가하는 로직.
//제자리가 아니면서 -> 1.row=row 혹은 col=col을 만족하는가(상하좌우,룩) 2.target에서 current를 뺀 row/col의 값이 동일한가(대각선,비숍)

int can_Move_King(int currentRow, int currentCol, int targetRow, int targetCol) {

	  if(currentRow == targetRow && currentCol == targetCol) { return 0; }
 else if((abs(currentCol - targetCol) <= 1 
		&&
		(abs(currentRow - targetRow) <=  1 )))
		{return 1;}
	

	else { return 0; }

	


}

int can_Move_Pawn(int currentRow, int currentCol, int targetRow, int targetCol, int White_piece) {
	if (currentRow == targetRow && currentCol == targetCol) { return 0; }

	else if ((currentRow == 2 //2는 임의로 정한 "시작 위치"
			 && 
			 currentCol == targetCol
			 &&
		abs(targetRow - currentRow) == 2 )) {return 1; } //시작 위치에 있다 = 첫턴이면 2칸 이동 로직
	
	else if (currentRow > 2 
			&&
			currentCol == targetCol
			&&
		abs(targetRow - currentRow) == 1 ) {return 1; }

	else { return 0; }



}

int canMovePawn(int currentRow, int currentCol, int targetRow, int targetCol, int Black_piece) {
	if (currentRow == targetRow && currentCol == targetCol) { return 0; }

	else if ((currentRow == 7 //2는 임의로 정한 "시작 위치"
		&&
		currentCol == targetCol
		&&
		abs(targetRow - currentRow) == 7)) {
		return 1;
	}





}


