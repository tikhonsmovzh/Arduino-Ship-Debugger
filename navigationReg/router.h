class router
{
    const float lineCoef = 3;
    const float integrateCoef = 0.01;

    DynamicStructure route;

    leg *current;

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

    Vector2 buoys [quantityBuoys] {Vector2(500, 500)};

  public:  
    router(Navigation *navigation)
    {
      this->navigation = navigation;

      current = NULL;
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
      route.Enqueu(new point(146, 192));
      route.Enqueu(new circles(79, 192, 66, 0, 180));
      route.Enqueu(new point(146, 83));
      route.Enqueu(new circles(79, 83, 61, 0, -180));
      route.Enqueu(new point(146, 192));
      route.Enqueu(new circles(79, 192, 66, 0, 180));
      route.Enqueu(new point(30, 5));

      motor.SetSpeed(StartSpeed);

      current = route.Dequeue();
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

    void ReRoute(int xb, int yb, int numberBuoy)
    {
      if (numberBuoy >= quantityBuoys)
        return;

      route.CurrentToFirst();

      while (!route.IsCurrentLast())
      {
        leg* cur = route.GetCurrent();

        if (cur->GetAttachment() == numberBuoy)
        {
          cur->SetX(xb + (buoys[numberBuoy].GetX() - cur->GetX()));
          cur->SetY(yb + (buoys[numberBuoy].GetY() - cur->GetY()));
        }

        route.StepForward();
      }
    }
};
