class router
{
    const float lineCoef = 1;
    const float integrateCoef = 0;

    DynamicStructure route;

    leg *targetLeg;

    const long loverDistance = 200;

    Navigation *navigation;

    const byte StartSpeed = 100;
    const byte minSpeed = 80;

    bool isAccurate = false;

    Motor motor;

    int sumError = 0;

    void SetRotate(int deegre)
    {
      motor.RearRotate(deegre);
      motor.ForvardRotate(-deegre);
    }

    Vector2 buoys [quantityBuoys] {Vector2(79, 192)};

  public:
    router(Navigation *navigation)
    {
      this->navigation = navigation;

      targetLeg = NULL;
    }

    void Init()
    {
      motor.Init();
    }

    void Start()
    {
      route.Clear();

      navigation->CompossGyro();

      // 1 задание
      //route.Enqueu(new point(85, 278));
      //route.Enqueu(new point(85, 0));

      // 2 задание
      //isAccurate = true;
      //route.Enqueu(new point(10, 250));
      //route.Enqueu(new point(135, 250));
      //route.Enqueu(new point(150, 35));
      //route.Enqueu(new point(10, 20));

      // 3 задание
      //route.Enqueu(new point(146, 162));
      //route.Enqueu(new circles(79, 162, 66, 0, 180));
      //route.Enqueu(new point(30, 5));

      // 4 задание
      //route.Enqueu(new point(146, 192));
      //route.Enqueu(new circles(79, 192, 66, 0, 180));
      //route.Enqueu(new point(146, 83));
      //route.Enqueu(new circles(79, 83, 61, 0, -180));
      //route.Enqueu(new point(146, 192));
      //route.Enqueu(new circles(79, 192, 66, 0, 180));
      //route.Enqueu(new point(30, 5));

      // 3 задание с учётом
      route.Enqueu(new point(30, 200, 1));
      route.Enqueu(new circles(96, 200, 50, 150, -120, 1));
      route.Enqueu(new point(146, 5));

      motor.SetSpeed(StartSpeed);

      targetLeg = route.Dequeue();
    }

    void Update()
    {
      if (targetLeg == NULL)
      {
        targetLeg = route.Dequeue();

        if (targetLeg != NULL)
          motor.SetSpeed(StartSpeed);

        return;
      }

      targetLeg->Update(navigation);

      if (targetLeg->isComplite())
      {
        targetLeg = route.Dequeue();

        digitalWrite(buzzer, HIGH);

        sumError = 0;

        if (targetLeg == NULL)
        {
          motor.SetSpeed(0);

          return;
        }

        motor.SetSpeed(StartSpeed);
      }

      if (targetLeg->GetDistance() < loverDistance && isAccurate && targetLeg->GetAccurate())
      {
        int speeds = StartSpeed * (float)targetLeg->GetDistance() / (float)loverDistance;

        if (speeds > minSpeed)
          motor.SetAccurateSpeed(speeds);
      }

      int deegre = navigation->GetErorr(-(atan2(navigation->GetY() - targetLeg->GetY(), navigation->GetX() - targetLeg->GetX()) * 180 / PI) - 90);
      int rot = deegre * lineCoef + sumError * integrateCoef;

      if (motor.GetSpeed() >= 0)
        SetRotate(rot);
      else
        SetRotate(-rot);

      if (abs(rot) < 90)
        sumError += deegre;
    }

    void ReRoute(int xb, int yb, int numberBuoy)
    { 
      route.CurrentToFirst();

      while (!route.IsCurrentLast())
      {
        leg* cur = route.GetCurrent();

        if (cur->GetAttachment() == numberBuoy)
          changeLeg(xb, yb, numberBuoy, cur);

        route.StepForward();
      }

      if (targetLeg == NULL)
        return;

      if (targetLeg->GetAttachment() == numberBuoy)
        changeLeg(xb, yb, numberBuoy, targetLeg);
    }

    void changeLeg(int xb, int yb, int numberBuoy, leg* cur)
    {
      cur->SetX(cur->centerX() - buoys[numberBuoy - 1].GetX() + xb);
      cur->SetY(cur->centerY() - buoys[numberBuoy - 1].GetY() + yb);
    }
};
