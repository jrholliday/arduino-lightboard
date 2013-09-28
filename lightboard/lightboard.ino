/*
 * LIGHT BOARD
 * 
 * A controller for a Looney Pyramids playing board.
 */

#define bluePin              9
#define redPin              10
#define greenPin            11

#define base_pulse_delay    25
#define flash_delay      10000
#define ramp_delay          20
#define bounce_delay       100

volatile unsigned long last_interrupt_time;

volatile int player_mode;
volatile int current_player;
volatile int pulse_delay;
volatile bool interrupt;

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

  player_mode    = 0;
  current_player = 0;
  pulse_delay    = base_pulse_delay;
  setColor(colors[0]);
  
  //attachInterrupt(0, setMode,    RISING);
  //attachInterrupt(1, nextPlayer, RISING);

  attachInterrupt(1, setMode, RISING);

  last_interrupt_time = millis();
  interrupt = false;
  Serial.begin(9600);
}

void setColor(int r, int g, int b, int scale=100)
{
  analogWrite(redPin,   (scale*r/100));
  analogWrite(greenPin, (scale*g/100));
  analogWrite(bluePin,  (scale*b/100));  
}
void setColor(int rgb[3])
{
  analogWrite(redPin,   rgb[0]);
  analogWrite(greenPin, rgb[1]);
  analogWrite(bluePin,  rgb[2]);  
}

void flash(int N)
{
  for (int i=0; i<N; i++)
  {
    for (int j=0; j<N; j++)
    {
      setColor(colors[j+1]);
      delay(flash_delay);
      setColor(0,0,0);
      delay(flash_delay/2);
    }
  }
}

void loop()
{
  interrupt = false;

  switch ( player_mode )
  {
    case 0:
      ramp();
      break;
    case 1:
      // Do nothing special
      break;
    default:
      pulse();
  }
}

void setMode()
{
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > bounce_delay) 
  {
    interrupt = true;

    switch ( player_mode )
    {
      case 0:
        player_mode = 1;
        setColor(colors[0]);
        break;
      case 1:
        player_mode = 2;
        flash(player_mode);
        break;
      case 2:
        player_mode = 3;
        flash(player_mode);
        break;
      case 3:
        player_mode = 4;
        flash(player_mode);
        break;
      case 4:
        player_mode = 0;
        break;
    }
  }
  
  last_interrupt_time = millis();
}

void nextPlayer()
{
  Serial.print(current_player);
  Serial.print("   ");
  Serial.print(player_mode);
  Serial.print("\n");
  
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > bounce_delay) 
  {
    current_player = ((current_player+1) % player_mode);
    setColor(colors[current_player+1]);

    pulse_delay = base_pulse_delay;
    interrupt = true;
  }

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
    if ( interrupt ) return;

    r = colors[current_player+1][0];
    g = colors[current_player+1][1];
    b = colors[current_player+1][2];

    setColor(r,g,b,i);
    delay(pulse_delay);
  }    
  for (int i=25; i<=100; i++)
  {
    if ( interrupt ) return;

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
    if ( interrupt ) return;
    setColor(256-i, i, 0);
    delay(ramp_delay);
  }
  for (int i=0; i<256; i++)
  {
    if ( interrupt ) return;
    setColor(0, 255-i, i);
    delay(ramp_delay);
  }
  for (int i=0; i<255; i++)
  {
    if ( interrupt ) return;
    setColor(i, 0, 255-i);
    delay(ramp_delay);
  }
}

