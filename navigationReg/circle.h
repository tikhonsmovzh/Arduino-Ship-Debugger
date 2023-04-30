class circle: public leg
{
    int x, y, radius, Start, End, alpha, rotation, potentialX, potentialY;

    const int steps = 15;

  public:
    circle(int x, int y, int radius, int starts, int rot)
    {
      this->y = y;
      this->x = x;
      this->radius = radius;
      Start = starts;
      End = Start + rot;
      rotation = rot;
    }

    int GetX() {return potentialX;}

    int GetY() {return potentialY;}

    bool GetAccurate() {return false;}

    bool isComplite(){return !(rotation > 0 ? alpha <= End : alpha >= Start);}

    void Update(Navigation *navigation)
    {
      alpha = (atan2(navigation->GetY() - y, navigation->GetX() - x) * 180 / PI) + 180;

      alpha += rotation > 0 ? steps : -steps;

      double radAlpha = (double)alpha * PI / 180;

      potentialX = x + radius * cos(radAlpha);
      potentialY = y + radius * sin(radAlpha);
    }
};
