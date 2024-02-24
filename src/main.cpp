#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <ctime>
#include <fstream>
#include <queue>

const int last = 120;
int color_top2[] = { 110,231,221,255 };
int color_bot2[] = { 230,249,247,255 };
int color_top[] = { 106,45,112,255 };
int color_bot[] = { 23,11,26,255 };
long double sin_start, sin_start2;
std::deque<int> qu;
int value[1000];
int checkLost[1000];
int valuendgame = 0;
int ggt = 0;
int oldsocre = 0;
int postionLast;
int deltahowtoplay = 0;
const int costintro = 20;
const int SinMax = 109;
const int SinMin = 90;
//---------------
Rectangle sourceRec;
Rectangle destRec;
Vector2 origin;
float rotation = 0;
float Width, Height, frameWidth, frameHeight;
//--------------
int col = 5, row = 5;
int val = 0, oldval, win = 0, highscore = 0;
int howtoplay = 0;
const char msg[50] = "MINESWEEPER";
const char msg2[50] = "PLAY";
const int screenWidth = 600;
const int screenHeight = 900;
Texture2D texture, texture2, texture3, texture4, texture5 , texture6 , texture7, texture8,texture9;
Font font2, font;
int numBomb, endgame = 0, lasttime = 0, score = 0, checked = 0;
int deltaship = 0;
std::vector <int> adj[1000];
std::string st;
Texture2D scarfy;
//save
int save;
// time 
int oldtime = 0, begintime = 0;

struct rRec {
    int x1, y1, x2, y2, value, bomb, count, flag;
    rRec() {}
    rRec(int x1, int x2, int y1, int y2, int value, int  bomb, int count, int flag) : x1(x1), x2(x2), y1(y1), y2(y2), value(value), bomb(bomb), count(count), flag(flag) {}
};
rRec REC[1000];

long double get_sin(long double x) {
    return 106.0 + (10.0 * sin(((x) / (55.0))));
}


long double get_sin2(long double x) {

    return 103.0 + (13.0 * sin(((x) / (45.0))));
}

void load_image() {
    scarfy = LoadTexture("./assets/test2.png");
    Image image = LoadImage("./assets/bomb5.png");  // Load image data into CPU memory (RAM)
    texture = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);
    image = LoadImage("./assets/flag.png");  // Load image data into CPU memory (RAM)
    texture2 = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);
    image = LoadImage("./assets/bom6real.png");  // Load image data into CPU memory (RAM)
    texture3 = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);
    image = LoadImage("./assets/clockreal.png");  // Load image data into CPU memory (RAM)
    texture4 = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);
    image = LoadImage("./assets/scorereal.png");
    texture5 = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);
    image = LoadImage("./assets/MINESWEEPER.png");
    texture6 = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);
    image = LoadImage("./assets/bannerwin.png");
    texture7 = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);
    image = LoadImage("./assets/bannerscore.png");
    texture8 = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);
    image = LoadImage("./assets/howtoplay.png");
    texture9 = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);
}

void load_font() {
    font = LoadFontEx("./assets/SNAP____.ttf", 96, 0, 0);
    GenTextureMipmaps(&font.texture);
    font2 = LoadFontEx("./assets/NotoSans-SemiBold.ttf", 96, 0, 0);
    GenTextureMipmaps(&font2.texture);
}

void setup() {

    while (!(qu.empty())) qu.pop_front();
    win = 0;
    Width = 600;
    Height = 120;
    deltaship = 0;
    frameWidth = scarfy.width;
    frameHeight = scarfy.height;
    sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    destRec = { Width / 2.0f, Height / 2.0f, frameWidth * 2.0f, frameHeight * 2.0f };
    origin = { (float)frameWidth, (float)frameHeight };
    oldtime = 0;
    begintime = (int)GetTime();
    endgame = 0;
    int x = std::min((590 - 10) / row, ((850 - 100) / col));
    int central = 450;
    int centralx = 300;
    int count = 0;
    for (int i = central - ((float)(col / 2.0) * x) + 50, j = 0; j < col; i += x, j++) {
        for (int j = 0; j < row; j++) {
            REC[count++] = rRec(centralx - ((float)((row / 2.0) * x)) + x * j + 3, centralx - ((float)((row / 2.0) * x)) + x * j + 3 + x - 6, i + 3, i + 3 + x - 6, 0, 0, 0, 0);
        }
    }

    numBomb = (col * row) / 7;
    //numBomb = 1;

    for (int i = 0; i < numBomb; i++) {
        while (true)
        {
            int x = rand() % (col * row);
            if (REC[x].bomb == 0) {
                REC[x].bomb = 1;
                break;
            }
        }
    }

    for (int i = 0; i < (col * row); i++) {
        adj[i].clear();
        if (i - row >= 0) adj[i].push_back(i - row);
        if (i % row > 0) {
            if (i - row >= 0) adj[i].push_back(i - row - 1);
            if (i + row - 1 < (col * row)) adj[i].push_back(i + row - 1);
            adj[i].push_back(i - 1);
        }

        if (i % row != row - 1) {
            if (i - row + 1 >= 0) adj[i].push_back(i - row + 1);
            if (i + row + 1 < (col * row)) adj[i].push_back(i + row + 1);
            adj[i].push_back(i + 1);
        }

        if (i + row < (col * row)) adj[i].push_back(i + row);
    }

}

