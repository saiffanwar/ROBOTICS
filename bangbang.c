  l_status = line_left.onLine(l_value - l_cal, 200);
  c_status = line_centre.onLine(c_value - c_cal, 200);
  r_status = line_right.onLine(r_value - r_cal, 200);

  if (c_status == 0) {
    move(30, 30);
    Serial.print("moved");
    Serial.print( "\n" );
    delay(50);
    move(0,0);
  }

  else if (r_status == 0) {
    move(-30, 30);
    Serial.print("moved");
    Serial.print( "\n" );
    delay(50);
    move(0,0);
  }

  else if (l_status == 0) {
    move(30, -30);
    Serial.print("moved");
    Serial.print( "\n" );
    delay(50);
    move(0,0);
  }

