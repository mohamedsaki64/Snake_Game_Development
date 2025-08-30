#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// Define display pins
#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Snake and apple positions
int position[100][2] = {{11, 15}, {12, 17}, {13, 19}};  
int apple_position[2];  
int score = 0;
int highscore = 0;
int speed = 300;
int element = 3;  
int richtung = 1;  
int level = 1;
int barrier_position[2];
unsigned long food_start_time = 0;  
unsigned long food_display_duration = 11000; 
bool food_visible = true;          
int Red_apple_positions[10][2];  
bool Red_apple_active[10];      
int Red_apple_count = 0;         
bool Red_apple_visible = false;  



// Function prototypes 
void Starting_screen();
void game();
void apple__snake();
void Logo();
void Text_start();
void Design();
void Apple_();
void Direction__check();
void step();
void Quadrat(int x_pos, int y_pos, int color);
void Delay_chk(int ms);
void apple_check();
void append_square();
void check_collision();
void draw_score();
void check_highscore();
void draw_level();
void barrier_placing();
void erase_barrier();
void shift_snake_to_corner();
void gameover();
void check_food_timer3();
void draw_countdown_timer();
void spawn_apl_level_for2();
void spawn_apl_level3();
void spawn_apl_level4();
void reset_game();
void spawn_apl_level5();

void setup() {
  pinMode(2, INPUT_PULLUP);  
  tft.begin();  
}

void loop() {
  Starting_screen();  
  game();         
}

// Start screen display
void Starting_screen() {
  tft.fillScreen(ILI9341_WHITE);  
  apple__snake();  
  Logo();          
  Text_start();      

  while (digitalRead(2) == HIGH) {}  
}

// Main game function
void game() {
  Design();  
  if (level < 3) {
    spawn_apl_level_for2();  
  } 
  else if (level == 3) {
    spawn_apl_level3();      
  } 
  else if( level == 4){
    spawn_apl_level4();       
  }
  else if(level >=5){
    spawn_apl_level5();
  }

  // Draw initial snake
  for (int i = 0; i < element; i++) {
    Quadrat(position[i][0], position[i][1], ILI9341_BLACK);  
  }

  Direction__check();  
  

}

// Check current direction and move the snake accordingly
void Direction__check() {
  while (true) {
    if (richtung == 1) {  
      step();
      position[0][1] -= 1;
    } else if (richtung == 2) {  
      step();
      position[0][0] += 1;
    } else if (richtung == 3) { 
      step();
      position[0][1] += 1;
    } else if (richtung == 4) {  
      step();
      position[0][0] -= 1;
    }

    // Screen wrapping logic
    if (position[0][0] > 22) position[0][0] = 1;  
    if (position[0][0] < 1) position[0][0] = 22;  
    if (position[0][1] > 30) position[0][1] = 3;  
    if (position[0][1] < 3) position[0][1] = 30;  

    Quadrat(position[0][0], position[0][1], ILI9341_BLACK);  
    Delay_chk(speed);  
    if (level >= 3) {
      check_food_timer3();
      draw_countdown_timer();  
    }
    
    
  }
}


void step() {
  int last = element - 1;
  Quadrat(position[last][0], position[last][1], ILI9341_DARKGREEN);  

  for (int i = last; i > 0; i--) {
    position[i][0] = position[i - 1][0];
    position[i][1] = position[i - 1][1];
  }

  apple_check();     
  check_collision();      
}

// Check snake eats the apple
void apple_check() {

  bool respawnNeeded = false;
  // Check if the snake's head is on the apple's position
  if (position[0][0] == apple_position[0] && position[0][1] == apple_position[1]) {
    score += 1;  
    draw_score();  
    append_square();  
    respawnNeeded = true; 

    // Level progression: Increase level every 2 points
    if (score % 2 == 0) {
      level++;  
      draw_level();  

    }
    if (level >= 2) {
          barrier_placing();  
          shift_snake_to_corner();  
        }

    if (level >= 5){
        speed = speed*0.8;
        Red_apple_count++;
      }

  }
    

  for (int i = 0; i < Red_apple_count; i++) {
        if (Red_apple_active[i] && position[0][0] == Red_apple_positions[i][0] && position[0][1] == Red_apple_positions[i][1]) {
            score -= 1;  
            draw_score();  
            Red_apple_active[i] = false;  
        }
    }
  if (respawnNeeded) {
    if (level < 3) {
      spawn_apl_level_for2();  
    }
    if (level >= 3) {
      spawn_apl_level3();  
    }
    if (level == 4) {
      spawn_apl_level4();  
    } 
    if(level >=5){
      spawn_apl_level5();
    }
  }   
}

void check_collision() {

  if (position[0][0] == barrier_position[0] && position[0][1] == barrier_position[1]) {
    gameover();
  }

  for (int i = 2; i < element; i++) {
    if (position[0][0] == position[i][0] && position[0][1] == position[i][1]) {
      gameover();
    }
  }
}

// Add a new square to the snake
void append_square() {
  position[element][0] = position[element - 1][0];
  position[element][1] = position[element - 1][1];
  element++;
}

