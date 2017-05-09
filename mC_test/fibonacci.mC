int fibonacci(int n)
{
  if (n > 1)
    return fibonacci(n-1) + fibonacci(n-2);
  else
    return n;
}


int main()
{
  output(fibonacci(0));
  output(fibonacci(1));
  output(fibonacci(2));
  output(fibonacci(3));
  output(fibonacci(4));
  output(fibonacci(5));
  output(fibonacci(6));
  output(fibonacci(7));
  output(fibonacci(8));
}


