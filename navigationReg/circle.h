class circles: public leg
{
    int x, y, radius, Start, End, alpha, rotation, potentialX, potentialY;

    const int steps = 30;
    const float ellipseCoef = 1;

  public:
    circles(int x, int y, int radius, int starts, int rot, int attachment = 0) : leg(attachment)
    {
      this->y = y;
      this->x = x;
      this->radius = radius;
      Start = starts;
      End = Start + rot;
      rotation = rot;
    }

    int GetX() {
      return potentialX;
    }
    int GetY() {
      return potentialY;
    }

    int centerX() {
      return x;
    }
    int centerY() {
      return y;
    }

    bool GetAccurate() {
      return false;
    }

    void SetX(int val) {
      x = val;
    }
    void SetY(int val) {
      y = val;
    }

    bool isComplite() {
      return !(rotation > 0 ? alpha <= End : alpha >= End);
    }

    void Update(Navigation *navigation)
    {
      alpha = atan2(navigation->GetY() - y, navigation->GetX() - x) * 180 / PI;

      if (alpha < -90)
        alpha += 360;

      alpha += rotation > 0 ? steps : -steps;

      //Serial.print("alpha = ");
      //Serial.println(alpha);
      //Serial.print("End = ");
      //Serial.println(End);

      double radAlpha = (double)alpha * PI / 180;

      potentialX = x + radius * cos(radAlpha);
      potentialY = y + radius * ellipseCoef * sin(radAlpha);
    }
};