// Delay and check for joystick input
void Delay_chk(int ms) { 
  for (int i = 0; i < ms; i++) {
    delay(1);
    if (analogRead(A0) == 1023 and richtung != 3) {
      richtung = 1; 
    }
    if (analogRead(A1) == 0    and richtung != 4) {
      richtung = 2; 
    }
    if (analogRead(A0) == 0    and richtung != 1) {
      richtung = 3; 
    }
    if (analogRead(A1) == 1023 and richtung != 2) {
      richtung = 4; 
    }
  }
}

// Other functions remain unchanged
void Quadrat(int x_pos, int y_pos, int color) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      tft.drawPixel(x_pos * 10 + i, y_pos * 10 + j, color);
    }
  }
}

void Design() { 
  tft.fillScreen(ILI9341_DARKGREEN);      
  tft.drawRect(9, 29, 222, 282, ILI9341_BLACK); 

  tft.setCursor(50, 5);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(3);
  tft.println("SCORE: ");
  tft.setCursor(175, 5);
  tft.println(score);
  Apple_();
}

void draw_score () { 
  tft.setCursor(175, 5);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_DARKGREEN);
  tft.println(score - 1);

  tft.setCursor(175, 5);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_BLACK);
  tft.println(score);
}

void apple__snake() { 

  tft.fillRect(80, 30, 130, 10, ILI9341_ORANGE);
  tft.fillRect(200, 30, 10, 70, ILI9341_ORANGE);
  tft.fillRect(30, 90, 170, 10, ILI9341_ORANGE);
  tft.fillRect(30, 90, 10, 80, ILI9341_ORANGE);
  tft.fillRect(30, 170, 80, 10, ILI9341_ORANGE);

  tft.fillRect(104, 176, 2, 2, ILI9341_BLACK);
  tft.fillRect(104, 172, 2, 2, ILI9341_BLACK);
  tft.fillRect(110, 174, 4, 2, ILI9341_RED);

  tft.fillCircle(140, 174, 5, ILI9341_RED);
  tft.drawCircle(140, 174, 6, ILI9341_BLACK);
  tft.fillCircle(143, 165, 1, ILI9341_GREEN);
  tft.fillCircle(144, 165, 1, ILI9341_GREEN);
  tft.fillCircle(144, 166, 1, ILI9341_GREEN);
  tft.drawLine(140, 170, 143, 163, ILI9341_MAROON);
}

void Logo (){  
  tft.setCursor(50, 52);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(5);
  tft.println("SNAKE");
  tft.setCursor(65, 105);
  tft.println("GAME_G2");
}

void Text_start(){  
  tft.setCursor(20, 240);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println("> START GAME <");

}

void Apple_() { 
  tft.fillCircle(220, 17, 8, ILI9341_RED);
  tft.drawCircle(220, 17, 9, ILI9341_BLACK);
  tft.fillCircle(223, 8, 2, ILI9341_GREEN);
  tft.fillCircle(224, 8, 2, ILI9341_GREEN);
  tft.fillCircle(224, 9, 2, ILI9341_GREEN);
  tft.drawLine(220, 13, 223, 3, ILI9341_MAROON);
  tft.drawLine(219, 13, 222, 3, ILI9341_MAROON);
}

void check_highscore() { 
  if (score > highscore) {   
    highscore = score;

    tft.setCursor(40, 230);          
    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(2);
    tft.println("NEW HIGHSCORE!");
  }

  tft.setCursor(40, 5);           
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.println("HIGHSCORE:");
  tft.setCursor(180, 5);
  tft.println(highscore);
}

