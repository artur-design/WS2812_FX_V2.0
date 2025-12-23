/*
  Скетч создан на основе FASTSPI2 EFFECTS EXAMPLES автора teldredge (www.funkboxing.com)
  А также вот этой статьи https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#cylon
  Доработан, переведён и разбит на файлы 2017 AlexGyver
  Смена выбранных режимов кнопкой. Кнопка подключена на D2 и GND
  В последствии доработан и оптимизирован artur-design
  Это бета версия, поэтому тут немного бардак в коде. Serial port по умолчанию включён, если не нужен - закомментируйте. 
*/

#include "FastLED.h"          // библиотека для работы с лентой

#define LED_COUNT 180          // число светодиодов в кольце/ленте
#define LED_DT 8             // пин, куда подключен DIN ленты

byte max_bright = 100;          // максимальная яркость (0 - 255)
//boolean adapt_light = 1;       // адаптивная подсветка (1 - включить, 0 - выключить)

byte fav_modes[] = {255, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 23, 24, 25, 27, 28, 29, 51, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 46, 255, 50, 104, 105};  // список "любимых" режимов
//byte fav_modes[] = {2, 36};
byte num_modes = sizeof(fav_modes);         // получить количество "любимых" режимов (они все по 1 байту..)
//unsigned long change_time, last_change, last_bright;
//int new_bright;

volatile byte ledMode = 0;
/*
  Стартовый режим
  0 - все выключены
  1 - все включены
  3 - кольцевая радуга
  47 - демо-режим
*/

// цвета мячиков для режима
byte ballColors[3][3] = {
  {220, 255, 255},
  {120, 255, 255},
  {50, 255, 255}
};

byte newkittmode[] = {1, 2, 3, 4, 2, 1, 3, 4}; //1 - слева направо, 2 - справа налево, 3 - с краёв к центру, 4 - с центра к краям
byte parovoz[] = {2, 1};

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
byte TOP_INDEX = LED_COUNT / 2;
boolean EVENODD = LED_COUNT % 2;
struct CRGB leds[LED_COUNT];

int thisdelay = 20;          //-FX LOOPS DELAY VAR
byte thisstep = 10;           //-FX LOOPS DELAY VAR
byte thishue = 0;             //-FX LOOPS DELAY VAR
byte thissat = 255;           //-FX LOOPS DELAY VAR
byte thue = 0;

int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
byte ihue = 0;                //-HUE (0-255)
byte ibright = 0;             //-BRIGHTNESS (0-255)
byte isat = 0;                //-SATURATION (0-255)
boolean bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;          //-INC VAR FOR SIN LOOPS

volatile uint32_t btnTimer;
volatile byte modeCounter;
volatile boolean changeFlag;

byte ta = 0;
byte tb = 0;
byte tc = 0;
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------


void setup()
{
  Serial.begin(9600);              // открыть порт для связи
  LEDS.setBrightness(max_bright);  // ограничить максимальную яркость

  LEDS.addLeds<WS2812B, LED_DT, GRB>(leds, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  setAll(0, 0, 0);          // погасить все светодиоды

  randomSeed(analogRead(0));
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, btnISR, FALLING);
}

