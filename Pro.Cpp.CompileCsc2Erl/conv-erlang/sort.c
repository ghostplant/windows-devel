const auto MAX=100;
auto arr[MAX];

void generate() {
	auto i;
	i=0;
	while (i<MAX) {
		arr[i]=89*i*i%101;
		i=i+1;
	}
}

void sort(auto times) {
	auto i, tmp;
	i=1;
	while (i<MAX) {
		if (arr[i-1]>arr[i]) {
			tmp = arr[i-1];
			arr[i-1]=arr[i];
			arr[i]=tmp;
		}
		i=i+1;
	}
	if (times<MAX) {
		sort(times+1);
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
	sort(0);
	output();
}