//Written By M.fayez al masri

#define STEPPER_R_PIN_1 2
#define STEPPER_R_PIN_2 3
#define STEPPER_R_PIN_3 4
#define STEPPER_R_PIN_4 5

#define STEPPER_L_PIN_1 8
#define STEPPER_L_PIN_2 9
#define STEPPER_L_PIN_3 10
#define STEPPER_L_PIN_4 11

#define batt_volt A0

float volt;

const float one_step_distance = 13.15/2048;  //one step distance cm
const float L = 7.26;

float X;
float Y;
float PHI;

float x = 5;
float y = 5;
float phi = 0 * PI/180;

float l_dis;
float r_dis;

unsigned long current_motor1_time;
unsigned long current_motor2_time;

unsigned long current_volt_time;

unsigned long previous_motor1_time = millis();
unsigned long previous_motor2_time = millis();

unsigned long previous_volt_time = millis();


//unsigned long start_time1 = millis();
//unsigned long start_time2 = millis();
//
//unsigned long end_time1;
//unsigned long end_time2;
//
//unsigned long delta_time1;
//unsigned long delta_time2;

float volt_time = 1000;

float vel_l;

float motor_r_interval = 0;
float motor_l_interval = 0;

String data;
char c;


bool dir_r,dir_l;

void setup() {
  
  pinMode(STEPPER_R_PIN_1, OUTPUT);
  pinMode(STEPPER_R_PIN_2, OUTPUT);
  pinMode(STEPPER_R_PIN_3, OUTPUT);
  pinMode(STEPPER_R_PIN_4, OUTPUT);
  
  pinMode(STEPPER_L_PIN_1, OUTPUT);
  pinMode(STEPPER_L_PIN_2, OUTPUT);
  pinMode(STEPPER_L_PIN_3, OUTPUT);
  pinMode(STEPPER_L_PIN_4, OUTPUT);

  pinMode(batt_volt, INPUT);

  Serial.begin(115200);
}

void loop() {

  while (Serial.available() > 0)
    {
      c=Serial.read();
      if(c=='\n'){break;}
      else{data+=c;}
    }
    if(c=='\n'){
      //Serial.println(data);
      get_step_delay(data);
      c=0;
      data="";
    }
    
  current_motor1_time = millis();
  current_motor2_time = millis();
  current_volt_time = millis();

  if((current_motor1_time - previous_motor1_time > motor_l_interval) && (motor_l_interval != 0)){
//    end_time1 = millis();
//    delta_time1 = end_time1 - start_time1;
//    start_time1 = 0; end_time1 = 0;
    one_step_l(dir_l);       /////steps
//    start_time1 = millis();
    
    get_speed_l(dir_l);
    
    previous_motor1_time = current_motor1_time;
  }

  if((current_motor2_time - previous_motor2_time > motor_r_interval) && (motor_r_interval != 0)){
    one_step_r(dir_r);
    get_speed_r(dir_r);
    
    previous_motor2_time = current_motor2_time;
  }

  if(current_volt_time - previous_volt_time > volt_time){
    get_volt();
    
    previous_volt_time = current_volt_time;
  }
  
  get_pos();
}
