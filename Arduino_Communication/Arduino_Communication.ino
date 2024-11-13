void setup() {
  Serial.begin(9600); // เริ่มต้น Serial Communication
}

void loop() {
  int sensorValue = analogRead(A0);               // อ่านค่าเซนเซอร์
  if (!isnan(sensorValue)) {                      // ตรวจสอบว่าค่าไม่ใช่ NaN
    float voltage = sensorValue * (5.0 / 1023.0); // แปลงค่า Analog เป็น Voltage
    float temperatureC = (voltage - 0.5) * 100;   // แปลง Voltage เป็น อุณหภูมิ (Celsius)
    Serial.println(temperatureC, 2);              // ส่งค่าผ่าน Serial
  }
  delay(1000);                                      // หน่วงเวลา 2 วินาที
}