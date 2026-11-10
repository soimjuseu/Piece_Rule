#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <ctype.h>
#include "Rule_header.h"

#undef main

#define BOARD_SIZE 8
#define TILE_SIZE 135
#define BOARD_PIXEL_SIZE (BOARD_SIZE * TILE_SIZE)

#define SIDEBAR_WIDTH 850
#define WINDOW_WIDTH (BOARD_PIXEL_SIZE + SIDEBAR_WIDTH)
#define WINDOW_HEIGHT BOARD_PIXEL_SIZE

HANDLE hOutput;

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

char captured_white_pieces[32];
int white_captured_count = 0;

char captured_black_pieces[32];
int black_captured_count = 0;

void init_console(void) {
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOutput, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
}

void draw_board(int selX, int selY, int mins, int secs, bool game_over) {
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hOutput, coord);

    printf("\n a b c d e f g h \n");
    printf(" ┌─────────────────┐ \n");
    for (int y = 0; y < BOARD_SIZE; y++) {
        printf("%d│ ", 8 - y);
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (x == selX && y == selY) {
                printf("[%c]", board[y][x]);
            }
            else {
                if (board[y][x] == '.') {
                    printf(". ");
                }
                else {
                    printf("%c ", board[y][x]);
                }
            }
        }
        printf("│%d\n", 8 - y);
    }
    printf(" └─────────────────┘ \n");
    printf(" a b c d e f g h \n\n");
}

SDL_Texture* get_piece_texture(char piece,
    SDL_Texture* w_p, SDL_Texture* w_r, SDL_Texture* w_n, SDL_Texture* w_b, SDL_Texture* w_q, SDL_Texture* w_k,
    SDL_Texture* b_p, SDL_Texture* b_r, SDL_Texture* b_n, SDL_Texture* b_b, SDL_Texture* b_q, SDL_Texture* b_k) {
    switch (piece) {
    case 'P': return w_p;
    case 'R': return w_r;
    case 'N': return w_n;
    case 'B': return w_b;
    case 'Q': return w_q;
    case 'K': return w_k;
    case 'p': return b_p;
    case 'r': return b_r;
    case 'n': return b_n;
    case 'b': return b_b;
    case 'q': return b_q;
    case 'k': return b_k;
    default: return NULL;
    }
}

SDL_Texture* load_transparent_bmp(SDL_Renderer* renderer, const char* filename) {

    SDL_Surface* temp_surface = SDL_LoadBMP(filename);

    if (!temp_surface) {
        SDL_Log("실패: %s (%s)", filename, SDL_GetError());
        return NULL;
    }

    // 알파 채널이 있는 RGBA 형식으로 변환
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(
        temp_surface, SDL_PIXELFORMAT_RGBA32, 0
    );

    SDL_FreeSurface(temp_surface);

    if (!surface) {
        return NULL;
    }

    int width = surface->w;
    int height = surface->h;

    // 방문할 픽셀을 저장할 배열
    int* queue = malloc(sizeof(int) * width * height);

    if (!queue) {
        SDL_FreeSurface(surface);
        return NULL;
    }

    SDL_LockSurface(surface);

    Uint32* pixels = (Uint32*)surface->pixels;

    int front = 0;
    int rear = 0;

    // 가장자리의 밝은 픽셀을 탐색 시작점으로 등록
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            if (x != 0 && y != 0 &&
                x != width - 1 && y != height - 1) {
                continue;
            }

            Uint8 r, g, b, a;

            Uint32* pixel = (Uint32*)((Uint8*)surface->pixels
                + y * surface->pitch) + x;

            SDL_GetRGBA(*pixel, surface->format, &r, &g, &b, &a);

            if (r >= 225 && g >= 225 && b >= 225) {
                *pixel = SDL_MapRGBA(surface->format, r, g, b, 0);
                queue[rear++] = y * width + x;
            }
        }
    }

    // 가장자리와 연결된 밝은 픽셀 제거
    while (front < rear) {

        int pos = queue[front++];

        int x = pos % width;
        int y = pos / width;

        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {

                int nx = x + dx;
                int ny = y + dy;

                if (nx < 0 || nx >= width ||
                    ny < 0 || ny >= height) {
                    continue;
                }

                Uint32* pixel = (Uint32*)((Uint8*)surface->pixels
                    + ny * surface->pitch) + nx;

                Uint8 r, g, b, a;

                SDL_GetRGBA(*pixel, surface->format, &r, &g, &b, &a);

                if (a == 0) {
                    continue;
                }

                if (r >= 225 && g >= 225 && b >= 225) {

                    *pixel = SDL_MapRGBA(
                        surface->format, r, g, b, 0
                    );

                    queue[rear++] = ny * width + nx;
                }
            }
        }
    }

    SDL_UnlockSurface(surface);

    free(queue);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
        renderer, surface
    );

    SDL_FreeSurface(surface);

    if (texture) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    }

    return texture;
}

