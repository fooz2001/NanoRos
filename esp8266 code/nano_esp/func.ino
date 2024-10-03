
double tr,tl;

void vel(float vl, float vr){

  if((vr < 0) && (vl < 0)){
    tr = O/(-1*vr*2048);
    tl = O/(-1*vl*2048);
    
    Serial.print('$');
    Serial.print(tl*1000);
    Serial.print("l");
    Serial.print(tr*1000);
    Serial.print("r");
    Serial.print("0");
    Serial.print("0");
    Serial.println("d");
  }
  else if((vr > 0) && (vl > 0)){
    tr = O/(vr*2048);
    tl = O/(vl*2048);
    
    Serial.print('$');
    Serial.print(tl*1000);
    Serial.print("l");
    Serial.print(tr*1000);
    Serial.print("r");
    Serial.print("1");
    Serial.print("1");
    Serial.println("d");
  }
  else if((vr > 0) && (vl < 0)){
    tr = O/(vr*2048);
    tl = O/(-1*vl*2048);
    
    Serial.print('$');
    Serial.print(tl*1000);
    Serial.print("l");
    Serial.print(tr*1000);
    Serial.print("r");
    Serial.print("1");
    Serial.print("0");
    Serial.println("d");
  }
  else if((vl == 0) || (vr == 0)){
    
    Serial.print('$');
    Serial.print("0.00");
    Serial.print("l");
    Serial.print("0.00");
    Serial.print("r");
    Serial.print("0");
    Serial.print("0");
    Serial.println("d");
  }
  else {
    tr = O/(-1*vr*2048);
    tl = O/(vl*2048);
    
    Serial.print('$');
    Serial.print(tl*1000);
    Serial.print("l");
    Serial.print(tr*1000);
    Serial.print("r");
    Serial.print("0");
    Serial.print("1");
    Serial.println("d");
  }
}

void get_pose(String msg){//data shape $000x000y000p
  if(msg[0] == '$'){
    X = msg.substring(1, msg.indexOf("x")).toDouble();
    Y = msg.substring(msg.indexOf("x") + 1, msg.indexOf("y")).toDouble();
    PHI = msg.substring(msg.indexOf("y") + 1, msg.indexOf("p")).toDouble();
//    Serial.print(X);
//    Serial.print(Y);
//    Serial.println(PHI);
  
    pos.x = X;
    pos.y = Y;
    pos.theta = PHI;
    pose.publish( &pos );
  }
}

void get_volt(String msg){//data shape %4.20
  if(msg[0] == '%'){
    batt.data = msg.substring(msg.indexOf("%") + 1, msg[5]).toFloat();
    volt.publish( &batt );
  }
}
