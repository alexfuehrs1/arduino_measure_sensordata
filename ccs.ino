
CCS811Data getCCS811Data() {
  CCS811Data data;
  data.valid = false;
  if(ccs.available()){
    if(!ccs.readData()){
      data.eco2 = ccs.geteCO2();
      data.tvoc = ccs.getTVOC();
      Serial.print("CO2: "); Serial.print(data.eco2); Serial.print(" ppm, ");
      Serial.print("TVOC: "); Serial.print(data.tvoc); Serial.println(" ppb");
      data.valid = true;
    } else {
      Serial.println("Error reading CCS811");
    }
  }
  return data;
}
