#define BOARD_SIZE 8
#define Rook (board[currentRow][currentCol] == 'R' || board[currentRow][currentCol] == 'r')
#define Pawn (board[currentRow][currentCol] == 'P' || board[currentRow][currentCol] == 'p')
#define Knight (board[currentRow][currentCol] == 'N' || board[currentRow][currentCol] == 'n')
#define Queen (board[currentRow][currentCol] == 'Q' || board[currentRow][currentCol] == 'q')
#define King  (board[currentRow][currentCol] == 'K' || board[currentRow][currentCol] == 'k')
#define Bishop (board[currentRow][currentCol] == 'B' || board[currentRow][currentCol] == 'b')

extern char board[BOARD_SIZE][BOARD_SIZE];

extern int promotion_mode;
extern int promotionCol;
extern int promotionRow;


int Pawn_promotion(int targetRow, int targetCol);

int move_piece(int currentRow, int currentCol,
    int targetRow, int targetCol);

int can_move_piece(int currentRow, int currentCol, int targetRow, int targetCol);

int can_Move_Rook(int currentRow, int currentCol, int targetRow, int targetCol);
int can_Move_Pawn(int currentRow, int currentCol, int targetRow, int targetCol);
int can_Move_Bishop(int currentRow, int currentCol, int targetRow, int targetCol);
int can_Move_Knight(int currentRow, int currentCol, int targetRow, int targetCol);
int can_Move_Queen(int currentRow, int currentCol, int targetRow, int targetCol);
int can_Move_King(int currentRow, int currentCol, int targetRow, int targetCol);

int Is_path_Clear(int currentRow, int currentCol, int targetRow, int targetCol);