void paint() {

    float roundness = 0.5f;
    float segments = 0.3f;
    Rectangle Rec = { (float)520 ,(float)10,(float)70,(float)70 };

    std::vector<unsigned char> g;
    for (int i = 1; i <= 130; i++) {
        g.assign(4, 0);
        for (int j = 0; j < 4; j++) g[j] = color_top2[j] - (1.0 * i * ((1.0 * color_top2[j] - color_bot2[j]) / (last * 1.0)));

        DrawRectangle(0,i, 600, 1, { g[0],g[1],g[2],g[3] });

    }


     int ok = 0;
    for (int i = 85; i <= 115; i++) {
        std::vector<unsigned char> g;
        g.assign(4, 0);
        for (int j = 0; j < 4; j++) g[j] = color_top[j] - (1.0 * i * ((1.0 * color_top[j] - color_bot[j]) / 900.0));

        int d = 0;
        for (int j = 0; j < 600; j++) {
            if (i > get_sin2(sin_start2*1.0 + j)) {
                d++;
                DrawRectangle(j, i, 1, 1, { 134,85,140,255 });
            }
        }
        if (ok == 1) {
            DrawRectangle(0, i, 600, 1, { 134,85,140,255 });
        }
        if (d == 601) ok = 1;
    }
   // for (int i = 111 ; i <= 120 ; i++ ) DrawRectangle(0, i, 600, 1, { 134,85,140,255 });

    DrawRectangleRounded(Rec, roundness, (int)segments, { 102,57,166,255 });

    for (int i = 30; i <= 35; i++) {
        g.assign(4, 0);
        for (int j = 0; j < 4; j++) g[j] = color_top2[j] - (1.0 * i * ((1.0 * color_top2[j] - color_bot2[j]) / (last * 1.0)));
        DrawRectangle(1,i, 600, 1, { g[0],g[1],g[2],g[3] });
    }


    for (int i = 55; i <= 60; i++) {
        g.assign(4, 0);
        for (int j = 0; j < 4; j++) g[j] = color_top2[j] - (1.0 * i * ((1.0 * color_top2[j] - color_bot2[j]) / (last * 1.0)));
        DrawRectangle(1,i, 600, 1, { g[0],g[1],g[2],g[3] });
    }

    rotation = -(1.0 * (get_sin(sin_start*1.0 + 255 - deltaship) - 93)) / ((112 - 93) * 1.0) * 10.0 + 5.0;
    DrawTexturePro(scarfy, sourceRec, destRec, origin, (float)rotation, WHITE);
    ok = 0;
     
    for (int i = 90; i <= 115; i++) {
        std::vector<unsigned char> g;
        g.assign(4, 0);
        for (int j = 0; j < 4; j++) g[j] = color_top[j] - (1.0 * i * ((1.0 * color_top[j] - color_bot[j]) / 900.0));

        int d = 0;
        for (int j = 0; j < 600; j++) {
            if (i > get_sin(sin_start*1.0 + j)) {
                d++;
                DrawRectangle(j, i, 1, 1, { g[0],g[1],g[2],g[3]});
            }
        }
        if (ok == 1) {
            DrawRectangle(0, i, 600, 1, { g[0],g[1],g[2],g[3] });
        }
        if (d == 601) ok = 1;
    }
    
    for (int i = 116; i <= 116; i++) {
        std::vector<unsigned char> g;
        g.assign(4, 0);
        for (int j = 0; j < 4; j++) g[j] = color_top[j] - (1.0 * i * ((1.0 * color_top[j] - color_bot[j]) / 900.0));
        
        DrawRectangle(0, i, 600, 900 - 112, { g[0],g[1],g[2],g[3] });

    }

    

}

void intro() {

    endgame = 0;
    oldsocre = 0;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int A = GetMouseX();
        int B = GetMouseY();

        if (A >= 100 && A <= 500 && B >= 650 + costintro && B <= 750 + costintro) {
            val = 1;
            std::ofstream file;
            file.open("./src/savegame.txt");
            file.clear();
            file << 0 << '\n';
            file.close();
            save = 0;
            setup();
            return;
        }

        B += 50;
        A /= 50; B /= 100;
        if (B == 4 && A == 1) row--;
        if (B == 4 && A == 10) row++;
        row = std::max(row, (int)5);
        row = std::min(row, (int)16);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        int A = GetMouseX();
        int B = GetMouseY();
        B += 50;
        A /= 50; B /= 100;
        if (B == 4 && A == 1) col--;
        if (B == 4 && A == 10) col++;
        col = std::max(col, (int)5);
        col = std::min(col, (int)16);
    }

    if (row == 5) col = std::min(col, 10);
    if (row == 6) col = std::min(col, 12);
    if (row == 7) col = std::min(col, 14);
}

