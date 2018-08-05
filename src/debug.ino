
/* Use these debuglines to test your outputs */
    Serial.println();

    Serial.print("while(!microUp && !error): ");
    Serial.println((!microUp && !error));    
    
    Serial.print("timeSunrise = ");
    Serial.print(timeSunrise);
    Serial.print("; timeSunset = ");
    Serial.print(timeSunset);

    Serial.print("    timeMinutes: ");
    Serial.print(timeMinutes);
    
    Serial.print("    sunlight: ");
    Serial.print(sunlight);

    Serial.print("    temp: ");
    Serial.print(temp.getTemperature(), DEC);

    Serial.print("    pinMovement = ");
    Serial.print(digitalRead(pinMovement));
    Serial.print("; movementDetected = ");
    Serial.print(movementDetected);
    
    Serial.print("    pinMicroUp = ");
    Serial.print(digitalRead(pinMicroUp));
    Serial.print("; microUp = ");
    Serial.print(microUp);

    Serial.print("    pinMicroDown = ");
    Serial.print(digitalRead(pinMicroDown));
    Serial.print(" ; microDown = ");
    Serial.println(microDown);
    
    Serial.print(t.year(), DEC);
    Serial.print('/');
    Serial.print(t.month(), DEC);
    Serial.print('/');
    Serial.print(t.day(), DEC);
    Serial.print(' ');
    Serial.print(t.hour(), DEC);
    Serial.print(':');
    Serial.print(t.minute(), DEC);
    Serial.print(':');
    Serial.print(t.second(), DEC);
    Serial.println();
  
    delay(100);