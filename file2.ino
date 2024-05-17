#define CHOICE_OFF      0 //Used to control LEDs
#define CHOICE_NONE     0 //Used to check buttons
#define CHOICE_RED  (1 << 0)
#define CHOICE_GREEN    (1 << 1)
#define CHOICE_BLUE (1 << 2)
#define CHOICE_YELLOW   (1 << 3)

// Led pin definitions
#define LED_RED     13
#define LED_GREEN   3
#define LED_BLUE    5
#define LED_YELLOW  10

// Button pin definitions
#define BUTTON_RED    12
#define BUTTON_GREEN  2
#define BUTTON_BLUE   6
#define BUTTON_YELLOW 9

// Buzzer pin definitions
#define BUZZER1  4
#define BUZZER2  7

// Define game parameters
#define ROUNDS_TO_WIN  10 //Number of rounds to win the game.
#define ENTRY_TIME_LIMIT   3000 //Time to press a button

byte gameSeq[10]; //sequence of leds lightened
byte gameRound = 0; //number of successfull rounds player has achieved
byte delay_time = 150; // Amount of time between two leds

void setup()
{
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  play_winner(); // After setup is complete
}

void loop()
{
  wait_to_start(); // Blink lights while waiting for user to start
 
  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE | CHOICE_YELLOW); // Turn all LEDs on

  delay(1000);
  setLEDs(CHOICE_OFF); // Turn off LEDs
  delay(250);

  if (play_memory() == true) 
    play_winner(); // Player won, play winner tones
  else 
    play_loser(); // Player lost, play loser tones 
}

// Returns 0 if player loses, 1 if player wins
boolean play_memory(void)
{
  randomSeed(millis()); // Seed the random generator with random amount of millis()

  gameRound = 0; // Reset the game 

  while (gameRound < ROUNDS_TO_WIN) 
  {
    add_to_moves(); // Add a button to the current moves

    playMoves(); // Plays back the current game sequence

    // wants the player to repeat the sequence.
    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)
    {
      byte choice = wait_for_button(); // See what button the user presses

      if (choice == 0) return false; // If wait timed out, player loses

      if (choice != gameSeq[currentMove]) return false; // If the choice is incorect, player loses
    }
    delay(1000); 
  }
  delay_time = 150;

  return true; // Player made it through all the rounds to win!
}

// Plays the current contents of the game moves
void playMoves(void)
{
  for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) 
  {
    toner(gameSeq[currentMove], 150);

    // Wait some amount of time between button playback
    delay(delay_time); 
    delay_time = delay_time-5; //shortens wait time by 5 each round
  }
}

// Adds a new random button to the game sequence,
void add_to_moves(void)
{
  byte newButton = random(0, 4); 

  if(newButton == 0) newButton = CHOICE_RED;
  else if(newButton == 1) newButton = CHOICE_GREEN;
  else if(newButton == 2) newButton = CHOICE_BLUE;
  else if(newButton == 3) newButton = CHOICE_YELLOW;
  gameSeq[gameRound++] = newButton; // Add new led to the game array
}

// Lights a given LEDs
void setLEDs(byte leds)
{
  if ((leds & CHOICE_RED) != 0)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);

    
  if ((leds & CHOICE_GREEN) != 0)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);


  if ((leds & CHOICE_BLUE) != 0)
    digitalWrite(LED_BLUE, HIGH);
  else
    digitalWrite(LED_BLUE, LOW);


  if ((leds & CHOICE_YELLOW) != 0)
    digitalWrite(LED_YELLOW, HIGH);
  else
    digitalWrite(LED_YELLOW, LOW);
}

// Wait for a button to be pressed. 
// Returns one of LED colors (LED_RED, etc.) if successful, 0 if timed out

byte wait_for_button(void)
{
  long startTime = millis(); // the time we started the this loop

  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Loop until too much time has passed
  {
    byte button = checkButton();

    if (button != CHOICE_NONE)
    { 
      toner(button, 150); // Play the button the user just pressed

      while(checkButton() != CHOICE_NONE) ;  // wait for user to release button

      delay(10); // This helps with debouncing and accidental double taps

      return button;
    }
  }

  return CHOICE_NONE; // if timed out!
}