void loop() {
  /*
  if (adapt_light) {                        // если включена адаптивная яркость
    if (millis() - last_bright > 500) {     // каждые полсекунды
      last_bright = millis();               // сброить таймер
      new_bright = map(analogRead(6), 1, 1000, 3, max_bright);   // считать показания с фоторезистора, перевести диапазон
      LEDS.setBrightness(new_bright);        // установить новую яркость
    }
  }
  */

  
    if (Serial.available() > 0) {     // если что то прислали
      ledMode = Serial.parseInt();    // парсим в тип данных int
      change_mode(ledMode);           // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)
    }
  
  switch (ledMode) {
    case 255: break;                           // пауза
    case  2: rainbow_fade(); break;            // плавная смена цветов всей ленты
    case  3: rainbow_loop(); break;            // крутящаяся радуга
    case  4: random_burst(); break;            // случайная смена цветов
    //case  6: color_bounceFADE(1); break;        // бегающий паровозик светодиодов! паровозик
//    case 34: CylonBounce(1, 1, 255, 255, 255, 4, 10, thisdelay); break;    // бегающие светодиоды паровозик (кр, зел, син, размер, задержка, время)
    case  5: color_bounce(2); break;            // бегающий светодиод! точка
    case  7: ems_lightsONE(1); break;           // вращаются красный и синий! точка
    case  8: ems_lightsALL(1); break;           // вращается половина красных и половина синих! (двухцветный пропеллер)
    case 27: rgb_propeller(1, thishue, thishue + 80, thishue + 160); break;           // RGB пропеллер! (трехцветный пропеллер)
    case  9: flicker(); break;                 // случайный стробоскоп (типа молния)
    case 10: pulse_one_color_all(); break;     // пульсация одним цветом (увеличение, затем уменьшение яркости)
    case 11: pulse_one_color_all_rev(); break; // пульсация со сменой цветов
    case 12: fade_vertical(); break;           // плавная смена яркости по вертикали (для кольца)
    case 13: rule30(); break;                  // безумие красных светодиодов!
    case 14: random_march(); break;            // безумие случайных цветов
    case 15: rwb_march(); break;               // белый синий красный бегут по кругу (ПАТРИОТИЗМ!)
    case 16: radiation(); break;               // пульсирует значок радиации
    case 17: color_loop_vardelay(1, 0); break;     // красный светодиод бегает по кругу! (1 - радужный 0-фиксированный, сдвиг цвета по радуге) точка
    case 18: white_temps(); break;             // бело синий градиент ?
    case 19: sin_bright_wave(); break;         // тоже хрень какая то
    case 20: pop_horizontal(0, 10); break;          // красные вспышки спускаются вниз точка
    //case 21: quad_bright_curve(); break;       // полумесяц
    case 22: flame(); break;                   // эффект пламени
    case 23: rainbow_vertical(); break;        // радуга в вертикаьной плоскости (кольцо)
    case 24: pacman(); break;                  // пакман
    case 25: random_color_pop(); break;        // безумие случайных вспышек
    case 26: ems_lightsSTROBE(0, 160 % 255); break;        // полицейская мигалка!
    case 28: kitt(); break;                    // случайные вспышки красного в вертикаьной плоскости
    case 29: matrix(1); break;                  // зелёненькие бегают по кругу случайно точки
    case 30: new_rainbow_loop(); break;        // крутая плавная вращающаяся радуга
    case 31: strip_march_ccw(); break;         // копирование светодиодов предыдущего режима и движение вперёд
    case 32: strip_march_cw(); break;          // копирование светодиодов предыдущего режима и движение назад
    case 33: colorWipe(0, 255, 0, thisdelay);
      colorWipe(0, 0, 0, thisdelay); break;                                // плавное заполнение цветом
    case 34: NewKITT(1, 1, 255, 255, 255, 8, 10, thisdelay, parovoz, 2); break;             // бегающие светодиоды паровозик (кр, зел, син, размер, задержка, время)
    case 35: Fire(55, 120, thisdelay); break;                                       // линейный огонь
    case 36: NewKITT(1, 1, 255, 255, 255, 8, 10, thisdelay, newkittmode, 8); break;                          // беготня секторов круга (работает)
    case 37: rainbowCycle(thisdelay); break;                                        // очень плавная вращающаяся радуга
    case 38: TwinkleRandom(20, thisdelay, 1); break;                                // случайные разноцветные включения (1 - танцуют все, 0 - случайный 1 диод)
    case 39: RunningLights(0xff, 0xff, 0x00, thisdelay); break;                     // бегущие огни
    case 40: Sparkle(0xff, 0xff, 0xff, thisdelay); break;                           // случайные вспышки белого цвета искры
    case 41: SnowSparkle(0x10, 0x10, 0x10, thisdelay, random(100, 1000)); break;    // случайные вспышки белого цвета на белом фоне искры
    case 42: theaterChase(255, 0, 0, thisdelay); break;                            // бегущие каждые 3 (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
    case 43: theaterChaseRainbow(thisdelay); break;                                 // бегущие каждые 3 радуга (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
    case 44: Strobe(255, 255, 255, 10, thisdelay, 1000); break;                  // стробоскоп

    case 46: BouncingColoredBalls(1, 3, ballColors); break;                            // прыгающие мячики цветные

    case 47: demo_modeA(); break;             // длинное демо
    case 48: demo_modeB(); break;             // короткое демо
    case 49: sparkles(random8(80,200), 100, 30); break; // искры
    case 50: gradient(); break; //смещающийся градиент
    case 101: setAll(255, 0, 0); break; //---ALL RED все красные
    case 102: setAll(0, 255, 0); break; //---ALL GREEN все зелёные
    case 103: setAll(0, 0, 255); break; //---ALL BLUE все синие
    case 104: setAll(255, 255, 0); break; //---ALL COLOR X 
    case 105: setAll(0, 255, 255); break; //---ALL COLOR Y
    case 106: setAll(255, 0, 255); break; //---ALL COLOR Z
    case 51: matrix2(1); break;
  }
}

