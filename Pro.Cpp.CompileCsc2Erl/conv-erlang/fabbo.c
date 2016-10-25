auto ans;

void fabbo(auto n) {
	auto i, a, b;
	if (n<=2) {
		ans=1;
	} else {
		a=1;
		b=1;
		while (n>=3){
			ans=a+b;
			a=b;
			b=ans;
			n=n-1;
		}
	}
}

void main() {
  auto n;
  scan(n);
  fabbo(n);
  print(ans);
  println();
}
