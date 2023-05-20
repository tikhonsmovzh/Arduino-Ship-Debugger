class leg
{
    int attachment;

  public:
    leg *Next = NULL;

    leg(int attachment) {
      this->attachment = attachment;
    }

    virtual void Update(Navigation *navigation) {}

    virtual long GetDistance() {
      return 0;
    }

    virtual int GetX() = 0;
    virtual int GetY() = 0;

    virtual int centerX() = 0;
    virtual int centerY() = 0;

    virtual void SetX(int val) {}
    virtual void SetY(int val) {}

    virtual bool GetAccurate() {
      return true;
    }

    int GetAttachment() {
      return attachment;
    }

    virtual bool isComplite() = 0;
};