void paint_intro() {

    Vector2 mousePoint = GetMousePosition();
    int x = std::min((450 - 150) / row, ((600 - 200) / col));
    int central = 400 + costintro;
    int centralx = 300;

    if (CheckCollisionPointTriangle(mousePoint,{50 + 450,450 + costintro }, { 100 + 450,400 + costintro}, { 50 + 450,350  + costintro}))
    DrawTriangle({  50 + 450,(float)450 + costintro}, { 100 + 450,(float)400+costintro}, { 50 + 450,(float)350 +costintro }, { 255, 255, 255, 100 });
    else  DrawTriangle({  50 + 450,(float)450  + costintro}, { 100 + 450,(float)400 + costintro }, { 50 + 450,(float)350 + costintro }, WHITE);
    if (CheckCollisionPointTriangle(mousePoint,{ 50,(float)400 + costintro}, { 100,(float)450 +costintro }, { 100,(float)350 +costintro }))
    DrawTriangle({ 50,(float)400 + costintro}, { 100,(float)450 + costintro }, { 100,(float)350  + costintro},{ 255, 255, 255, 100 });
     else DrawTriangle({ 50,(float)400 + costintro }, { 100,(float)450  + costintro}, { 100,(float)350 + costintro }, WHITE);

    int ggggt = 0;
    if (1) ggggt = x/2;
    for (int i = central - ((float)(col / 2.0) * x) + x - ggggt, j = 0; j < col; i += x, j++) {
        for (int j = 0; j < row; j++)
            DrawRectangle(centralx - ((float)((row / 2.0) * x)) + x * j + 3, i + 3, x - 6, x - 6, WHITE);
    }

    int frameWidth = texture6.width;
    int frameHeight = texture6.height;
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    Rectangle destRec = { (float)286,(float)190, (float)frameWidth * 2.0f,(float)frameHeight * 2.0f };
    Vector2 origin = { (float)frameWidth, (float)frameHeight };
    DrawTexturePro(texture6, sourceRec, destRec, origin, (float)0, WHITE);

    //DrawTextEx(font, msg, { screenWidth / 2 - MeasureTextEx(font,msg,60,0).x / 2,152 }, 60, 0, WHITE);
    if (CheckCollisionPointRec(mousePoint, { 100,650 + costintro,400,100 })) {
        DrawRectangleRounded({ (float)100 ,(float)650 + costintro,(float)400,(float)100 }, 0.2f, (int)0.25f, WHITE);
        DrawTextEx(font2, msg2, { screenWidth / 2 - MeasureTextEx(font2,msg2,90,0).x / 2,655 + costintro }, 90, 0, { 102,57,166,255 });
    }
    else {
        DrawRectangleRounded({ (float)100 ,(float)650 + costintro,(float)400,(float)100 }, 0.3f, (int)0.35f, { 102,57,166,255 });
        DrawTextEx(font2, msg2, { screenWidth / 2 - MeasureTextEx(font2,msg2,90,0).x / 2,655 + costintro}, 90, 0, WHITE);
    }
}

void Endgame() {

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int A = GetMouseX();
        int B = GetMouseY();
        if (A >= 300 && A <= 580 && B >= 825 && B <= 885) {

            if (win == 0) oldsocre = 0;
            if (win == 1){
                int delta = oldtime + lasttime - begintime;
                oldsocre = row * col * (numBomb/3 + 1) - delta / 10 + oldsocre;
            }
            if (std::min(row,col) == 16 || win == 0); else{
                if (row < col) row++;
                else col++;
            }
            setup();
            val = 1;
            endgame = 0;
            win = 0;
            return;
        }
    }


    ggt++;
    if (ggt == 10 - (row*col)/50){
        valuendgame++;
        ggt = 0;
    }

   // std::cout << valuendgame << "\n";

    while (!qu.empty() && checkLost[qu.front()] == valuendgame){
        int u = qu.front();
        qu.pop_front();

        if (checkLost[u] != valuendgame) return;

        for (int v : adj[u]) if (checkLost[v] == -1){
            checkLost[v] = checkLost[u] + 1;
      //      std::cout << u << " " << v << " " << checkLost[v] << " " << checkLost[u] << " " << valuendgame << "\n";
            qu.push_back(v);
        }
    }
}

void paint_Endgame() {
    DrawRectangleRounded({ (float)10 ,(float)820,(float)580,(float)70 }, 0.4f, (int)0.25f, WHITE);
    Vector2 getmouse = GetMousePosition();
    if (!CheckCollisionPointRec(getmouse,{(float)300 ,(float)825,(float)280,(float)60})){
        DrawRectangleRounded({ (float)300 ,(float)825,(float)280,(float)60 }, 0.2f, (int)0.3f, { 102,57,166,255 });

        if (win)
        DrawTextEx(font2, "NEXT LEVEL", { 300 + 280 / 2 - MeasureTextEx(font2,"NEXT LEVEL",45,0).x / 2,823 + 8 }, 45, 0, WHITE);
        else DrawTextEx(font2, "REPLAY", { 300 + 280 / 2 - MeasureTextEx(font2,"REPLAY",45,0).x / 2,823 + 8 }, 45, 0, WHITE);
    }
    else{
        DrawRectangleRounded({ (float)300 ,(float)825,(float)280,(float)60 }, 0.2f, (int)0.3f, { 102,57,166,135 });
        if (win)
        DrawTextEx(font2, "NEXT LEVEL", { 300 + 280 / 2 - MeasureTextEx(font2,"NEXT LEVEL",45,0).x / 2,823 + 8 }, 45, 0, WHITE);  
        else DrawTextEx(font2, "REPLAY", { 300 + 280 / 2 - MeasureTextEx(font2,"REPLAY",45,0).x / 2,823 + 8 }, 45, 0, WHITE);
    }

    if (win) {
        DrawTextEx(font2, "YOU WIN", { 300 / 2 - MeasureTextEx(font2,"YOU WIN",45,0).x / 2,823 + 8 }, 45, 0, { 82,18,97,255 });
        
        if (valuendgame - std::max(row,col) <= 5) return ;

        int frameWidth = texture7.width;
        int frameHeight = texture7.height;
        Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
        Rectangle destRec = { (float)300,(float)470, (float)frameWidth * 2.0f,(float)frameHeight * 2.0f };
        Vector2 origin = { (float)frameWidth, (float)frameHeight };
        DrawTexturePro(texture7, sourceRec, destRec, origin, (float)0, WHITE);
        int delta = oldtime + lasttime - begintime;
        DrawTextEx(font2, TextFormat("%d", row * col * (numBomb/3 + 1) - delta / 10 + oldsocre),{325 - MeasureTextEx(font2,TextFormat("%d", row * col * (numBomb/3 + 1) - delta / 10 + oldsocre),40,0).x / 2,358 - 10},40,0.2f,{ 82,18,97,255 });
        std::vector <std::pair<int, std::string> > p;
        std::ifstream fin;
        fin.open("./src/highscore.txt");
        p.resize(15);
        for (std::pair<int, std::string>& v : p) {
            fin >> v.first;
            fin.ignore();
            getline(fin, v.second);
        }
        fin.close();

        if (!checked) {
            int t = 'a' - 'A';
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))  t = 0;
            for (int i = 'A'; i <= 'Z'; i++) if (IsKeyPressed(i)) st.push_back(i + t);
            for (int i = '0'; i <= '9'; i++) if (IsKeyPressed(i)) st.push_back(i);
            if (IsKeyPressed(' ')) st.push_back(' ');
            if (IsKeyPressed(KEY_BACKSPACE)) if (!st.empty()) st.pop_back();
            if (st.size() > 15) st.pop_back();
            if (IsKeyPressed(KEY_ENTER) && !st.empty()) checked = 1;

        }

        if (checked == 1) {
            p.push_back(std::pair<int, std::string>((row * col * (numBomb/3 + 1) - delta / 10 + oldsocre), st));
            std::sort(p.begin(), p.end(), std::greater<std::pair<int, std::string> >());
            p.pop_back();
            checked = 2;
        }

        {
            char g[100];
            int d = 0;
            for (char gg : st) g[d++] = gg;
            g[d] = '\0';
            if (d == 0){
                DrawTextEx(font2, TextFormat("ENTER YOUR NAME"), { 300 - MeasureTextEx(font2,TextFormat("ENTER YOUR NAME"),40,0).x / 2,410}, 40, 0, { 82,18,97,150 });
            }
            else DrawTextEx(font2, TextFormat("%s", g), { 300 - MeasureTextEx(font2,TextFormat("%s", g),40,0).x / 2, 410 }, 40, 0, { 82,18,97,150 });             
        }

        std::ofstream fout;
        fout.open("./src/highscore.txt");
        for (std::pair<int, std::string>& v : p) fout << v.first << '\n' << v.second << '\n';
        fout.close();

        int tt = 0;
        for (std::pair<int, std::string> v : p) {
            char g[100];
            int d = 0;
            for (char gg : v.second) g[d++] = gg;
            g[d] = '\0';
            tt++;
            float eed = 545 + 12 + (tt  - 1) * 51;
            DrawTextEx(font2, TextFormat("%s", g), { 330 - MeasureTextEx(font2,TextFormat("%s", g),25,0).x / 2,eed }, 25, 0, WHITE);
            DrawTextEx(font2, TextFormat("%d", v.first),{500 - MeasureTextEx(font2,TextFormat("%d", v.first),25,0).x / 2,eed}, 25, 0, WHITE);
            if (tt == 3) break;
        }

    }
    else
        DrawTextEx(font2, "GAME OVER", { 300 / 2 - MeasureTextEx(font2,"GAME OVER",45,0).x / 2,823 + 8 }, 45, 0, { 82,18,97,255 });
}

