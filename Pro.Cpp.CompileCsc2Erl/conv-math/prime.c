auto f(N) {
  M=N%2;
  if (N<2)
    Result=false;
  elseif (N<4)
    Result=true;
  elseif (M==0)
    Result=false;
  else {
    Found=false;
    At=2;
    while ((Found==false) and (At*At<=N)) {
      if (N%At==0)
        Found=true;
      At=At+1;
    }
    Result=(Found==false);
  }
}
