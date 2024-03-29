class point : public leg
{
    int x = 0, y = 0;
    long distance;
    const long maxDistances = 20;

  public:
    point(int x = 0, int y = 0, int attachment = 0) : leg(attachment)
    {
      this->y = y;
      this->x = x;
    }

    int GetX() {
      return x;
    }
    int GetY() {
      return y;
    }

    int centerX() {
      return x;
    }
    int centerY() {
      return y;
    }

    void SetX(int val) {
      x = val;
    }
    void SetY(int val) {
      y = val;
    }

    long GetDistance() {
      return distance;
    }

    bool isComplite()
    {
      return distance < maxDistances;
    }

    void Update(Navigation *navigation)
    {
      long legX = navigation->GetX() - x;
      long legY = navigation->GetY() - y;

      distance = sqrt(legX * legX + legY * legY);
    }
};