void dfs(int x) {
    
    if (REC[x].value) return;
    qu.push_back(x);
    // if (REC[x].bomb) return;

    // REC[x].value = 1;
    // for (int v : adj[x])
    //     REC[x].count += REC[v].bomb;

    // if (!REC[x].count) {
    //     for (int v : adj[x])
    //         if (!REC[v].value) dfs(v);
    // }
}

void start_game() {

    if (howtoplay || highscore) return;
    int EED = std::max((int)((((float)(row*col)/(16*16.0))*5.0)),1);
   // std::cout << EED << "\n";
    while (!qu.empty() && EED-- > 0){
      //  std::cout << EED << "\n";
        int u = qu.front();
        if (REC[u].value){
            qu.pop_front();
            continue;
        }
        value[u]++;
        if (value[u] >= 0){
            value[u] = 0;
            REC[u].value = 1;
            for (int v : adj[u]) 
                REC[u].count += REC[v].bomb;

            for (int v : adj[u])
            if (!REC[v].value && !REC[v].bomb && !REC[u].count) qu.push_back(v);
            qu.pop_front();
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int A = GetMouseX();
        int B = GetMouseY();
        for (int i = 0; i < col * row; i++)
            if (A >= REC[i].x1 && A <= REC[i].x2 && B >= REC[i].y1 && B <= REC[i].y2 && !REC[i].flag) {
                if (REC[i].value) return;
                dfs(i);
                if (REC[i].bomb && !REC[i].flag) {
                    endgame = 1;
                    valuendgame = 0;
                    ggt = 0;
                    win = 0;
                    memset(checkLost,-1,sizeof(checkLost));
                    memset(value,0,sizeof(value));
                    checkLost[i] = 0;
                    while (!qu.empty()) qu.pop_back();
                    qu.push_back(i);
                }
                postionLast = i;
                return;
            } 
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        int A = GetMouseX();
        int B = GetMouseY();
        for (int i = 0; i < col * row; i++)
            if (A >= REC[i].x1 && A <= REC[i].x2 && B >= REC[i].y1 && B <= REC[i].y2) {
                if (REC[i].value) {
                    int d = 0;
                    for (int v : adj[i]) d += REC[v].flag;
                    if (d == REC[i].count) {
                        postionLast = i;
                        for (int v : adj[i])
                            if (!REC[v].value && !REC[v].flag) {
                                dfs(v);
                                if (REC[v].bomb && !REC[v].flag) {
                                    endgame = 1;
                                    win = 0;
                                    ggt = 0;
                                    valuendgame = 0;
                                    memset(checkLost,-1,sizeof(checkLost));
                                    memset(value,0,sizeof(value));
                                    checkLost[i] = 0;
                                    while (!qu.empty()) qu.pop_back();
                                    qu.push_back(i);
                                }
                            }
                    }
                    return;
                }
                REC[i].flag ^= 1;
            }
    }
}

void paint_game() {

  // for (int i = 1 ; i <= 50 ; i++) if (value[i] > 0) std::cout << i << "\n"; 

    float roundness = 0.4f;
    float segments = 0.3f;
    int x = std::min((590 - 10) / row, ((850 - 100) / col));
    int central = 450;
    int centralx = 300;
    int count = 0;
    int countWin = 0;
    int countt = 0;
    int gg = 0;
    Vector2 getmouse = GetMousePosition();
    if (std::max(col, row) >= 12) gg = 1;
    for (int i = central - ((float)(col / 2.0) * x) + 50, j = 0; j < col; i += x, j++) {
        for (int j = 0; j < row; j++) {
            Rectangle rec = { (float)centralx - ((float)((row / 2.0) * x)) + x * j + 3 - gg,(float)i + 3 - gg,(float)x - 6 + 2 * gg,(float)x - 6 + 2 * gg };

            if (endgame && REC[count].bomb && checkLost[count] <= valuendgame && checkLost[count] != -1) {

                texture.width *= ((1.0 * x) / (1.0 * texture.width));
                texture.height *= ((1.0 * x) / (1.0 * texture.height));
                int frameWidth = texture.width;
                int frameHeight = texture.height;
                DrawRectangleRounded(rec, roundness, (int)segments, Fade(WHITE, 0.2f));
                Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
                Rectangle destRec = { (float)(centralx - ((float)((row / 2.0) * x)) + x * j + x / 2.0),(float)(i + x / 2.0 - 3), (float)frameWidth * 2.0f,(float)frameHeight * 2.0f };
                Vector2 origin = { (float)frameWidth, (float)frameHeight };
                DrawTexturePro(texture, sourceRec, destRec, origin, (float)0, WHITE);
                count++;
                continue;
            }

            if (REC[count].value == 0) {

                if (!CheckCollisionPointRec(getmouse,rec))
                DrawRectangleRounded(rec, roundness, (int)segments, Fade(WHITE, 0.9f));
                else DrawRectangleRounded(rec, roundness, (int)segments, Fade(WHITE, 0.75f));
                if (REC[count].flag) {
                    countt++;
                    texture2.width *= ((0.7 * x) / (1.0 * texture2.width));
                    texture2.height *= ((0.7 * x) / (1.0 * texture2.height));
                    int frameWidth = texture2.width;
                    int frameHeight = texture2.height;
                    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
                    Rectangle destRec = { (float)(centralx - ((float)((row / 2.0) * x)) + x * j + x / 2.0 + x / 20.0),(float)(i + x / 2.0 + x / 5.0), (float)frameWidth * 2.0f,(float)frameHeight * 2.0f };
                    Vector2 origin = { (float)frameWidth, (float)frameHeight };
                    DrawTexturePro(texture2, sourceRec, destRec, origin, (float)0, WHITE);
                }
            }
            else {
                DrawRectangleRounded(rec, roundness, (int)segments, Fade(WHITE, 0.2f));
                countWin++;
            }
            if (REC[count].count > 0) {
                char g[2] = { (char)(REC[count].count + '0') };
                DrawTextEx(font2, g, { (float)centralx - ((float)((row / 2.0) * x)) + x * j - gg + x / 2.0f - MeasureTextEx(font2,g,x,0).x / 2,(float)i - 2 }, x, 0, { 50,144,222,255 });
            }

            if (endgame && win && checkLost[count] != -1 && valuendgame - checkLost[count] <= 5){
                float gt = valuendgame - checkLost[count];
                gt = (gt/5.0)*1.0;
              //  std::cout << " -" << ggt << "\n";
                DrawRectangleRounded(rec, roundness, (int)segments, Fade(WHITE,(float) std::max(1.0f - gt,(float)0.0)));
                count++;
                continue;
            } else
            if (endgame && checkLost[count] != -1 && valuendgame - checkLost[count] <= 10){
                float gt = valuendgame - checkLost[count];
                gt = (gt/10.0)*4.0;
              //  std::cout << " -" << ggt << "\n";
                DrawRectangleRounded(rec, roundness, (int)segments, Fade(BLACK,(float) std::max(0.4f - gt,(float)0.0)));
                count++;
                continue;
            }
            count++;
        }
    }

    texture3.width = 65;
    texture3.height = 65;
    DrawTexture(texture3, -12, 13, WHITE);
    DrawText(TextFormat("%d", numBomb - countt), 40, 40, 20, { 82,18,97,255 });


    if (!endgame) lasttime = GetTime();
    int delta = oldtime + lasttime - begintime;

    texture4.width = 55;
    texture4.height = 55;
    DrawTexture(texture4, -7, -9, WHITE);
    DrawText(TextFormat("%d : %d ", delta / 60, delta % 60), 40, 10, 20, { 82,18,97,255 });

    texture5.width = 85;
    texture5.height = 85;
    DrawTexture(texture5, -22, 36, WHITE);
    DrawText(TextFormat("%d", row * col * (numBomb/3 + 1) - delta / 10 + oldsocre), 40, 70, 20, { 82,18,97,255 });
   // if (IsKeyPressed(KEY_A)) std::cout << oldsocre << "\n";
    if (countWin + numBomb == row * col) if (win == 0) {
        endgame = 1;
        win = 1;
        checked = 0;
        ggt = 0;
        valuendgame = 0;
        memset(checkLost,-1,sizeof(checkLost));
        memset(value,0,sizeof(value));
        checkLost[postionLast] = 0;
        while (!qu.empty()) qu.pop_back();
        qu.push_back(postionLast);
        st.clear();
    }

}

void Out_highscores() {

    int frameWidth = texture8.width;
    int frameHeight = texture8.height;
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    Rectangle destRec = { (float)300,(float)470, (float)frameWidth * 2.0f,(float)frameHeight * 2.0f };
    Vector2 origin = { (float)frameWidth, (float)frameHeight };
    DrawTexturePro(texture8, sourceRec, destRec, origin, (float)0, WHITE);

    std::vector <std::pair<int, std::string> > p;
    std::string st;
    st.clear();
    std::ifstream fin;
    fin.open("./src/highscore.txt");
    p.resize(15);
    for (std::pair<int, std::string>& v : p) {
        fin >> v.first;
        fin.ignore();
        getline(fin, v.second);
    }
    fin.close();

    std::ofstream fout;
    fout.open("./src/highscore.txt");
    for (std::pair<int, std::string>& v : p) fout << v.first << '\n' << v.second << '\n';
    fout.close();

    int tt = 0;
    for (std::pair<int, std::string> v : p) {
        char g[100];
        int d = 0;
        for (char gg : v.second) g[d++] = gg;
        g[d] = '\0';
        tt++;
        float eed = 305 + 17 + (tt  - 1) * 53;
        DrawTextEx(font2, TextFormat("%s", g), { 330 - MeasureTextEx(font2,TextFormat("%s", g),25,0).x / 2,eed }, 25, 0, WHITE);
        DrawTextEx(font2, TextFormat("%d", v.first),{500 - MeasureTextEx(font2,TextFormat("%d", v.first),25,0).x / 2,eed}, 25, 0, WHITE);
        if (tt == 7) break;
    }
}

void menu() {

    if (howtoplay || highscore ) return ;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int A = GetMouseX();
        int B = GetMouseY();

        if (A >= 150 && A <= 450 && B >= 250 && B <= 250 + 70) {
            if (oldval == 0) return;
            std::ofstream file;
            file.open("./src/savegame.txt");
            file.clear();
            file << 0 << '\n';
            file.close();
            save = 0;
            setup();
            val = 1;
            oldsocre = 0;
        }

        if (A >= 150 && A <= 450 && B >= 350 && B <= 350 + 70) {
            val = 0;
            setup();
            return;
        }

        if (A >= 150 && A <= 450 && B >= 450 && B <= 450 + 70) {
            howtoplay = 1;
            deltahowtoplay = 0;
        }

        if (A >= 150 && A <= 450 && B >= 550 && B <= 550 + 70) {
            if (oldval == 0) {
                if (save == 0) return;
                std::ifstream file;
                file.open("./src/savegame.txt");
                begintime = (int)GetTime();
                win = 0;
                file >> save >> oldtime;
                file >> col >> row;
                file >> numBomb;
                for (int i = 0; i < col * row; i++) {
                    file >> REC[i].x1;
                    file >> REC[i].x2;
                    file >> REC[i].y1;
                    file >> REC[i].y2;
                    file >> REC[i].value;
                    file >> REC[i].bomb;
                    file >> REC[i].count;
                    file >> REC[i].flag;
                }

                for (int i = 0; i < (col * row); i++) {
                    adj[i].clear();
                    if (i - row >= 0) adj[i].push_back(i - row);
                    if (i % row > 0) {
                        if (i - row >= 0) adj[i].push_back(i - row - 1);
                        if (i + row - 1 < (col * row)) adj[i].push_back(i + row - 1);
                        adj[i].push_back(i - 1);
                    }

                    if (i % row != row - 1) {
                        if (i - row + 1 >= 0) adj[i].push_back(i - row + 1);
                        if (i + row + 1 < (col * row)) adj[i].push_back(i + row + 1);
                        adj[i].push_back(i + 1);
                    }

                    if (i + row < (col * row)) adj[i].push_back(i + row);
                }
                file.close();
                val = 1;
            }
            else {

                if (endgame == 1) return ;
                std::ofstream file;
                file.open("./src/savegame.txt");
                file.clear();
                save = 1;
                deltaship = 0;
                Width = 600;
                Height = 120;
                frameWidth = scarfy.width;
                frameHeight = scarfy.height;
                sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
                destRec = { Width / 2.0f, Height / 2.0f, frameWidth * 2.0f, frameHeight * 2.0f };
                origin = { (float)frameWidth, (float)frameHeight };
                file << 1 << " " << (int)(oldtime + GetTime() - begintime) << '\n';
                file << col << " " << row << '\n';
                file << numBomb << '\n';
                for (int i = 0; i < col * row; i++)
                    file << REC[i].x1 << " " << REC[i].x2 << " " << REC[i].y1 << " " << REC[i].y2 << " " << REC[i].value << " " << REC[i].bomb << " " << REC[i].count << " " << REC[i].flag << '\n';
                file.close();
                val = 0;
            }

            return;
        }

        if (A >= 150 && A <= 450 && B >= 650 && B <= 650 + 70) {
            highscore = 1;
        }
    }

}