// Returns a '1' bit in the position corresponding to CHOICE_RED, CHOICE_GREEN, etc.
byte checkButton(void)
{
  if (digitalRead(BUTTON_RED) == 0) return(CHOICE_RED); 
  else if (digitalRead(BUTTON_GREEN) == 0) return(CHOICE_GREEN); 
  else if (digitalRead(BUTTON_BLUE) == 0) return(CHOICE_BLUE); 
  else if (digitalRead(BUTTON_YELLOW) == 0) return(CHOICE_YELLOW);

  return(CHOICE_NONE); // If no button is pressed, return none
}



// Light an LED and play tone
void toner(byte which, int buzz_length_ms)
{

  setLEDs(which); //Turn on a given LED

  //Play the sound associated with the given LED
  switch(which) 
  {
  case CHOICE_RED:
    buzz_sound(buzz_length_ms, 1136); 
    break;

  case CHOICE_GREEN:
    buzz_sound(buzz_length_ms, 568); 
    break;

  case CHOICE_BLUE:
    buzz_sound(buzz_length_ms, 851); 
    break;

  case CHOICE_YELLOW:
    buzz_sound(buzz_length_ms, 638); 
    break;
  }
  setLEDs(CHOICE_OFF); // Turn off all LEDs
}


// Toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms.

void buzz_sound(int buzz_length_ms, int buzz_delay_us)
{
  // Convert total play time from milliseconds to microseconds
  long buzz_length_us = buzz_length_ms * (long)1000;

  // Loop until the remaining play time is less than a single buzz_delay_us
  while (buzz_length_us > (buzz_delay_us * 2))
  {
    buzz_length_us -= buzz_delay_us * 2; //Decrease the remaining play time

    // Toggle the buzzer at various speeds
    digitalWrite(BUZZER1, LOW);
    digitalWrite(BUZZER2, HIGH);
    delayMicroseconds(buzz_delay_us);

    digitalWrite(BUZZER1, HIGH);
    digitalWrite(BUZZER2, LOW);
    delayMicroseconds(buzz_delay_us);
  }
}

// Play the winner sound and lights
void play_winner(void)
{
  setLEDs(CHOICE_GREEN | CHOICE_BLUE);
  winner_sound();

  setLEDs(CHOICE_RED | CHOICE_YELLOW);
  winner_sound();

  setLEDs(CHOICE_GREEN | CHOICE_BLUE);
  winner_sound();

  setLEDs(CHOICE_RED | CHOICE_YELLOW);
  winner_sound();
}

// Play the winner sound
void winner_sound(void)
{
  // Toggle the buzzer at various speeds
  for (byte x = 250 ; x > 70 ; x--)
  {
    for (byte y = 0 ; y < 3 ; y++)
    {
      digitalWrite(BUZZER2, HIGH);
      digitalWrite(BUZZER1, LOW);
      delayMicroseconds(x);

      digitalWrite(BUZZER2, LOW);
      digitalWrite(BUZZER1, HIGH);
      delayMicroseconds(x);
    }
  }
}
// Play the loser sound/lights
void play_loser(void)
{
  setLEDs(CHOICE_RED | CHOICE_GREEN);
  buzz_sound(255, 1500);

  setLEDs(CHOICE_BLUE | CHOICE_YELLOW);
  buzz_sound(255, 1500);

  setLEDs(CHOICE_RED | CHOICE_GREEN);
  buzz_sound(255, 1500);

  setLEDs(CHOICE_BLUE | CHOICE_YELLOW);
 buzz_sound(255, 1500);
}

// while waiting leds lightens in a sequence
void wait_to_start(void)
{
  while(1) 
  {
    setLEDs(CHOICE_RED);
    delay(100);

    if (checkButton() != CHOICE_NONE) return;

    setLEDs(CHOICE_BLUE);
    delay(100);

    if (checkButton() != CHOICE_NONE) return;

    setLEDs(CHOICE_GREEN);
    delay(100);

    if (checkButton() != CHOICE_NONE) return;

    setLEDs(CHOICE_YELLOW);
    delay(100);

    if (checkButton() != CHOICE_NONE) return;
  }
}
