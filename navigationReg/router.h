class router
{
    const float lineCoef = 3;
    const float integrateCoef = 0.01;

    DynamicStructure route;

    leg *current;

    const long loverDistance = 200;

    Navigation *navigation;

    const byte StartSpeed = 140;
    const byte minSpeed = 80;

    bool isAccurate = false;

    Motor motor;

    int sumError = 0;

    void SetRotate(int deegre)
    {
      motor.RearRotate(deegre);
      motor.ForvardRotate(-deegre);
    }

  public:
    const int buzzer = 13;

    router(Navigation *navigation)
    {
      this->navigation = navigation;

      current = NULL;
    }

    void Init()
    {
      motor.Init();

      pinMode(buzzer, OUTPUT);
    }

    void Start()
    {
      route.Clear();

      navigation->CompossGyro();

      isAccurate = true;
      route.Enqueu(new point(10, 250));
      route.Enqueu(new point(135, 250));
      route.Enqueu(new point(150, 35));
      route.Enqueu(new point(10, 20));

      //route.Enqueu(new point(85, 278));
      //route.Enqueu(new point(85, 0));

      motor.SetSpeed(StartSpeed);

      current = route.Dequeue();
    }

    void End()
    {
      motor.SetSpeed(0);
    }

    void Update()
    {
      if (current == NULL)
      {
        current = route.Dequeue();

        if (current != NULL)
          motor.SetSpeed(StartSpeed);

        return;
      }

      current->Update(navigation);
      
      if (current->isComplite())
      {
        current = route.Dequeue();

        digitalWrite(buzzer, HIGH);

        sumError = 0;

        if (current == NULL)
        {
          motor.SetSpeed(0);

          return;
        }

        motor.SetSpeed(StartSpeed);
      }

      if (current->GetDistance() < loverDistance && isAccurate && current->GetAccurate())
      {
        int speeds = StartSpeed * (float)current->GetDistance() / (float)loverDistance;

        if (speeds > minSpeed)
          motor.SetAccurateSpeed(speeds);
      }

      int deegre = navigation->GetErorr(-(atan2(navigation->GetY() - current->GetY(), navigation->GetX() - current->GetX()) * 180 / PI) - 90);
      int rot = deegre * lineCoef + sumError * integrateCoef;

      if (motor.GetSpeed() >= 0)
        SetRotate(rot);
      else
        SetRotate(-rot);

      if (abs(rot) < 90)
        sumError += deegre;
    }
};
