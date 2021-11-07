#include <Arduino.h>
#include <ArduinoJson.h>
#include <Stepper.h>
#include <AccelStepper.h>
#include <MultiStepper.h> 

#include "pinout.h"
#include "queue.h"

AccelStepper motorRotate = AccelStepper(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
const float STEP_PER_DEGREE_ROTATION = 123;

AccelStepper motorLower(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN);
const float STEP_PER_DEGREE_LOWER = 456;

AccelStepper motorHigher(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN);
const float STEP_PER_DEGREE_HIGHER = 789;

AccelStepper motorGripper(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN, E_ENABLE_PIN);

MultiStepper allMotors;
long positionMotors[4] = {0}; //one per motor

Queue<JsonObject> queueJson(15);
String message;


bool receiveJson(){
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
       return false; 
    }
    if (c == '\r') {
      //bool b = processMessage(message);
      StaticJsonDocument<1000> messageJson;

      DeserializationError error = deserializeJson(messageJson, message);

      // Test if parsing succeeds.
      if (error) {
        JsonObject jsonReponse;
        jsonReponse["Statut"] = true;
        jsonReponse["Error"] = error.f_str();
        Serial.println(jsonReponse);
        //Serial.print(F("deserializeJson() failed: "));
        //Serial.println(error.f_str());
        return false;
      }
      queueJson.push(messageJson.to<JsonObject>());

      message = "";
      return true;
    } else {
       message += c; 
    }
  }  
  return false;
}


void executJson(JsonObject instruction)
{
  instruction.containsKey("SpeedMax");
  //take it in account 

  instruction.containsKey("Acceleration");
  //take it in account 
  
  if (instruction.containsKey("Position"))
  {
    if (instruction.containsKey("Rz"))
    {
      positionMotors[0] = (float)(instruction["position"]["Rz"])*STEP_PER_DEGREE_ROTATION;
    }
    if (instruction.containsKey("Ry1"))
    {
      positionMotors[1] = (float)(instruction["position"]["Ry1"])*STEP_PER_DEGREE_LOWER;
    }
    if (instruction.containsKey("Ry2"))
    {
      positionMotors[2] = (float)(instruction["position"]["Ry2"])*STEP_PER_DEGREE_HIGHER;
    }
    if (instruction.containsKey("Gripper"))
    {
      positionMotors[3] = (float)(instruction["position"]["Gripper"])*STEP_PER_DEGREE_HIGHER;
    }
    
    allMotors.moveTo(positionMotors);
  }
}


void setup() {
  Serial.begin(9600);		//initialisation
  delay(3000);
  Serial.println("initialisation ok");

  motorRotate.setMaxSpeed(200);
  motorLower.setMaxSpeed(200);
  motorHigher.setMaxSpeed(200);
  motorGripper.setMaxSpeed(200);

  motorRotate.setAcceleration(30);
  motorLower.setAcceleration(30);
  motorHigher.setAcceleration(30);
  motorGripper.setAcceleration(30);

  motorRotate.setCurrentPosition(0);
  motorLower.setCurrentPosition(0); //change this angle
  motorHigher.setCurrentPosition(0); //change this angle
  motorGripper.setCurrentPosition(0); //change this angle
  
  allMotors.addStepper(motorRotate);
  allMotors.addStepper(motorLower);
  allMotors.addStepper(motorHigher);
  allMotors.addStepper(motorGripper);

}

void loop() {
  Serial.println("loop");
  if (!queueJson.isFull()) {            //if the queue is not full
    if (receiveJson()) {    //get one more caracter from serial : is it end of line ?
      Serial.println("ok"); 
    }
  }
  if ((!allMotors.run()) && (!queueJson.isEmpty())) {  //run the motors. If it's finish and the queue is not empty, go the the next step
    executJson(queueJson.pop());  //loar the next element in the queue
  }



/*
  String reception = "";
  while (!Serial.available());
  while (Serial.available()) { 	//tant que des données sont en attente
    char c = Serial.read(); 	// lecture
    reception += String(c); 	//on ajoute à l'objet réception
    //delay(10); 		//petite attente
  }
  reception.trim(); 	//on enlève le superflu en début et fin de chaîne

  JsonObject jsonRecu = JsonObject.parse(reception);
  

  JsonObject jsonReponse;
  double val1 = double(jsonRecu["Rz"]);
  if(val1 == 10){
    //digitalWrite(D13, HIGH);
  }
  else{
    //digitalWrite(D13, LOW);
  }*/

  
}