void paint_menu() {

    if (highscore || howtoplay){
            Rectangle Rec = { (float)520 ,(float)10,(float)70,(float)70 };
            DrawRectangleRounded(Rec, 0.4f, (int)0.3f, { 102,57,166,255 });
            DrawTextEx(font2, "X", { 535,3 }, 80, 0, WHITE);
    }

    if (highscore) return ;
    if (howtoplay == 1) {
        // DrawRectangleRounded({ (float)40 ,(float)250,(float)520,(float)350 }, 0.1f, (int)0.25f, WHITE);
        // DrawTextEx(font2, "How to play game?", { 300 - MeasureTextEx(font2,"How to play game?",60,0).x / 2,250 + 8 }, 60, 0, { 82,18,97,255 });
        // DrawTextEx(font2, "     - To increase or decrease the number of rows, click ", { 50,330 }, 25, 0, { 102,57,166,255 });
        // DrawTextEx(font2, "the corresponding arrows with the right mouse button.", { 50,330 + 25 }, 25, 0, { 102,57,166,255 });
        // DrawTextEx(font2, "     - To increase or decrease the number of columns, click ", { 50,330 + 75 }, 25, 0, { 102,57,166,255 });
        // DrawTextEx(font2, "the corresponding arrows with the left mouse button.", { 50,330 + 100 }, 25, 0, { 102,57,166,255 });
        // DrawTextEx(font2, "     - To place a flag, right-click the corresponding cell.", { 50,330 + 150 }, 25, 0, { 102,57,166,255 });
        // DrawTextEx(font2, "     - To open a new cell, left-click the corresponding cell.", { 50,330 + 200 }, 25, 0, { 102,57,166,255 });

        int frameWidth = texture9.width;
        int frameHeight = texture9.height;
        int scrollSpeed = 4; 
        deltahowtoplay -= (GetMouseWheelMove()*scrollSpeed);
        deltahowtoplay = std::min(deltahowtoplay,1700 + 20);
        deltahowtoplay = std::max(deltahowtoplay,0);       
   //     if (IsKeyPressed(KEY_A)) std::cout << deltahowtoplay << "\n";
        Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight  };
        Rectangle destRec = { (float)300,(float)1285 + 20 - deltahowtoplay, (float)frameWidth * 2.0f,(float)frameHeight * 2.0f };
        Vector2 origin = { (float)frameWidth, (float)frameHeight };
        DrawTexturePro(texture9, sourceRec, destRec, origin, (float)0, WHITE);
        return;
    }

    Rectangle Rec = { (float)520 ,(float)10,(float)70,(float)70 };
    DrawRectangleRounded(Rec, 0.4f, (int)0.3f, { 102,57,166,255 });
    DrawTextEx(font2, "X", { 535,3 }, 80, 0, WHITE);
    float roundness = 0.05f;
    float segments = 0.0f;
    Rectangle rec = { (float)50 ,(float)120,(float)500,(float)750 };
    DrawRectangleRounded(rec, roundness, (int)segments, { 102,57,166,255 });

    DrawTextEx(font2, "MENU", { screenWidth / 2 - MeasureTextEx(font2,"MENU",65,0).x / 2,150 }, 65, 0, WHITE);
    Vector2 getmouse = GetMousePosition();

    float g = 1;
    if (oldval == 0) g = 0.3f;
    if (CheckCollisionPointRec(getmouse, { (float)300 - 150 ,(float)250,(float)300,(float)70 }) && oldval != 0) {
        DrawRectangleRounded({ (float)300 - 150 ,(float)250,(float)300,(float)70 }, 0.4f, (int)0.3f, Fade({ 82,18,97,255 }, 0.8f));
        DrawTextEx(font2, "RESTART", { screenWidth / 2 - MeasureTextEx(font2,"RESTART",40,0).x / 2,255 + 8 }, 40, 0, WHITE);
    }
    else {
        DrawRectangleRounded({ (float)300 - 150 ,(float)250,(float)300,(float)70 }, 0.4f, (int)0.3f, Fade(WHITE, g));
        DrawTextEx(font2, "RESTART", { screenWidth / 2 - MeasureTextEx(font2,"RESTART",40,0).x / 2,255 + 8 }, 40, 0, { 82,18,97,255 });
    }

    if (!CheckCollisionPointRec(getmouse, { (float)300 - 150 ,(float)350,(float)300,(float)70 })) {
        DrawRectangleRounded({ (float)300 - 150 ,(float)350,(float)300,(float)70 }, 0.4f, (int)0.3f, WHITE);
        DrawTextEx(font2, "CUSTOM SIZE", { screenWidth / 2 - MeasureTextEx(font2,"CUSTOM SIZE",40,0).x / 2,355 + 8 }, 40, 0, { 82,18,97,255 });
    }
    else {
        DrawRectangleRounded({ (float)300 - 150 ,(float)350,(float)300,(float)70 }, 0.4f, (int)0.3f, { 82,18,97,255 });
        DrawTextEx(font2, "CUSTOM SIZE", { screenWidth / 2 - MeasureTextEx(font2,"CUSTOM SIZE",40,0).x / 2,355 + 8 }, 40, 0, WHITE);
    }

    if (!CheckCollisionPointRec(getmouse, { (float)300 - 150 ,(float)450,(float)300,(float)70 })) {
        DrawRectangleRounded({ (float)300 - 150 ,(float)450,(float)300,(float)70 }, 0.4f, (int)0.3f, WHITE);
        DrawTextEx(font2, "HOW TO PLAY", { screenWidth / 2 - MeasureTextEx(font2,"HOW TO PLAY",40,0).x / 2,455 + 8 }, 40, 0, { 82,18,97,255 });
    }
    else {
        DrawRectangleRounded({ (float)300 - 150 ,(float)450,(float)300,(float)70 }, 0.4f, (int)0.3f, { 82,18,97,255 });
        DrawTextEx(font2, "HOW TO PLAY", { screenWidth / 2 - MeasureTextEx(font2,"HOW TO PLAY",40,0).x / 2,455 + 8 }, 40, 0, WHITE);
    }

    if (oldval == 1) {
        if (CheckCollisionPointRec(getmouse, { (float)300 - 150 ,(float)550,(float)300,(float)70 }) && !endgame) {
            DrawRectangleRounded({ (float)300 - 150 ,(float)550,(float)300,(float)70 }, 0.4f, (int)0.3f, { 82,18,97,255 });
            DrawTextEx(font2, "SAVE GAME ", { screenWidth / 2 - MeasureTextEx(font2,"SAVE GAME",40,0).x / 2,555 + 12 }, 40, 0, WHITE);
        }
        else {
            DrawRectangleRounded({ (float)300 - 150 ,(float)550,(float)300,(float)70 }, 0.4f, (int)0.3f, Fade(WHITE, 1));
            DrawTextEx(font2, "SAVE GAME ", { screenWidth / 2 - MeasureTextEx(font2,"SAVE GAME",40,0).x / 2,555 + 12 }, 40, 0, { 82,18,97,255 });
        }
    }
    else {
        if (save == 1) {
            if (CheckCollisionPointRec(getmouse, { (float)300 - 150 ,(float)550,(float)300,(float)70 })) {
                DrawRectangleRounded({ (float)300 - 150 ,(float)550,(float)300,(float)70 }, 0.4f, (int)0.3f, { 82,18,97,255 });
                DrawTextEx(font2, "RESUMED ", { screenWidth / 2 - MeasureTextEx(font2,"RESUMED",40,0).x / 2,555 + 12 }, 40, 0, WHITE);
            }
            else {
                DrawRectangleRounded({ (float)300 - 150 ,(float)550,(float)300,(float)70 }, 0.4f, (int)0.3f, Fade(WHITE, 1));
                DrawTextEx(font2, "RESUMED ", { screenWidth / 2 - MeasureTextEx(font2,"RESUMED",40,0).x / 2,555 + 12 }, 40, 0, { 82,18,97,255 });
            }
        }
        else {
            DrawRectangleRounded({ (float)300 - 150 ,(float)550,(float)300,(float)70 }, 0.4f, (int)0.3f, Fade(WHITE, 0.2f));
            DrawTextEx(font2, "RESUMED ", { screenWidth / 2 - MeasureTextEx(font2,"RESUMED",40,0).x / 2,555 + 12 }, 40, 0, { 82,18,97,255 });
        }
    }

    if (!CheckCollisionPointRec(getmouse, { (float)300 - 150 ,(float)650,(float)300,(float)70 })) {
        DrawRectangleRounded({ (float)300 - 150 ,(float)650,(float)300,(float)70 }, 0.4f, (int)0.3f, Fade(WHITE, 1));
        DrawTextEx(font2, "HIGH SCORE", { screenWidth / 2 - MeasureTextEx(font2,"HIGH SCORE",40,0).x / 2,655 + 12 }, 40, 0, { 82,18,97,255 });
    }
    else {
        DrawRectangleRounded({ (float)300 - 150 ,(float)650,(float)300,(float)70 }, 0.4f, (int)0.3f, { 82,18,97,255 });
        DrawTextEx(font2, "HIGH SCORE", { screenWidth / 2 - MeasureTextEx(font2,"HIGH SCORE",40,0).x / 2,655 + 12 }, 40, 0, WHITE);
    }

    if (oldval == 1) {
        if (!endgame) lasttime = GetTime();
        int delta = oldtime + lasttime - begintime;
    }
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Sailors cross the sea");
    SetTargetFPS(120);
    setup();
    load_image();
    load_font();

    std::ifstream file;
    file.open("./src/savegame.txt");
    file >> save;
    file.close();

    scarfy.width /= 3;
    scarfy.height /= 3;
    texture6.width /=3;
    texture6.height /= 3;
    texture7.width /= 7;
    texture7.height /= 7;
    texture8.width /= 7;
    texture8.height /= 7;
    texture9.width /= 2.8;
    texture9.height /= 2.8;

    Width = 600;
    Height = 120;
    frameWidth = scarfy.width;
    frameHeight = scarfy.height;
    sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    destRec = { Width / 2.0f, Height / 2.0f, frameWidth * 2.0f, frameHeight * 2.0f };
    origin = { (float)frameWidth, (float)frameHeight };
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_A)){
            int X = GetMouseX();
            int Y = GetMouseY();
        //    std::cout << X << " " << Y << "\n";
        }

        if (endgame && win == 0) {
            if (Height <= 250) Height += 0.4*2;
            if (Width < 1300) Width += 0.4*2;
            frameWidth = scarfy.width;
            frameHeight = scarfy.height;
            sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
            destRec = { Width / 2.0f, Height / 2.0f, frameWidth * 2.0f, frameHeight * 2.0f };
            origin = { (float)frameWidth, (float)frameHeight };
        }

        if (win == 1 && Width > -100) {
            Width -= 0.4*2;
            deltaship += 0.3*2;
            frameWidth = scarfy.width;
            frameHeight = scarfy.height;
            sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
            destRec = { Width / 2.0f, Height / 2.0f, frameWidth * 2.0f, frameHeight * 2.0f };
            origin = { (float)frameWidth, (float)frameHeight };
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int A = GetMouseX();
            int B = GetMouseY();
            if (A >= 520 && A <= 590 && B >= 20 && B <= 90) {
                if (highscore == 1) highscore = 0;
                else
                if (howtoplay == 1) {
                    howtoplay = 0;
                }
                else {
                    if (val == 2) {
                        val = oldval;
                    }
                    else {
                        oldval = val;
                        val = 2;
                    }
                }
            }
        }

        if (val == 0) intro();
        else if (val == 1) {
            if (endgame == 0) start_game();
            else{
                Endgame();
            }
        }
        else menu();
        BeginDrawing();
        ClearBackground(RAYWHITE);


        paint();

        if (val == 0)
            paint_intro();
        else if (oldval != 0 || val == 1) 
            paint_game();

        if (endgame) paint_Endgame();

        if (val == 2) paint_menu();
        if (highscore == 1) Out_highscores();

       // DrawFPS(10,850);
        EndDrawing();

       sin_start -= 1*3;
       sin_start2 -= 0.7*3;
        if (fabs(get_sin(0) - get_sin(sin_start)) <= 0.1 && fabs(get_sin(50) - get_sin(sin_start + 50)) <= 0.1){
            sin_start = 0;
         //   std::cout << "co2\n";
        }

      //  std::cout << sin_start2 << " " << fabs(get_sin2(0) - get_sin2(-1.2)) << "\n";
        if (fabs(get_sin2(0) - get_sin2(sin_start2)) < 0.1 && fabs(get_sin2(50) - get_sin2(sin_start2 + 50)) < 0.1){
            sin_start2 = 0;
            //std::cout << "co\n";
        }
    }

    UnloadTexture(scarfy);
    return 0;
}