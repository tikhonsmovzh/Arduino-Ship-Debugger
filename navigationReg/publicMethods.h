class Vector2
{
    int x, y;
  public:
    Vector2(int x = 0, int y = 0)
    {
      this->x = x;
      this->y = y;
    }

    int GetX() {
      return x;
    }
    int GetY() {
      return y;
    }
};

int sign(int val) {
  if (val < 0) return -1;
  if (val == 0) return 0;
  return 1;
}

int toIntB(byte a, byte b)
{
  int val = a << 8;
  return b | val;
}

void servoRotate(int degree1, Servo *serv0)
{
  if (abs(degree1) > 90)
    degree1 = sign(degree1) * 90;

  serv0->write(degree1 + 90);
}

void WriteInt(int val)
{
  Serial.write((byte)(val >> 8));
  Serial.write((byte)val);
}

bool isEnable = false;

const int buzzer = 13;
const int quantityBuoys = 1;

enum Dist { xPlus, yPlus, xMinus, yMinus };
