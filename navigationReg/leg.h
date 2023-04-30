class leg
{
  public:
  leg *Next = NULL;
  
  virtual void Update(Navigation *navigation) {}

  virtual long GetDistance(Navigation *navigation) = 0;

  virtual int GetX() = 0;
  virtual int GetY() = 0;

  virtual bool GetAccurate() {return true;}
};