void gameover() { 
  tft.fillScreen(ILI9341_BLACK);    
  tft.setCursor(15, 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(4);
  tft.println("GAME OVER");

  tft.setCursor(70, 200);           
  tft.setTextColor(ILI9341_GREENYELLOW);
  tft.setTextSize(2);
  tft.println("Score:");
  tft.setCursor(155, 200);
  tft.println(score);

  check_highscore();

  tft.setCursor(7, 300);         
  tft.setTextColor(ILI9341_DARKGREY);
  tft.setTextSize(2);
  tft.println("<Play>");

  while (digitalRead(2) == HIGH) {};
  reset_game();
}

void reset_game() { 

  richtung = 1;
  score = 0;
  level = 1;
  speed = 300;
  element = 3;


  for (int i = 0; i < 3; i++) {
    position[i][0] = 12;
    position[i][1] = 16 + i;
  }

  game(); 
}

bool barrier_placed = false; 
void barrier_placing() {
  if (barrier_placed) return; 

  // Set a position for the barrier
  barrier_position[0] = random(1, 23); 
  barrier_position[1] =  random(3, 31); 

  
  tft.setCursor(barrier_position[0] * 10, barrier_position[1] * 10);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.print("2");  

  
  for (int i = 0; i < element; i++) {
    if (position[i][0] == barrier_position[0] && position[i][1] == barrier_position[1]) {
      
      break;
      }
    }

    
    if (abs(barrier_position[0] - apple_position[0]) < 3 && abs(barrier_position[1] - apple_position[1]) < 3) {
    }
  }



void draw_level() {
  tft.setCursor(5, 20);  
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Level: ");
  tft.print(level);
}

void shift_snake_to_corner() {
  
  int center_x = 16;
  int center_y = 20;
  int corner_x = 1;
  int corner_y = 3;

  if (position[0][0] > center_x - 5 && position[0][0] < center_x + 5 &&
      position[0][1] > center_y - 5 && position[0][1] < center_y + 5) {
    
    position[0][0] = corner_x;
    position[0][1] = corner_y;
    for (int i = 1; i < element; i++) {
      position[i][0] = corner_x;
      position[i][1] = corner_y + i;
    }
    Apple_();  
  }
}



void spawn_apl_level_for2() {
  bool validPosition = false;
  
  while (!validPosition) {
    int random_x = random(1, 23);   
    int random_y = random(3, 31);   
    validPosition = true;

    // Check if apple spawns on the snake
    for (int i = 0; i < element; i++) {
      if (position[i][0] == random_x && position[i][1] == random_y) {
        validPosition = false;  
        break;
      }
    }

    // Check if the apple spawns too close to the barrier 
    if (level == 2 && abs(random_x - barrier_position[0]) < 3 && abs(random_y - barrier_position[1]) < 3) {
      validPosition = false;  
    }

    if (validPosition) {
      apple_position[0] = random_x;
      apple_position[1] = random_y;
      Quadrat(random_x, random_y, ILI9341_YELLOW);  
    }
  }
}

void spawn_apl_level3() {
  bool validPosition = false;

  while (!validPosition) {
    int random_x = random(1, 23);   
    int random_y = random(3, 31);   
    validPosition = true;

    // Check if apple spawns on the snake
    for (int i = 0; i < element; i++) {
      if (position[i][0] == random_x && position[i][1] == random_y) {
        validPosition = false;  
        break;
      }
    }

    // Check if apple is too close to the barrier
    if (abs(random_x - barrier_position[0]) < 3 && abs(random_y - barrier_position[1]) < 3) {
      validPosition = false;  
    }

    if (validPosition) {
      apple_position[0] = random_x;
      apple_position[1] = random_y;
      Quadrat(random_x, random_y, ILI9341_YELLOW);  

      
      food_start_time = millis(); 
      food_visible = true;         
    }
  }
}


void check_food_timer3() {
  unsigned long current_time = millis();

  if (food_visible && (current_time - food_start_time >= food_display_duration)) {
    
    Quadrat(apple_position[0], apple_position[1], ILI9341_DARKGREEN);  
    food_visible = false;  

    
    delay(100);  
    spawn_apl_level3();  
  }
}


void draw_countdown_timer() {
  if (food_visible) {  
    int time_left = (food_display_duration - (millis() - food_start_time)) / 1000; 
    tft.setCursor(160, 5);  
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_DARKGREEN);  
    tft.println(time_left);
  }
}

void erase_barrier() {
  
  tft.setCursor(barrier_position[0] * 10, barrier_position[1] * 10);
  tft.setTextColor(ILI9341_DARKGREEN);  
  tft.setTextSize(3);
  tft.print("6");  
}

void spawn_apl_level4() {
    bool RedappleValid = false;

    

    
    if (Red_apple_count < 1) {  
        while (!RedappleValid) {
            int random_x = random(1, 23);
            int random_y = random(3, 31);
            RedappleValid = true;

            
            for (int i = 0; i < element; i++) {
                if (position[i][0] == random_x && position[i][1] == random_y) {
                    RedappleValid = false;
                    break;
                }
            }
            if ((random_x == apple_position[0] && random_y == apple_position[1]) ||
                (abs(random_x - barrier_position[0]) < 3 && abs(random_y - barrier_position[1]) < 3)) {
                RedappleValid = false;
            }

            if (RedappleValid) {
                Red_apple_positions[Red_apple_count][0] = random_x;
                Red_apple_positions[Red_apple_count][1] = random_y;
                Red_apple_active[Red_apple_count] = true;  
                Quadrat(random_x, random_y, ILI9341_RED);  
                Red_apple_count++;  
            }
        }
    }

    food_start_time = millis();  
}

void spawn_apl_level5() {
    bool RedappleValid = false;


   
    if (Red_apple_count > (2) && Red_apple_count < 10) {  
        while (!RedappleValid) {
            int random_x = random(1, 23);
            int random_y = random(3, 31);
            RedappleValid = true;

           
            for (int i = 0; i < element; i++) {
                if (position[i][0] == random_x && position[i][1] == random_y) {
                    RedappleValid = false;
                    break;
                }
            }
            if ((random_x == apple_position[0] && random_y == apple_position[1]) ||
                (abs(random_x - barrier_position[0]) < 3 && abs(random_y - barrier_position[1]) < 3)) {
                RedappleValid = false;
            }

            if (RedappleValid) {
                Red_apple_positions[Red_apple_count][0] = random_x;
                Red_apple_positions[Red_apple_count][1] = random_y;
                Red_apple_active[Red_apple_count] = true;  
                Quadrat(random_x, random_y, ILI9341_RED);  
                Red_apple_count++;  
            }
        }
      
    }

    food_start_time = millis();  
}
