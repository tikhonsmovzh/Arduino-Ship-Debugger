#include "motor.h"

class router
{
    const float lineCoef = 3;
    const float integrateCoef = 0.01;

    DynamicStructure route;

    point *current;

    const long maxDistances = 20;

    const long loverDistance = 200;

    Navigation *navigation;

    const byte StartSpeed = 140;
    const byte minSpeed = 80;

    bool isAccurate = false;

    Motor motor;

    int sumError = 0;

    void SetRotate(int deegre)
    {
      int rot = deegre * lineCoef + sumError * integrateCoef;

      motor.RearRotate(rot);
      motor.ForvardRotate(-rot);

      if (abs(rot) < 90)
        sumError += deegre;
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

      long legX = navigation->GetX() - current->GetX();
      long legY = navigation->GetY() - current->GetY();

      long distance = sqrt(legX * legX + legY * legY);

      if (distance < maxDistances)
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

      if (distance < loverDistance && isAccurate)
      {
        int speeds = StartSpeed * (float)distance / (float)loverDistance;

        if (speeds > minSpeed)
          motor.SetAccurateSpeed(speeds);
      }

      int deegre = navigation->GetErorr(-(atan2(navigation->GetY() - current->GetY(), navigation->GetX() - current->GetX()) * 180 / PI) - 90);

      if (motor.GetSpeed() >= 0)
        SetRotate(deegre);
      else
        SetRotate(-deegre);
    }
};
