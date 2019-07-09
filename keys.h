// Заголовочный файл определения клавиши

// возможно, НЕ НУЖЕН.

#define is_downkey(a)  (a == 's' || a == 258 || a % 256 == 2)
#define is_upkey(a)    (a == 'w' || a == 259 || a % 256 == 3)
#define is_leftkey(a)  (a == 'a' || a == 260 || a % 256 == 4)
#define is_rightkey(a) (a == 'd' || a == 261 || a % 256 == 5)
#define is_escape(a)   (            a == 27  || a % 256 == 27)
#define is_enter(a)    (a == '\n')
#define is_tab(a)      (a == '\t')
#define is_space(a)    (a == ' ')

