class leg
{
  public:
  leg *Next = NULL;
  
  virtual void Update(Navigation *navigation) {}

  virtual long GetDistance(){return 0;}

  virtual int GetX() = 0;
  virtual int GetY() = 0;

  virtual bool GetAccurate() {return true;}

  virtual bool isComplite() = 0;
};
