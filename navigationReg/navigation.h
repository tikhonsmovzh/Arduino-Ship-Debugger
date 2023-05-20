class Navigation
{
    sector* deegres[4] {new sector0(0, yPlus, xPlus), new sector(90, xPlus, yMinus), new sector(180, yMinus, xMinus), new sector(270, xMinus, yPlus)};

    int preferred, maxX, compass, minX, maxY, minY, centerX, centerY, x, y, h, l, gyro, gyroSpeed, distance1, distance2,
        pool = 0, errorX = 0, errorY = 0, previosX = 0, previosY = 0, counterBuoys = 0;

    unsigned long previosIgnoreTime = 0, currentTime = 0;

    double rot = 0;

    bool isSeeBuoyX = false, isSeeBuoyY = false;

    unsigned long int timer = 0;

    const int SizeX = 26;
    const int SizeY = 59;

    const int addr = 0x1E;

    const int Addr1 = 0x11;
    const int Addr2 = TFL_DEF_ADR;

    const int sensitivity = 60;

    const int GyroError = 379;

    const int ignoreTime = 2;

    TFLI2C tflI2C;

    MPU6050 accgyro;

    Servo servo;

    int GetValue()
    {
      Wire.beginTransmission(addr);
      Wire.write(0x03);
      Wire.endTransmission();

      Wire.requestFrom(addr, 6);

      for (int i = 0; i < 6; i++)
        buf[i] = Wire.read();

      int xC = toIntB(buf[0], buf[1]) - centerX;
      int yC = toIntB(buf[4], buf[5]) - centerY;

      int g = (int)(atan2(yC, xC) * 180 / PI) + 180 - pool;

      if (g < 0)
        g += 360;

      return g;
    }

    void InitCompass()
    {
      EEPROM.get(SaveBaseData, maxX);
      EEPROM.get(SaveBaseData + 2, minX);
      EEPROM.get(SaveBaseData + 4, maxY);
      EEPROM.get(SaveBaseData + 6, minY);

      centerX = (maxX + minX) / 2;
      centerY = (maxY + minY) / 2;
    }

  public:
    const int SaveBaseData = 0;

    void (* ReRoute)(int xb, int yb, int numberBuoy);

    byte buf[6];

    void Init()
    {
      Wire.beginTransmission(addr);
      Wire.write(0x00);
      Wire.write(0x70);
      Wire.write(0xA0);
      Wire.write(0x00);
      Wire.endTransmission();

      EEPROM.get(SaveBaseData + 8, pool);

      InitCompass();

      accgyro.initialize();

      servo.attach(10);

      pinMode(10, OUTPUT);

      EEPROM.get(SaveBaseData + 10, l);
      EEPROM.get(SaveBaseData + 12, h);

      tflI2C.getData(distance1, Addr2);
      tflI2C.getData(distance2, Addr1);

      previosX = deegres[preferred]->GetX(distance1, distance2, l, h);
      previosY = deegres[preferred]->GetY(distance1, distance2, l, h);
    }

    void SaveCompass()
    {
      InitCompass();
    }

    void ZeroingCompass()
    {
      pool = (compass + pool) % 360;

      EEPROM.put(SaveBaseData + 8, pool);
    }

    void ZeroingGyro()
    {
      gyro = 0;
      rot = 0;
    }

    void UpdateDist()
    {
      tflI2C.getData(distance1, Addr2);
      tflI2C.getData(distance2, Addr1);

      compass = GetValue();

      if (currentTime >= previosIgnoreTime)
      {
        x = deegres[preferred]->GetX(distance1, distance2, l, h);
        y = deegres[preferred]->GetY(distance1, distance2, l, h);

        if (isEnable)
        {
          int differenceX = x - previosX, differenceY = y - previosY;

          if (abs(differenceX) > sensitivity)
          {
            if (isSeeBuoyX)
            {
              errorX = 0;
              isSeeBuoyX = false;
            }
            else
            {
              errorX = differenceX;
              isSeeBuoyX = true;

              if (counterBuoys < quantityBuoys)
              {
                digitalWrite(buzzer, HIGH);

                counterBuoys++;

                if (ReRoute != NULL) ReRoute(h - previosX, y, counterBuoys);
              }
            }
            
          }

          if (abs(differenceY) > sensitivity)
          {
            if (isSeeBuoyY)
            {
              errorY = 0;
              isSeeBuoyY = false;
            }
            else
            {
              errorY = differenceY;
              isSeeBuoyY = true;
            }
          }
        }

        previosX = x;
        previosY = y;

        x += errorX;
        y += errorY;
      }

      currentTime++;

      if (!deegres[preferred]->isIncludedEx(gyro))
      {
        previosIgnoreTime = currentTime + ignoreTime;

        for (int i = 0; i < 4; i++)
        {
          if (deegres[i]->isIncluded(gyro))
          {
            preferred = i;

            break;
          }
        }
      }

      servoRotate(GetErorr(deegres[preferred]->GetDeegre()), &servo);
    }

    void GyroUpdate()
    {
      gyroSpeed = -(accgyro.getRotationZ() + GyroError) / 131.0;

      unsigned long int nowMic = micros();

      rot += (double)gyroSpeed * ((double)(nowMic - timer) / 1000000);

      timer = nowMic;

      if (rot < 0)
        rot += 360;

      if (rot >= 360)
        rot = 0;

      gyro = (int)rot;
    }

    void Fixation()
    {
      l = distance1;
      h = distance2;

      EEPROM.put(SaveBaseData + 10, l);
      EEPROM.put(SaveBaseData + 12, h);
    }

    float GetErorr(int alpha)
    {
      if (alpha < 0)
        alpha += 360;

      alpha %= 360;

      int e = gyro - alpha;

      int s = sign(e);

      int absE = abs(e);

      if (absE > 180)
        e = (360 - absE) * s * -1;

      return e;
    }

    void CompossGyro()
    {
      rot = compass;
      gyro = compass;
    }

    int GetX() {
      return x;
    }
    int GetY() {
      return y;
    }
    int GetCompass() {
      return compass;
    }
    int GetGyro() {
      return gyro;
    }
    int GetGyroSpeed() {
      return gyroSpeed;
    }
    int GetDistance1() {
      return distance1;
    }
    int GetDistance2() {
      return distance2;
    }
};
