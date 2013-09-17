/*
 * LIGHT BOARD
 * 
 * A controller for a Looney Pyramids playing board.
 */

int redPin =   11;
int greenPin = 10;
int bluePin =   9;

void setup()
{
  pinMode(redPin,   OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin,  OUTPUT);  
}

void loop()
{
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

  for (int j=0; j<3; j++)
  {
    for (int i=0; i<256; i++)
    {
      setColor(256-i, i, 0);
      delay(5);
    }
    for (int i=0; i<256; i++)
    {
      setColor(0, 255-i, i);
      delay(5);
    }
    for (int i=0; i<255; i++)
    {
      setColor(i, 0, 255-i);
      delay(5);
    }
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin,   255-red);
  analogWrite(greenPin, 255-green);
  analogWrite(bluePin,  255-blue);  
}
