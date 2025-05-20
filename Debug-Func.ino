void sensorDebug(){
    // Update sensor readings
    
    // Print raw sensor values
    Serial.print("Raw - Front: ");
    Serial.print(rawFront);
    Serial.print(" Left: ");
    Serial.print(rawLeft);
    Serial.print(" Right: ");
    Serial.println(rawRight);
    
    // Print filtered sensor values
    Serial.print("Filtered - Front: ");
    Serial.print(filteredFront);
    Serial.print(" Left: ");
    Serial.print(filteredLeft);
    Serial.print(" Right: ");
    Serial.println(filteredRight);
    
    // Print wall detection status
    Serial.print("Walls - Front: ");
    Serial.print(rawFront < wallThresholdFront ? "Yes" : "No");
    Serial.print(" Left: ");
    Serial.print(rawLeft < wallThresholdLeft ? "Yes" : "No");
    Serial.print(" Right: ");
    Serial.println(rawRight < wallThresholdRight ? "Yes" : "No");
    
    Serial.println("-------------------");
}

void rawSensorDebug(){
    // Update sensor readings
    updateRawSensors();
    stop();
    // Print raw sensor values

    Serial.print(rawFront);
    Serial.print(" ");
    Serial.print(rawLeft);
    Serial.print(" ");
    Serial.println(rawRight);
    delay(10);
}
