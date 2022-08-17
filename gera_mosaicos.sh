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

	for i in 10 16 20 30 32 
	do
		printf "\nCriando mosaico $i\n\n"
		./mosaico < $AUX > saida$i.ppm -p tiles/tiles$i/
		printf "\n#############################################\n"
	done
