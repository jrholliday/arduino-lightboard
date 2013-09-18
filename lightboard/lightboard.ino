/*
 * LIGHT BOARD
 * 
 * A controller for a Looney Pyramids playing board.
 */

#define redPin       11
#define greenPin     10
#define bluePin       9

#define pulse_delay  10
#define flow_delay   10
enum mode {
  players_0,
  players_1,
  players_2,
  players_3,
  players_4,
  N_modes
};

volatile mode player_mode; 
volatile int  current_player;

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

  player_mode    = players_0;
  current_player = 0;
  setColor(colors[0]);
  
  attachInterrupt(2, setMode,    RISING);
  attachInterrupt(3, nextPlayer, RISING);
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
  if ( player_mode == players_0 )
  {
    flow();
  }
  else if ( player_mode == players_1 )
  {
    // no op
  }
  else
  {
    pulse();
  }
  
  
  
  
  return;
  
  setColor(255, 255, 255); // white
  delay(1000);
  
  setColor(0, 255, 0);  // green
  delay(1000);
  setColor(0, 0, 255);  // blue
  delay(1000);
  setColor(255, 0, 0);  // red
  delay(1000);
  setColor(255, 255, 0); // yellow
  delay(1000); 
}

void setMode()
{
  player_mode = (mode)(player_mode+1 % N_modes);
}

void nextPlayer()
{
  current_player = (current_player+1) % player_mode;
  setColor(colors[current_player+1]);
}

void pulse()
{
  int r = colors[current_player+1][0];
  int g = colors[current_player+1][1];
  int b = colors[current_player+1][2];

  for (int i=100; i>=25; i--)
  {
    setColor(r,g,b,i);
    delay(pulse_delay);
  }    
  for (int i=25; i<=100; i++)
  {
    setColor(r,g,b,i);
    delay(pulse_delay);
  }    
}

void flow()
{
  for (int i=0; i<256; i++)
  {
    setColor(256-i, i, 0);
    delay(flow_delay);
  }
  for (int i=0; i<256; i++)
  {
    setColor(0, 255-i, i);
    delay(flow_delay);
  }
  for (int i=0; i<255; i++)
  {
    setColor(i, 0, 255-i);
    delay(flow_delay);
  }
}

