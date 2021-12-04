#include "FastLED.h"

// Pride2015
// Animated, ever-changing rainbows.
// by Mark Kriegsman

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// LED Stuff
#define DATA_PIN    2
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    29
#define BRIGHTNESS  255  // Limit global brightness
CRGB leds[NUM_LEDS];  // store LED data that will be output
CRGB ledA[NUM_LEDS];  // two arrays to store calculated data into
CRGB ledB[NUM_LEDS];  // it is possible to fade between them

// LED strip part lengths
#define L1 2  // back (beginning of strip)
#define L2 7  // left
#define L3 11 // front
#define L4 7  // right
#define L5 2  // back (end of strip)

// LED strip start positions
#define L1p 0  // beginning of strip
#define L2p L1
#define L3p L1+L2
#define L4p L1+L2+L3
#define L5p L1+L2+L3+L4

// Button Stuff
const int modeBtn = 6;
const int brightBtn = 4;
int modeState = 0;
int brightState = 0;
int lastModeState = LOW;
int lastBrightState = LOW;
unsigned long modeBtnTime = 0;
unsigned long brightBtnTime = 0;
unsigned long debounceDelay = 50;

// Modes and Brightnesses
#define NUM_MODES 8 // 4 orientations, white, pannenblinker, drehlicht, regenbogen
#define NUM_BRIGHTNESS 6  // 3 brightnesses, each with all LEDs or just a few
int mode = 7;//0;
int brightness = 3;  // brightness level

// Animations
int animPos = 0;  // last position of the animation
unsigned long animTimer = 0;  // time of last animation step
#define ANIM_STEP 30          // how many ms between steps
uint8_t hue=0; 


void setup() {
  delay(100); // 3 second delay for recovery

  // init buttons
  pinMode(modeBtn, INPUT_PULLUP);
  pinMode(brightBtn, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  lastModeState = digitalRead(modeBtn);
  lastBrightState = digitalRead(brightBtn);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    //.setDither(BRIGHTNESS < 255);
    .setDither( BINARY_DITHER );
  FastLED.setBrightness ( BRIGHTNESS );

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


void loop()
{
  // READ BUTTONS
  
  modeState = digitalRead(modeBtn);
  brightState = digitalRead(brightBtn);
  
  if( modeState != lastModeState ){
    lastModeState = modeState;
    if( modeState == LOW ){
      mode++;
      if( mode >= NUM_MODES )
        mode = 0;
    }
    else{
      // button released: do nothing
    }
  }

  if( brightState != lastBrightState ){
    lastBrightState = brightState;
    if( brightState == LOW ){
      brightness++;
      if( brightness >= NUM_BRIGHTNESS )
        brightness = 0;
    }
    else{
      // button released: do nothing
    }
  }
  


  // LIGHT LEDS

  if( mode == 0 ){
    // white front
    fill_solid( leds    , L1, CRGB::Red );
    fill_solid( leds+L2p, L2, CRGB::Yellow );
    fill_solid( leds+L3p, L3, CRGB::White );
    fill_solid( leds+L4p, L4, CRGB::Yellow );
    fill_solid( leds+L5p, L5, CRGB::Red );
    killHalf();
  }

  
  if( mode == 1 ){
    // white left (for right side of rear rack)
    fill_solid( leds    , L1, CRGB::Yellow );
    fill_solid( leds+L2p, L2, CRGB::White );
    fill_solid( leds+L3p, L3, CRGB::Yellow );
    fill_solid( leds+L4p, L4, CRGB::Red );
    fill_solid( leds+L5p, L5, CRGB::Yellow );
    killHalf();
  }

  
  if( mode == 2 ){
    // white back (no use?)
    fill_solid( leds    , L1, CRGB::White );
    fill_solid( leds+L2p, L2, CRGB::Yellow );
    fill_solid( leds+L3p, L3, CRGB::Red );
    fill_solid( leds+L4p, L4, CRGB::Yellow );
    fill_solid( leds+L5p, L5, CRGB::White );
    killHalf();
  }

  
  if( mode == 3 ){
    // white right (for left side of rear rack)
    fill_solid( leds    , L1 , CRGB::Yellow );
    fill_solid( leds+L2p, L2, CRGB::Red );
    fill_solid( leds+L3p, L3, CRGB::Yellow );
    fill_solid( leds+L4p, L4, CRGB::White );
    fill_solid( leds+L5p, L5, CRGB::Yellow );
    killHalf();
  }

  
  if( mode == 4 ){
    // all white (camping light)
    // brightness level 4+5: only front
    if( brightness > 3){
      fill_solid( leds, NUM_LEDS, CRGB::Black );
      fill_solid( leds+L3p, L3, CRGB::Goldenrod );
    }
    else
      fill_solid( leds, NUM_LEDS, CRGB::Goldenrod );
  }

  
  if( mode == 5 ){
    // orange blinking (Pannenblinker)
    if( (millis() % 1000) > 530 )
      fill_solid( leds, NUM_LEDS, CRGB::Orange );
    else
      fill_solid( leds, NUM_LEDS, CRGB::Black );

    killHalf();
  }

  
  if( mode == 6 ){
    // orange rotating light
    FastLED.clear();

    if( (animTimer + ANIM_STEP) < millis() ){
      animTimer = millis();
      animPos++;
    }
    if( animPos >= NUM_LEDS )
      animPos = 0;

    // set length of lit LEDs to half of the strip,
    // except in brightness modes 4+5: only 1/4
    int lightLength;
    if( brightness < 4)
      lightLength = NUM_LEDS/2;
    else
      lightLength = NUM_LEDS/4;

    for( int i=0; i<NUM_LEDS; i++ ){
      if( i < lightLength ){
        leds[(i+animPos)%NUM_LEDS] = CRGB::Orange;
      }
    }
  }

  
  if( mode == 7 ){
    // wild things...
/*    uint8_t ratio;
    if( (millis()%10000) < 4000 )
      ratio = 0;
    else if( (millis()%10000) < 5000 )
      ratio = (millis()-4000) >> 2;
    else if( (millis()%10000) < 9000 )
      ratio = 255;
    else
      ratio = (10000-millis()) >> 2;

    //pride(leds);
    pride(ledA);
    Fire2012(ledB);

    for (int i = 0; i < NUM_LEDS; i++) {                        // mix the 2 arrays together
      leds[i] = blend( ledA[i], ledB[i], ratio );
    }*/

    persistentLines(leds);

    // brightness modes 4 and 5: Kill every other LED
    if( brightness >= 4 ){
      for( int i=0; i<NUM_LEDS; i++ ){
        if( i%2 ){
          leds[i] = CRGB::Black;
        }
      }
    }
  }



  // BRIGHTNESS

  // Levels 0-3: full, half, quarter, 1/8 brightness
  if( brightness < 4 )
    FastLED.setBrightness(BRIGHTNESS >> brightness);
  // Level 4: half, with most LEDs off (depends on mode)
  else if( brightness == 4 )
    FastLED.setBrightness(BRIGHTNESS >> 1);
  // Level 5: 1/8, with most LEDs off (depends on mode)
  else
    FastLED.setBrightness(BRIGHTNESS >> 3);
  


  // SHOW
  
  FastLED.show();

  // wait for next round (10 ms = 100 Hz)
  delay(10);
}


// kill some LEDs to save energy
void killHalf(){
  if( brightness >= 4 ){
    fill_solid( leds    , L1, CRGB::Black );
    fill_solid( leds+L2p+2, L2-4, CRGB::Black );
    fill_solid( leds+L3p+2, L3-4, CRGB::Black );
    fill_solid( leds+L4p+2, L4-4, CRGB::Black );
    fill_solid( leds+L5p, L5, CRGB::Black );
  }
}


void pride(CRGB* lx) 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( lx[pixelnumber], newcolor, 64);
  }
}


