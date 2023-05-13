class DynamicStructure
{
    leg *first, *last, *current;

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

    leg* GetCurrent() {
      return current;
    }

    void Clear()
    {
      first = NULL;
      last = NULL;
    }

    bool IsCurrentLast() {
      return current == NULL;
    }

    void StepForward() {
      current = current->Next;
    }

    void CurrentToFirst() {
      current = first;
    }
};
