#!/bin/bash

for (( I = 1 ; I < 10 ; I = I + 1 )) do
	ab -n 100 -c 4 http://10.32.170.112:8000/?kbyte=10
	sleep 5
done

echo 'Finish Quick'

sleep 15

ab -n 2000 -c 10 http://10.32.170.112:8000/?kbyte=20

echo 'Finish Huge'

sleep 15

for (( I = 1 ; I < 10 ; I = I + 1 )) do
	ab -n 300 -c 2 http://10.32.170.112:8000/?kbyte=20
	sleep 15
done

echo 'Finish Medium'

