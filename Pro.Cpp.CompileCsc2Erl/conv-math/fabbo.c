auto f(N) {
  if (N<3)
    Result=1;
  else {
    A=1,B=1;
    while (N>2) {
      A=A+B;
      B=A-B;
      N=N-1;
    }
    Result=A;
  }
}

