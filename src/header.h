#define RELEASE

void menu();
int single_game();
int multi_game_create();
int multi_game_join();

void delay(float sec);
int zero_exit();
int single_game_set_frame();
int single_game_set_speed();
int multi_game_set_ip();
int multi_game_set_frame();
int multi_game_set_speed();
char * get_ip(char * file_name);
void pexit(char * message);
void print(char * arr, size_t x, size_t y, int from_x, int from_y);

int minus_one();
int minus_two();

int set(char * message, int min, int max);


#define LOG_NAME "log.txt"
extern FILE * log;

#define SETTINGS_PATH "Settings"
#define SINGLE_GAME_CONFIG(what) SETTINGS_PATH "\\" "singlegame_" what ".cfg"
#define MULTI_GAME_CONFIG(what)  SETTINGS_PATH "\\"  "multigame_" what ".cfg"


