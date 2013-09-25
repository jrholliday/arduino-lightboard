/*
 * LIGHT BOARD
 * 
 * A controller for a Looney Pyramids playing board.
 */

#define redPin            11
#define greenPin          10
#define bluePin            9

#define base_pulse_delay  25
#define ramp_delay        50

volatile unsigned long last_interrupt_time;

volatile int player_mode;
volatile int current_player;
volatile int pulse_delay;

int colors[5][3] = { {255, 255, 255}, // White
                     {255,   0,   0}, // Red
                     {  0, 255,   0}, // Green
                     {  0,   0, 255}, // Blue
                     {255, 255,   0}  // Yellow
                   };

void setup()
{
  pinMode(redPin,   OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin,  OUTPUT);

  player_mode    = 4;
  current_player = 0;
  pulse_delay    = base_pulse_delay;
  setColor(colors[0]);
  
  attachInterrupt(0, setMode,    RISING);
  attachInterrupt(1, nextPlayer, RISING);
  //attachInterrupt(0, debounce,   FALLING);
  //attachInterrupt(1, debounce,   FALLING);

  last_interrupt_time = 0;
}

void setColor(int r, int g, int b, int scale=100)
{
  analogWrite(redPin,   255-(scale*r/100));
  analogWrite(greenPin, 255-(scale*g/100));
  analogWrite(bluePin,  255-(scale*b/100));  
}
void setColor(int rgb[3])
{
  analogWrite(redPin,   255-rgb[0]);
  analogWrite(greenPin, 255-rgb[1]);
  analogWrite(bluePin,  255-rgb[2]);  
}

void loop()
{
  if ( player_mode == 0 )
  {
    ramp();
  }
  else if ( player_mode == 1 )
  {
    // Just shine white
  }
  else
  {
    pulse();
  }
}

void setMode()
{
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 500) 
  {
    player_mode = player_mode+1 % 5;
  }
  
  debounce();
}

void nextPlayer()
{
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 500) 
  {
    current_player = ((current_player+1) % player_mode);
    setColor(colors[current_player+1]);

    pulse_delay = base_pulse_delay;
  }

  debounce();
}

void debounce()
{
  last_interrupt_time = millis();
}

void pulse()
{
  int r,g,b;

  if ( pulse_delay > 0 )
  {
    pulse_delay = pulse_delay - 1;
  }

  for (int i=100; i>=25; i--)
  {
    r = colors[current_player+1][0];
    g = colors[current_player+1][1];
    b = colors[current_player+1][2];

    setColor(r,g,b,i);
    delay(pulse_delay);
  }    
  for (int i=25; i<=100; i++)
  {
    r = colors[current_player+1][0];
    g = colors[current_player+1][1];
    b = colors[current_player+1][2];

    setColor(r,g,b,i);
    delay(pulse_delay);
  }    
}

void ramp()
{
  for (int i=0; i<256; i++)
  {
    setColor(256-i, i, 0);
    delay(ramp_delay);
  }
  for (int i=0; i<256; i++)
  {
    setColor(0, 255-i, i);
    delay(ramp_delay);
  }
  for (int i=0; i<255; i++)
  {
    setColor(i, 0, 255-i);
    delay(ramp_delay);
  }
}

