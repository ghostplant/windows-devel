if [[ ( $1 != "" ) && ( -f "$1.c" ) ]]; then
	g++ toErlangX.cpp -o toErlangX.bin
	g++ toBraches.cpp -o toBraches.bin
	g++ toMerge.cpp -o toMerge.bin

	./toErlangX.bin < "$1.c" | ./toBraches.bin | ./toMerge.bin
	rm *.bin
else
	echo -e "\e[36mUsage: ./makebin_and_convert.sh [fabbo|prime|..]\e[0m"
fi
