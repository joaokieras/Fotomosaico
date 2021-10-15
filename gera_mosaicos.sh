#!/bin/bash
	make
	printf "\n"
	make clean
	printf "\n"
	ARQ=$1
	TIPO_ARQ=`file -b $ARQ | cut -c1-6`
	if [ "$TIPO_ARQ" != "Netpbm" ]
	then
		printf "Convertendo o arquivo $ARQ em PPM\n"
		convert $ARQ saida0.ppm
		AUX=saida0.ppm 
	else
		AUX=$ARQ
	fi
	printf "\nCriando mosaico 10\n\n"
	./mosaico < $AUX > saida10.ppm -p tiles10/
	printf "\n=============================================\n"
	printf "\nCriando mosaico 16\n\n"
	./mosaico < $AUX > saida16.ppm -p tiles16/
	printf "\n=============================================\n"
	printf "\nCriando mosaico 20\n\n"
	./mosaico < $AUX > saida20.ppm -p tiles20/
	printf "\n=============================================\n"
	printf "\nCriando mosaico 30\n\n"
	./mosaico < $AUX > saida30.ppm -p tiles30/
	printf "\n=============================================\n"
	printf "\nCriando mosaico 32\n\n"
	./mosaico < $AUX > saida32.ppm -p tiles32/