void btnISR() {
  if (millis() - btnTimer > 150) {
    btnTimer = millis();  // защита от дребезга
    if (++modeCounter >= num_modes) modeCounter = 0;
    ledMode = fav_modes[modeCounter];    // получаем новый номер следующего режима
    change_mode(ledMode);               // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)    
    changeFlag = true;
  }
}
// ---------------НАСТРОЙКИ ЗАДЕРЖКИ И ЦВЕТА-----------------
void change_mode(byte newmode) {
  thissat = 255;
  switch (newmode) {
    case 0: setAll(0, 0, 0); break; //---ALL OFF все выключены
    case 1: setAll(255, 255, 255); break; //---ALL ON все включены
    case 2: thisdelay = 60; break;                      //---STRIP RAINBOW FADE
    case 3: thisdelay = 20; thisstep = 10; break;       //---RAINBOW LOOP
    case 4: setAll(0, 0, 0); thisdelay = 20; break;                      //---RANDOM BURST
    case 5: thisdelay = 20; thishue = 0; break;         //---CYLON v1
    //case 6: thisdelay = 80; thishue = 0; break;         //---CYLON v2
    case 7: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SINGLE
    case 8: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SOLID
    case 9: thishue = 160; thissat = 50; break;         //---STRIP FLICKER
    case 10: thisdelay = 15; thishue = 0; break;        //---PULSE COLOR BRIGHTNESS
    case 11: thisdelay = 30; thishue = 0; break;        //---PULSE COLOR SATURATION
    case 12: thisdelay = 60; thishue = 180; break;      //---VERTICAL SOMETHING
    case 13: thisdelay = 100; break;                    //---CELL AUTO - RULE 30 (RED)
    case 14: thisdelay = 80; break;                     //---MARCH RANDOM COLORS
    case 15: thisdelay = 80; break;                     //---MARCH RWB COLORS
    case 16: setAll(0, 0, 0); thisdelay = 60; thishue = 95; break;       //---RADIATION SYMBOL
    case 17: thishue = 0; break;
    //---PLACEHOLDER FOR COLOR LOOP VAR DELAY VARS
    case 19: thisdelay = 35; thishue = 180; break;      //---SIN WAVE BRIGHTNESS
    case 20: thisdelay = 100; thishue = 0; break;       //---POP LEFT/RIGHT
    //case 21: thisdelay = 100; thishue = 180; break;     //---QUADRATIC BRIGHTNESS CURVE
    //---PLACEHOLDER FOR FLAME VARS
    case 23: thisdelay = 50; thisstep = 15; break;      //---VERITCAL RAINBOW
    case 24: thisdelay = 50; break;                     //---PACMAN
    case 25: thisdelay = 35; break;                     //---RANDOM COLOR POP
    case 26: thisdelay = 25; thishue = 0; break;        //---EMERGECNY STROBE
    case 27: thisdelay = 100; thishue = 0; break;        //---RGB PROPELLER
    case 28: thisdelay = 100; thishue = 0; break;       //---KITT
    case 29: thisdelay = 100; thishue = 95; break;       //---MATRIX RAIN
    case 30: thisdelay = 15; break;                      //---NEW RAINBOW LOOP
    case 31: thisdelay = 100; break;                    //---MARCH STRIP NOW CCW
    case 32: thisdelay = 100; break;                    //---MARCH STRIP NOW CCW
    case 33: thisdelay = 50; break;                     // colorWipe
    case 34: thisdelay = 50; break;                     // CylonBounce
    case 35: thisdelay = 15; break;                     // Fire
    case 36: thisdelay = 50; break;                     // NewKITT
    case 37: thisdelay = 20; break;                     // rainbowCycle
    case 38: thisdelay = 10; break;                     // rainbowTwinkle
    case 39: thisdelay = 50; break;                     // RunningLights
    case 40: thisdelay = 0; break;                      // Sparkle
    case 41: thisdelay = 30; break;                     // SnowSparkle
    case 42: thisdelay = 50; break;                     // theaterChase
    case 43: thisdelay = 50; break;                     // theaterChaseRainbow
    case 44: thisdelay = 100; break;                    // Strobe
    case 50: setAll(0, 0, 0); thisdelay = 30; break;
    case 51: thisdelay = 50; thishue = 95; break;

  }
  bouncedirection = 0;
  //setAll(0, 0, 0);
  ledMode = newmode;
}
