MODULE_NAME=$1

if [[ ( $MODULE_NAME != "" ) && ( -f "$MODULE_NAME.c" ) ]]; then

	echo -e "\e[36mCompiling the source code of converters...\e[0m"
	rm -f "$MODULE_NAME.erl" "$MODULE_NAME.beam"
	gcc -ggdb3 ./convert1-src/cs*.c -o convert1
	g++ ./convert2-src/*.cpp -o convert2

	echo -e "\e[36mPerforming the converting algorithm...\e[0m"
	./convert1 "$MODULE_NAME.c" | ./convert2 "$MODULE_NAME"
	rm convert1 convert2

	if [[ ( "`which erl`" == "" ) ]]; then
		echo -e "\e[36mFinish converting to Erlang code, but no erlang compiler found to continue.\e[0m"
	else
		[ -f "$MODULE_NAME.erl" ] && cat "$MODULE_NAME.erl" && erlc "$MODULE_NAME.erl" | grep -v "Warning"
		[ -f "$MODULE_NAME.beam" ] && echo -e "\e[36mSuccessfully converting to erlang code and compiling!\e[0m"
	fi
else
	echo -e "\e[36mUsage: ./makebin_and_convert.sh [fabbo|prime|sort|qsort|..]\e[0m"
fi
