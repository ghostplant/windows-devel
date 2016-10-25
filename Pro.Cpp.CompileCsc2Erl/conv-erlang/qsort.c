const auto MAX=100000;
auto arr[MAX];

void generate() {
	auto i;
	i=0;
	while (i<MAX) {
		arr[i]=100*i*i%31;
		i=i+1;
	}
}

void qsort(auto l,auto r) {
	auto i, j, flag, tmp;
	i=l;
	j=r;
	tmp=arr[i];
	while (i<j) {
		flag=0;
		if (i<j) {
			if (tmp<=arr[j]) {
				flag=1;
			}
		}
		while (flag>0) {
		//while (i<j && tmp<=arr[j]) {
			j=j-1;

			flag=0;
			if (i<j) {
				if (tmp<=arr[j]) {
					flag=1;
				}
			}
		}
		arr[i]=arr[j];

		flag=0;
		if (i<j) {
			if (tmp>=arr[i]) {
				flag=1;
			}
		}
		while (flag>0) {
			i=i+1;
		//while (i<j && tmp>=arr[i]) {
			flag=0;
			if (i<j) {
				if (tmp>=arr[i]) {
					flag=1;
				}
			}
		}
		arr[j]=arr[i];
	}
	arr[i]=tmp;
	if (l<i-1) {
		qsort(l, i-1);
	}
	if (r>i+1) {
		qsort(i+1, r);
	}
}

void output() {
	auto i;
	i=0;
	while (i<MAX) {
		print(arr[i]);
		println();
		i=i+1;
	}
}

void main() {
	generate();
	qsort(0, MAX-1);
	output();
}