void lines(CRGB* lx)
{
  static uint8_t len;
  static uint16_t pos;    // 16x accuracy, i. e. /16 (>>4) for led position
  static uint8_t spd;
  static uint8_t col;

  // create a new line
  if( len == 0 ){
    len = random8(2, 10);
    pos = 0;
    col = random8();  // hue
    spd = random8(2, 6);
  }
  uint8_t posl = (pos>>4);

  FastLED.clear();  // clear all

  for( uint8_t i=0; i<NUM_LEDS; i++ ){
    if( i < (posl - len) ){
      // remain black
    }
    else if( i < (posl - len + 1) ){
      // trailing led: fade out
      lx[i] = CHSV( col, 255, 255-((pos%16)*16) );
    }
    else if( i > (posl + 1) ){
      // remain black
    }
    else if( i > posl ){
      // leading led: fade in
      lx[i] = CHSV( col, 255, (pos%16)*16 );
    }
    else{
      // the line: full brightness
      lx[i] = CHSV( col, 255, 255);
    }
  }

  pos += spd;

  // reached the end of the strip? start a new line next time
  if( (pos>>4) >= (NUM_LEDS + len + 1) ){
    len = 0;
  }
}


void persistentLines(CRGB* lx)
{
  static uint16_t pos;    // 16x accuracy, i. e. /16 (>>4) for led position. Allows for fading in the LEDs
  static uint8_t spd; // speed
  static uint8_t col; // colour

  // create a new line
  if( pos == 0 ){
    col += random8(48, 208);  // hue
    spd = random8(3, 8);
  }
  uint8_t posl = (pos>>4);  // discrete LED positions (pos/16)

  for( uint8_t i=0; i<NUM_LEDS; i++ ){
    if( i < (posl) ){
      // the line: full brightness
      lx[i] = CHSV( col, 255, 255);
    }
  }

  pos += spd;

  // reached the end of the strip? start a new line next time
  if( (pos>>4) > NUM_LEDS ){
    pos = 0;
  }
}


bool gReverseDirection = false;
void Fire2012(CRGB* lx)
{
  #define COOLING  55
  #define SPARKING 100
  
  // Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // skip 3 out of 4 iterations to make it slooooow
  if( (millis()%80) < 10 ){
    // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      lx[pixelnumber] = color;
    }
  }
}
