#include "include.h"

const byte SensorP = 198;
const byte ZeroingCompassP = 1;
const byte saveP = 2;
const byte ZeroingGyroP = 3;
const byte distFixationP = 4;

const int timeDist = 50;
const int timeGyro = 20;

const int key = 4;

unsigned long tim = 0, gyroTim = 0, currentMil = 0;

Navigation navigation;

router Router(&navigation);

void RouterReRoute(int xb, int yb, int numberBuoy) {
  Router.ReRoute(xb, yb, numberBuoy);
}

void setup() {
  Serial.begin(9600);

  Wire.begin();

  pinMode(key, INPUT);
  pinMode(buzzer, OUTPUT);

  Router.Init();
  navigation.Init();

  navigation.ReRoute = RouterReRoute;
}

void loop() {
  currentMil = millis();

  if (gyroTim < currentMil)
  {
    gyroTim = currentMil + timeGyro;

    navigation.GyroUpdate();

    currentMil = millis();
  }

  if (tim < currentMil)
  {
    tim = currentMil + timeDist;

    digitalWrite(buzzer, LOW);

    navigation.UpdateDist();

    if (isEnable)
      Router.Update();
  }

  if (digitalRead(key) && !isEnable)
  {
    isEnable = true;

    Router.Start();
    navigation.Start();
  }

  if (Serial.available() > 0)
  {
    byte buf[1];

    Serial.readBytes(buf, 1);

    switch (buf[0])
    {
      case SensorP:
        {
          for (int i = 0; i < 6; i++)
            Serial.write(navigation.buf[i]);

          WriteInt(navigation.GetCompass());
          WriteInt(navigation.GetDistance1());
          WriteInt(navigation.GetDistance2());
          WriteInt(navigation.GetX());
          WriteInt(navigation.GetY());
          WriteInt(navigation.GetGyroSpeed());
          WriteInt(navigation.GetGyro());

          break;
        }

      case saveP:
        {
          byte bufer[8];
          Serial.readBytes(bufer, 8);
          EEPROM.put(navigation.SaveBaseData, bufer);

          navigation.SaveCompass();

          break;
        }

      case ZeroingCompassP:
        navigation.ZeroingCompass();
        break;

      case ZeroingGyroP:
        navigation.ZeroingGyro();
        break;

      case distFixationP:
        navigation.Fixation();
        break;
    }
  }
}
