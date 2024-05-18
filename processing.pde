import processing.serial.*;

Serial myPort;
int incomingByte = -1;

boolean gameOver = false;
boolean gameWon = false;
int messageTimer = 0;
final int MESSAGE_DURATION = 60; // Duration of the message

void setup() {
  size(400, 200); // Increase the window size to fit all four LEDs
  println("Available serial ports:");
  printArray(Serial.list());
  
  // Use the correct serial port
  String portName = "COM8";
  myPort = new Serial(this, portName, 9600);
  
  background(0);
}

void draw() {
  background(0); // Clear the background each frame
  
  // Read from the serial port
  if (myPort.available() > 0) {
    incomingByte = myPort.read();
    println("Received: " + incomingByte);
  }
  
  if(incomingByte == 100) {
    gameWon = true;
    gameOver = true;
    messageTimer = MESSAGE_DURATION;
  } else if(incomingByte == 101) {
    gameWon = false;
    gameOver = true;
    messageTimer = MESSAGE_DURATION;
  }
  
  // Light up the corresponding LEDs on the Processing window
  stroke(255); // Set the stroke to white
  
  if ((incomingByte & 2) != 0) {
    fill(0, 255, 0); // Green
  } else {
    noFill();
  }
  ellipse(50, height / 2, 50, 50);
  
  if ((incomingByte & 4) != 0) {
    fill(0, 0, 255); // Blue
  } else {
    noFill();
  }
  ellipse(150, height / 2, 50, 50);
  
  if ((incomingByte & 8) != 0) {
    fill(255, 255, 0); // Yellow
  } else {
    noFill();
  }
  ellipse(250, height / 2, 50, 50);
  
  if ((incomingByte & 1) != 0) {
    fill(255, 0, 0); // Red
  } else {
    noFill();
  }
  ellipse(350, height / 2, 50, 50);
  
  if(gameOver) {
    if(messageTimer > 0) {
      textAlign(CENTER);
      textSize(32);
      fill(255);
      
      if(gameWon) {
        text("Game Won", width / 2, height - 50);
      } else {
        text("Game Over", width / 2, height - 50);
      }
      messageTimer--;
    } else {
      // Reset game state after message duration
      gameOver = false;
      incomingByte = -1;
      gameWon = false;
      messageTimer = 0;
    }
  }
}

void mousePressed() {
  // Send data to the Arduino based on mouse click location
  if (mouseX > 25 && mouseX < 75 && mouseY > height/2 - 25 && mouseY < height/2 + 25) {
    myPort.write(2); // Green
  } else if (mouseX > 125 && mouseX < 175 && mouseY > height/2 - 25 && mouseY < height/2 + 25) {
    myPort.write(4); // Blue
  } else if (mouseX > 225 && mouseX < 275 && mouseY > height/2 - 25 && mouseY < height/2 + 25) {
    myPort.write(8); // Yellow
  } else if (mouseX > 325 && mouseX < 375 && mouseY > height/2 - 25 && mouseY < height/2 + 25) {
    myPort.write(1); // Red
  }
}

void keyPressed() {
  // Send data to the Arduino based on key pressed
  if (key == 'g' || key == 'G') {
    myPort.write(2); // Green
  } else if (key == 'b' || key == 'B') {
    myPort.write(4); // Blue
  } else if (key == 'y' || key == 'Y') {
    myPort.write(8); // Yellow
  } else if (key == 'r' || key == 'R') {
    myPort.write(1); // Red
  }
}
