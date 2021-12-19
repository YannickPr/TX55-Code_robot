#include <Arduino.h>
#include <ArduinoJson.h>
#include <Stepper.h>
#include <AccelStepper.h>
#include <MultiStepper.h> 

#include "pinout.h"
#include "queue.h"

AccelStepper motorRotate = AccelStepper(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
const float STEP_PER_DEGREE_ROTATION = 32. / 9. * 200 * 16 / 360.;    //big gear: 32, small gear: 9, steps per rev: 200, microsteps: 16

AccelStepper motorLower(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
const float STEP_PER_DEGREE_LOWER = 32. / 9. * 200 * 16 / 360.;

AccelStepper motorHigher(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
const float STEP_PER_DEGREE_HIGHER = 32. / 9. * 200 * 16 / 360.;

AccelStepper motorGripper(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);

MultiStepper allMotors;
long positionMotors[4] = {0}; //one per motor

Queue<JsonObject> queueJson(15);
String message;


bool receiveJson(){
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == ' '|| c == '\t') {
       return false; 
    }
    if (c == '\r') {
      Serial.print("reception de ");
      Serial.println(message);
      //bool b = processMessage(message);
      StaticJsonDocument<1000> messageJson;

      DeserializationError error = deserializeJson(messageJson, message);

      message = "";
      // Test if parsing succeeds.
      if (error) {
        //JsonObject jsonReponse;
        //jsonReponse["Statut"] = true;
        //jsonReponse["Error"] = error.f_str();
        //Serial.println(jsonReponse);
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
      }
      queueJson.push(messageJson.to<JsonObject>());

      return true;
    } else {
       message += c; 
    }
  }  
  return false;
}


void executJson(JsonObject instruction)
{
  if(instruction.containsKey("SpeedMax")){
    int speed = 2000;//instruction["SpeedMax"].as<int>();
    Serial.print("detection de speed : ");
    Serial.println(speed);
    
    motorRotate.setMaxSpeed(speed);
    motorLower.setMaxSpeed(speed);
    motorHigher.setMaxSpeed(speed);
  }
  //take it in account 

  instruction.containsKey("Acceleration");
  //take it in account 
  
  if (instruction.containsKey("Position"))
  {
    if (instruction["Position"].containsKey("Rz"))
    {
      positionMotors[0] = instruction["Position"]["Rz"].as<int>()/10.*STEP_PER_DEGREE_ROTATION;
    }
    if (instruction["Position"].containsKey("Ry1"))
    {
      positionMotors[1] = instruction["Position"]["Ry1"].as<int>() /10. * STEP_PER_DEGREE_LOWER;
    }
    if (instruction["Position"].containsKey("Ry2"))
    {
      positionMotors[2] = instruction["Position"]["Ry2"].as<int>() /10. * STEP_PER_DEGREE_HIGHER;
    }
    if (instruction["Position"].containsKey("Gripper"))
    {
      positionMotors[3] = instruction["Position"]["Gripper"].as<int>() /10.;// * STEP_PER_DEGREE_HIGHER;
    }
    Serial.print("lecture de Rz : ");
    Serial.println(positionMotors[0]);

    allMotors.moveTo(positionMotors);
  }
}


void setup() {
  Serial.begin(9600);		//initialisation
  Serial.println("initialisation ok");

  motorRotate.setMaxSpeed(500);
  motorLower.setMaxSpeed(500);
  motorHigher.setMaxSpeed(500);
  motorGripper.setMaxSpeed(500);

  motorRotate.setAcceleration(50);
  motorLower.setAcceleration(50);
  motorHigher.setAcceleration(50);
  motorGripper.setAcceleration(50);

  motorRotate.setCurrentPosition(0);
  motorLower.setCurrentPosition(45 * STEP_PER_DEGREE_LOWER); //change this angle
  motorHigher.setCurrentPosition(5 * STEP_PER_DEGREE_HIGHER); //change this angle
  motorGripper.setCurrentPosition(0); //change this angle
  
  allMotors.addStepper(motorRotate);
  allMotors.addStepper(motorLower);
  allMotors.addStepper(motorHigher);
  allMotors.addStepper(motorGripper);

  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);

}

void loop() {
  if (!queueJson.isFull()) {            //if the queue is not full
    if (receiveJson()) {    //get one more caracter from serial : is it end of line ?
      Serial.println("ok"); 
    }
  }

  if ((!allMotors.run()) && (!queueJson.isEmpty())) {  //run the motors. If it's finish and the queue is not empty, go the the next step
    Serial.println("lecture du json suivant : "); 
    executJson(queueJson.pop());  //loar the next element in the queue
  }
  
}
