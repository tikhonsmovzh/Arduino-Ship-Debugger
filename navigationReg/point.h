class point : public leg
{
    int x = 0, y = 0;

  public:
    point(int x = 0, int y = 0)
    {
      this->y = y;
      this->x = x;
    }

    int GetX(){return x;}

    int GetY(){return y;}

    long GetDistance(Navigation *navigation)
    {
      long legX = navigation->GetX() - x;
      long legY = navigation->GetY() - y;

      return sqrt(legX * legX + legY * legY);
    }
};
