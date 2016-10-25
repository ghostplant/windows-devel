auto ans;

void prime(auto x) {
  auto i;
  if (x<=1) {
    ans=-1;
  } else {
    ans=1;
    i=2;
    while (i*i<=x) {
      if (x%i==0) {
        ans=-1;
        i=x;
      }
      i=i+1;
    }
  }
}

void main() {
  auto n;
  scan(n);
  prime(n);
  print(ans);
  println();
}