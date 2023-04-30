class DynamicStructure
{
    leg *first, *last;

  public:
    leg* Dequeue()
    {
      leg *saveFirst = first;

      if (first != NULL)
      {
        first = first->Next;

        if (first == NULL)
          last = NULL;
      }

      return saveFirst;
    }

    void Enqueu(leg *data)
    {
      if (first == NULL)
      {
        first = data;
        last = data;

        return;
      }

      last->Next = data;

      last = data;
    }

    void Clear()
    {
      first = NULL;
      last = NULL;
    }
};
