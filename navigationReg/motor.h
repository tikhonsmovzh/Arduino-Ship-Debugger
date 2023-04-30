class Motor
{
    const int shimMotor = 5;
    const int powerMotor1 = 6;
    const int powerMotor2 = 8;

    const int sensitivity = 5;

    int workSpeed = 0;

    Servo servoForvard;
    Servo servoRear;

  public:
    void Init()
    {
      pinMode(shimMotor, OUTPUT);
      pinMode(powerMotor1, OUTPUT);
      pinMode(powerMotor2, OUTPUT);

      digitalWrite(powerMotor1, HIGH);
      digitalWrite(powerMotor2, LOW);

      analogWrite(shimMotor, 0);

      servoRear.attach(12);
      servoForvard.attach(7);

      pinMode(12, OUTPUT);
      pinMode(7, OUTPUT);

      RearRotate(0);
      ForvardRotate(0);
    }

    void SetSpeed(int speeds)
    {
      workSpeed = speeds;
      
      if (speeds == 0)
      {
        digitalWrite(shimMotor, LOW);
        return;
      }

      if(abs(speeds) == 255)
        digitalWrite(shimMotor, HIGH);
      else
        analogWrite(shimMotor, abs(speeds));

      if (speeds < 0)
      {
        digitalWrite(powerMotor1, HIGH);
        digitalWrite(powerMotor2, LOW);
      }
      else
      {
        digitalWrite(powerMotor1, LOW);
        digitalWrite(powerMotor2, HIGH);
      }
    }

    void SetAccurateSpeed(int speeds)
    {
      if (abs(speeds - workSpeed) > sensitivity) 
        SetSpeed(speeds);
    }

    int GetSpeed() {
      return workSpeed;
    }

    void RearRotate(int deegre){
      servoRotate(deegre, &servoRear);
    }

    void ForvardRotate(int deegre){
      servoRotate(deegre, &servoForvard);
    }
};
