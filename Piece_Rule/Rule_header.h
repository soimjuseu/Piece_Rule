int currentRow, currentCol, targetRow, targetCol; // 현재 위치 판별 함수
int can_Move_Pawn; // 폰 이동 가능 여부
int can_Move_Rook;
int can_Move_Knight;
int can_Move_Bishop;
int can_Move_Queen;
int can_Move_King;
int White_pice; //백 기물
int Black_piece; //흑 기물
int isPathClear; //장애물 판별
int My_piece; //내 기물인지 판별
int other_piece; //상대방의 기물인지 판별
int Empty(int col, int row);