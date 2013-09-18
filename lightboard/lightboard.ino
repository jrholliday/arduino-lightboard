/*
 * LIGHT BOARD
 * 
 * A controller for a Looney Pyramids playing board.
 */

#define redPin       11
#define greenPin     10
#define bluePin       9

#define pulse_delay  5
#define flow_delay   10

volatile int player_mode;
volatile int current_player;

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
  setColor(colors[0]);
  
  //attachInterrupt(0, setMode,    HIGH);
  attachInterrupt(1, nextPlayer, HIGH);
  
  Serial.begin(9600);      
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
  Serial.print(player_mode);
  Serial.print(" ");
  Serial.print(current_player);
  Serial.print("\n");

  if ( player_mode == 0 )
  {
    flow();
  }
  else if ( player_mode == 1 )
  {
    // no op
  }
  else
  {
    pulse();
  }
}

void setMode()
{
  //player_mode = (mode)(player_mode+1 % N_modes);
}

void nextPlayer()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 200) 
  {
    current_player = ((current_player+1) % player_mode);
    setColor(colors[current_player+1]);

    Serial.print(current_player);
    Serial.print("  ");
    Serial.print(player_mode);
    Serial.print("\n");
  }

  last_interrupt_time = interrupt_time;
}

void pulse()
{
  int r,g,b;

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