int main(int argc, char* argv[]) {
    init_console();
    system("cls");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL 초기화 실패: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image 초기화 실패: %s\n", IMG_GetError());
    }

    SDL_Window* window = SDL_CreateWindow(

        "Chess Game with UI Sidebar",
        0, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    if (!window) {
        printf("SDL 윈도우 생성 실패: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //여기부터는 따로 폰 승격을 위한 UI 코드입니다 
    SDL_Window* promotionWindow = SDL_CreateWindow(
        "Pawn promotion",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        480, 180,
        SDL_WINDOW_HIDDEN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS
    );

    SDL_Renderer* promotionRederer = SDL_CreateRenderer(promotionWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* promotionTextures[8] = { NULL };
    const char* promotionFiles[8] = {
    "w_q.bmp", "w_r.bmp", "w_b.bmp", "w_n.bmp",
    "b_q.bmp", "b_r.bmp", "b_b.bmp", "b_n.bmp"
    };

    for (int i = 0; i < 8; i++) {
            promotionTextures[i] =
                load_transparent_bmp(promotionRederer, promotionFiles[i]);
    }

    SDL_Texture* w_p = load_transparent_bmp(renderer, "w_p.bmp");
    SDL_Texture* w_r = load_transparent_bmp(renderer, "w_r.bmp");
    SDL_Texture* w_n = load_transparent_bmp(renderer, "w_n.bmp");
    SDL_Texture* w_b = load_transparent_bmp(renderer, "w_b.bmp");
    SDL_Texture* w_q = load_transparent_bmp(renderer, "w_q.bmp");
    SDL_Texture* w_k = load_transparent_bmp(renderer, "w_k.bmp");

    SDL_Texture* b_p = load_transparent_bmp(renderer, "b_p.bmp");
    SDL_Texture* b_r = load_transparent_bmp(renderer, "b_r.bmp");
    SDL_Texture* b_n = load_transparent_bmp(renderer, "b_n.bmp");
    SDL_Texture* b_b = load_transparent_bmp(renderer, "b_b.bmp");
    SDL_Texture* b_q = load_transparent_bmp(renderer, "b_q.bmp");
    SDL_Texture* b_k = load_transparent_bmp(renderer, "b_k.bmp");

    int selX = -1, selY = -1;
    bool running = true;
    bool game_over = false;
    bool promotion_window_shown = false;
    SDL_Event e;

    Uint32 game_start_tick = SDL_GetTicks();
    int total_time_limit = 2 * 60;


    while (running) {
        Uint32 current_tick = SDL_GetTicks();
        int elapsed_seconds = (current_tick - game_start_tick) / 1000;
        int remaining_seconds = total_time_limit - elapsed_seconds;

        if (remaining_seconds <= 0) {
            remaining_seconds = 0;
            game_over = true;
        }

        int mins = remaining_seconds / 60;
        int secs = remaining_seconds % 60;

        draw_board(selX, selY, mins, secs, game_over);

        if (game_over) {
            printf("시간 초과로 기물을 움직일 수 없습니다! \n");
        }
        else if (selX != -1 && selY != -1) {
            printf("선택된 기물: %c (%c%d) - 이동할 위치를 클릭하세요. \n",
                board[selY][selX], 'a' + selX, 8 - selY);
        }
        else {
            printf("움직일 기물을 SDL 창에서 마우스로 클릭하세요. \n");
        }

        
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
      
                    running = false;
                }
            }
            
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                //따로 승급 조건이 만족 될 때만 승급 창이 뜰 수 있게 추가 조건 부여
                if (e.button.windowID == SDL_GetWindowID(promotionWindow)
                    && e.button.button == SDL_BUTTON_LEFT) {
                    int selectedOption = -1;
                    //좌표를 기준으로 뭘 눌렀는지 판단 할 것 이기에, 들어가는 변수
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    if (mouseX >= 0 && mouseX < 480 &&
                        mouseY >= 0 && mouseY < 145) {
                        selectedOption = mouseX / 120;
                    }
                    if (selectedOption >= 0 && selectedOption <= 3) {
                        char selectedPiece;

                        if (isupper(board[promotionRow][promotionCol])) {
                            selectedPiece = "QRBN"[selectedOption];
                        }
                        else {
                            selectedPiece = "qrbn"[selectedOption];
                        }

                        board[promotionRow][promotionCol] = selectedPiece;
                        promotion_mode = 0;
                        SDL_HideWindow(promotionWindow);
                    }

                }
                else if (e.button.windowID == SDL_GetWindowID(window)
                    && !game_over 
                    && promotion_mode == 0
                    && e.button.button == SDL_BUTTON_LEFT) {

                    int mouseX = (int)e.button.x;
                    int mouseY = (int)e.button.y;
                     

                    if (mouseX < BOARD_PIXEL_SIZE) {
                        int bx = mouseX / TILE_SIZE;
                        int by = mouseY / TILE_SIZE;

                        if (bx >= 0 && bx < BOARD_SIZE && by >= 0 && by < BOARD_SIZE) {
                            if (selX == -1 && selY == -1) {
                                if (board[by][bx] != '.') {
                                    selX = bx;
                                    selY = by;
                                }
                            }
                            else {
                                if (selX == bx && selY == by) {
                                    selX = -1;
                                    selY = -1;
                                }
                                else {
                                    char target_piece = board[by][bx];

                                    if (move_piece(selY, selX, by, bx)) {
                                        if (target_piece != '.') {
                                            if (target_piece >= 'A' && target_piece <= 'Z') {
                                                if (white_captured_count < 32) {
                                                    captured_white_pieces[white_captured_count++] = target_piece;
                                            }
                                        }
                                        else {
                                            if (black_captured_count < 32) {
                                                captured_black_pieces[black_captured_count++] = target_piece;
                                            }
                                        }
                                    }
                                 }
                                    selX = -1;
                                    selY = -1;
                                }
                            }
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                SDL_Rect tile = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                if ((x + y) % 2 == 1) {
                    SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
                if (x == selX && y == selY) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 150);
                    SDL_RenderFillRect(renderer, &tile);
                }
                SDL_Texture* pieceTex = get_piece_texture(board[y][x], w_p, w_r, w_n, w_b, w_q, w_k, b_p, b_r, b_n, b_b, b_q, b_k);
                if (pieceTex != NULL) {
                    SDL_RenderCopy(renderer, pieceTex, NULL, &tile);
                }
            }
        }

        SDL_Rect sidebar = { BOARD_PIXEL_SIZE, 0, SIDEBAR_WIDTH, WINDOW_HEIGHT };
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &sidebar);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(renderer, BOARD_PIXEL_SIZE, 0, BOARD_PIXEL_SIZE, WINDOW_HEIGHT);

        int timer_width = 400; // 게이지 바 너비를 더 시원하게 넓힘
        int timer_x = BOARD_PIXEL_SIZE + (SIDEBAR_WIDTH - timer_width) / 2;
        int timer_y = 80; // 상단 여백 확보

        SDL_Rect timer_bg = { timer_x, timer_y, timer_width, 35 };
        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        SDL_RenderFillRect(renderer, &timer_bg);

        float time_ratio = (float)remaining_seconds / total_time_limit;
        SDL_Rect timer_fill = { timer_x, timer_y, (int)(timer_width * time_ratio), 35 };

        if (game_over) {
            SDL_SetRenderDrawColor(renderer, 231, 76, 60, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255);
        }
        SDL_RenderFillRect(renderer, &timer_fill);
        //두 번째 타이머 (하단, 예: 잡힌 기물들 위쪽이나 아래쪽 빈 공간)
        int timer_y2 = 950;

        SDL_Rect timer_bg2 = { timer_x, timer_y2, timer_width, 35 };
        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        SDL_RenderFillRect(renderer, &timer_bg2);

        SDL_Rect timer_fill2 = { timer_x, timer_y2, (int)(timer_width * time_ratio), 35 };

        if (game_over) {
            SDL_SetRenderDrawColor(renderer, 231, 76, 60, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // 두 번째는 파란색 계열로 구분
        }
        SDL_RenderFillRect(renderer, &timer_fill2);

        int icon_size = 70; // 기물 아이콘
        int cols = 10; // 한 줄에 10개씩 배치
        int spacing = 10; // 간격

        // 렌더러 색상을 선명한 빨간색으로 설정
        SDL_SetRenderDrawColor(renderer, 231, 76, 60, 255);
        int padding = 10;

        
        int block_width = (cols * icon_size) + ((cols - 1) * spacing);
        int start_x = BOARD_PIXEL_SIZE + (SIDEBAR_WIDTH - block_width) / 2;
        int start_y = 220;

        for (int i = 0; i < white_captured_count; i++) {
            int cx = start_x + (i % cols) * (icon_size + spacing);
            int cy = start_y + (i / cols) * (icon_size + spacing);
            SDL_Rect dest = { cx, cy, icon_size, icon_size };
            SDL_Texture* pTex = get_piece_texture(captured_white_pieces[i], w_p, w_r, w_n, w_b, w_q, w_k, b_p, b_r, b_n, b_b, b_q, b_k);
            if (pTex) {
                SDL_RenderCopy(renderer, pTex, NULL, &dest);
            }

            //선명한 빨간색으로 설정
            SDL_SetRenderDrawColor(renderer, 231, 76, 60, 255);

            //선을 여러 번 겹쳐서 두껍게 만들기 (여기서는 약 3픽셀 두께)
            for (int w = 0; w < 3; w++) {
                // 대각선 1 (오른쪽이나 아래로 1픽셀씩 이동시키며 겹침)
                SDL_RenderDrawLine(renderer, cx + padding + w, cy + padding, cx + icon_size - padding + w, cy + icon_size - padding);
                SDL_RenderDrawLine(renderer, cx + padding, cy + padding + w, cx + icon_size - padding, cy + icon_size - padding + w);

                // 대각선 2
                SDL_RenderDrawLine(renderer, cx + icon_size - padding + w, cy + padding, cx + padding + w, cy + icon_size - padding);
                SDL_RenderDrawLine(renderer, cx + icon_size - padding, cy + padding + w, cx + padding, cy + icon_size - padding + w);
            }
        }

        int start_y_black = 770;
        for (int i = 0; i < black_captured_count; i++) {
            int cx = start_x + (i % cols) * (icon_size + spacing);
            int cy = start_y_black + (i / cols) * (icon_size + spacing);
            SDL_Rect dest = { cx, cy, icon_size, icon_size };
            SDL_Texture* pTex = get_piece_texture(captured_black_pieces[i], w_p, w_r, w_n, w_b, w_q, w_k, b_p, b_r, b_n, b_b, b_q, b_k);
            if (pTex) {
                SDL_RenderCopy(renderer, pTex, NULL, &dest);
            }

            //빨간색 X 겹쳐서 두껍게 그리기
            SDL_SetRenderDrawColor(renderer, 231, 76, 60, 255);
            for (int w = 0; w < 3; w++) {
                SDL_RenderDrawLine(renderer, cx + padding + w, cy + padding, cx + icon_size - padding + w, cy + icon_size - padding);
                SDL_RenderDrawLine(renderer, cx + padding, cy + padding + w, cx + icon_size - padding, cy + icon_size - padding + w);

                SDL_RenderDrawLine(renderer, cx + icon_size - padding + w, cy + padding, cx + padding + w, cy + icon_size - padding);
                SDL_RenderDrawLine(renderer, cx + icon_size - padding, cy + padding + w, cx + padding, cy + icon_size - padding + w);
            }
        }

        SDL_RenderPresent(renderer);

        if (promotion_mode == 1) {
            if (promotion_window_shown == false) {
                SDL_ShowWindow(promotionWindow);
                SDL_RaiseWindow(promotionWindow);
                promotion_window_shown = true;
            }
           

            SDL_SetRenderDrawColor(promotionRederer, 240, 240, 240, 255);
            SDL_RenderClear(promotionRederer);

            SDL_Rect card;
            card.x = 15;
            card.y = 55;
            card.w = 100;
            card.h = 110;

            SDL_RenderFillRect(promotionRederer, &card);
            SDL_RenderDrawRect(promotionRederer, &card);

            SDL_Rect pieceRect[4];
            int startIndex = isupper(board[promotionRow][promotionCol]) ? 0 : 4;

            for (int i = 0; i < 4; i++) {
                pieceRect[i].x = i * 120;
                pieceRect[i].y = 35;
                pieceRect[i].w = 120;
                pieceRect[i].h = 110;
            }
            for (int i = 0; i < 4; i++) {
                SDL_RenderCopy(
                    promotionRederer,
                    promotionTextures[startIndex + i],
                    NULL,
                    &pieceRect[i]
                );
            }

            SDL_RenderPresent(promotionRederer);
        }
        else {
            if (promotion_window_shown == true) {
                SDL_HideWindow(promotionWindow);
                promotion_window_shown = false;
            }
            
        }

        SDL_Delay(10);
    }

    if (w_p) SDL_DestroyTexture(w_p);
    if (w_r) SDL_DestroyTexture(w_r);
    if (w_n) SDL_DestroyTexture(w_n);
    if (w_b) SDL_DestroyTexture(w_b);
    if (w_q) SDL_DestroyTexture(w_q);
    if (w_k) SDL_DestroyTexture(w_k);
    if (b_p) SDL_DestroyTexture(b_p);
    if (b_r) SDL_DestroyTexture(b_r);
    if (b_n) SDL_DestroyTexture(b_n);
    if (b_b) SDL_DestroyTexture(b_b);
    if (b_q) SDL_DestroyTexture(b_q);
    if (b_k) SDL_DestroyTexture(b_k);

    for (int i = 0; i < 8; i++) {
        if (promotionTextures[i] != NULL) {
            SDL_DestroyTexture(promotionTextures[i]);
        }
    }

    SDL_DestroyRenderer(promotionRederer);
    SDL_DestroyWindow(promotionWindow);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}