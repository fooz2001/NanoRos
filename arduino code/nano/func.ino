
void get_step_delay(String msg){//////data shipe $0.00l0.00r00d
  if(msg[0] == '$'){
    motor_l_interval = msg.substring(1, msg.indexOf("l")).toFloat();
    motor_r_interval = msg.substring(msg.indexOf("l") + 1, msg.indexOf("r")).toFloat();
    dir_r = msg.substring(msg.indexOf("r") + 1, msg.indexOf("d") - 1).toInt();
    dir_l = msg.substring(msg.indexOf("r") + 2, msg.indexOf("d")).toInt();
//    Serial.println(motor_l_interval);
//    Serial.println(motor_r_interval);
//    Serial.println(dir_l);
//    Serial.println(dir_r);
  }
  if(msg[0] == '#'){
    commands(msg);
  }
}

void get_speed_l(bool dir){
  
  if (dir) {
    l_dis = l_dis + one_step_distance;
  }
  else{
    l_dis = l_dis - one_step_distance;
  }
}

void get_speed_r(bool dir){
  
  if (dir) {
    r_dis = r_dis + one_step_distance;
  }
  else{
    r_dis = r_dis - one_step_distance;
  }
}

void get_pos(){
  X = (x + ((l_dis + r_dis)/2) * cos(phi));
  Y = (y + ((l_dis + r_dis)/2) * sin(phi));
  PHI = (phi + ((l_dis - r_dis)/L));

  l_dis = 0;
  r_dis = 0;
  x = X;
  y = Y;
  phi = PHI;

  Serial.print('$');
  Serial.print(X);
  Serial.print('x');
  Serial.print(Y);
  Serial.print('y');
  Serial.print(PHI*180/PI);
  Serial.println('p');
//  Serial.print(Y);
//  Serial.print(" ");
//  Serial.println(PHI*180/PI);
}

void commands(String msg){
  if(msg[1] == '0'){
    reset_pose();
  }
  if(msg[1] == 'p'){
    set_pose(msg);
  }
}

void reset_pose(){
  X = 0; x = 0;
  Y = 0; y = 0;
  PHI = 0; phi = 0;
}

void set_pose(String pose){
  X = x = pose.substring(2, pose.indexOf("x")).toFloat();
  Y = y = pose.substring(pose.indexOf("x") +1, pose.indexOf("y")).toFloat();
  PHI = phi = pose.substring(pose.indexOf("y") +1, pose.indexOf("t")).toFloat()*PI/180;
}

void get_volt(){
  volt = (float)(analogRead(batt_volt) * 5)/1023;
  
  Serial.print("%");
  Serial.println(volt);
}
