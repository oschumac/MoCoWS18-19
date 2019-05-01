#include <Servo.h>

Servo servo_0;
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;

void setup() {  
  Serial.begin(74880);
  Serial.println("Start ardunio");

  // Begin serial communication on baud 74880
  Serial1.begin(74880);
  Serial.println("Start Hardware Serial");

  // Assign the five servos to different pins
  servo_0.attach(8);
  servo_1.attach(9);
  servo_2.attach(10);
  servo_3.attach(11);
  servo_4.attach(12); 

  Serial.println("Assigned servo motors");
}

void loop() {
  delay(50);
  String message = "";

  // Read all character from the serial port
  while (Serial1.available() > 0)
    message += char(Serial1.read());


  if(message != "") {
    int messageCount = 0;
    for (int i = 0; i < message.length(); i++) {
      if (message[i] == '#') 
        messageCount += 1;
    }

    // call moveServo function for each receive message
    for (int i = 0; i < messageCount; i++)
    {
      String splitMessage = split(message, '#', i);
      if (splitMessage.length() < 3)
        continue;
        
      moveServos(splitMessage);
    }
  }
}

// move the given servo to a specific position
void moveServos(String message) {  
  String topic = split(message, '|', 0);
  int value = (split(message, '|', 1)).toInt();

  if (value < 0 || 360 < value)
    return;

  if (topic.equals("0")) {    
    servo_0.write(value);
  } else if (topic.equals("1")) {
    servo_1.write(value);
  } else if (topic.equals("2")) {
    servo_2.write(value);
  } else if (topic.equals("3")) {
    servo_3.write(value);
  } else if (topic.equals("4")) {
    servo_4.write(value);
  } else if (topic.equals("5")) {
    servo_5.write(value);
  }
}

// Return the xten substring between delimiter
String split(String s, char delimiter, int index) {
  int delimiterCounter = 0;
  int start_Index = 0;
  int s_index = 0;

  while (s_index < s.length())
  {
    if (s[s_index] == delimiter) {
       delimiterCounter++;   
      if (delimiterCounter > index)
        return s.substring(start_Index, s_index);
      start_Index = s_index + 1;
    }         

    s_index = s_index + 1;
  }

  if (delimiterCounter < index)
    return "";

  return s.substring(start_Index, s_index);
}
