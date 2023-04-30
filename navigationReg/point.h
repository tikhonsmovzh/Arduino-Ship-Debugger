class point
{
    int x = 0, y = 0;

  public:
    point *Next;

    point(int x = 0, int y = 0)
    {
      this->y = y;
      this->x = x;

      Next = NULL;
    }

    int GetX()
    {
      return x;
    }

    int GetY()
    {
      return y;
    }
};
