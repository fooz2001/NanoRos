int step_number_l = 0;

void one_step_l(bool dir){
    if(dir){
    switch(step_number_l){
      case 0:
        PORTD = PORTD & B11100011;
        PORTD = PORTD | B00100000;
        break;
      case 1:
        PORTD = PORTD & B11110011;
        PORTD = PORTD | B00010000;
        PORTD = PORTD & B11011111;
        break;
      case 2:
        PORTD = PORTD & B11111011;
        PORTD = PORTD | B00001000;
        PORTD = PORTD & B11001111;
        break;
      case 3:
        PORTD = PORTD | B00000100;
        PORTD = PORTD & B11000111;
        break; 
  } 
}
else{
    switch(step_number_l){
      case 0:
        PORTD = PORTD | B00000100;
        PORTD = PORTD & B11000111;
        break;
      case 1:
        PORTD = PORTD & B11111011;
        PORTD = PORTD | B00001000;
        PORTD = PORTD & B11001111;
        break;
      case 2:
        PORTD = PORTD & B11110011;
        PORTD = PORTD | B00010000;
        PORTD = PORTD & B11011111;
        break;
      case 3:
        PORTD = PORTD & B11100011;
        PORTD = PORTD | B00100000;
        break; 
  }
}
  step_number_l++;
  if(step_number_l > 3){
    step_number_l = 0;
    }
}

int step_number_r = 0;

void one_step_r(bool dir){
    if(dir){
    switch(step_number_r){
      case 0:
        PORTB = PORTB | B00000001;
        PORTB = PORTB & B11110001;
        break;
      case 1:
        PORTB = PORTB & B11111110;
        PORTB = PORTB | B00000010;
        PORTB = PORTB & B11110011;
        break;
      case 2:
        PORTB = PORTB & B11111100;
        PORTB = PORTB | B00000100;
        PORTB = PORTB & B11110111;
        break;
      case 3:
        PORTB = PORTB & B11111000;
        PORTB = PORTB | B00001000;
        break; 
  } 
}
else{
    switch(step_number_r){
      case 0:
        PORTB = PORTB & B11111000;
        PORTB = PORTB | B00001000;
        break;
      case 1:
        PORTB = PORTB & B11111100;
        PORTB = PORTB | B00000100;
        PORTB = PORTB & B11110111;
        break;
      case 2:
        PORTB = PORTB & B11111110;
        PORTB = PORTB | B00000010;
        PORTB = PORTB & B11110011;
        break;
      case 3:
        PORTB = PORTB | B00000001;
        PORTB = PORTB & B11110001;
        break;
  }
}
  step_number_r++;
  if(step_number_r > 3){
    step_number_r = 0;
    }
}
