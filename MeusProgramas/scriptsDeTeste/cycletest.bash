#!/bin/bash

#1. Geração do arquivo de histograma
#1.1 Executa 10 milhões de repetições:
#./cyclictest -a0 -i100 -h500 -l10000000 -m -n -p99 -q -t20 > saida.hst
#./cyclictest -a0 -D3h -i100 h500 -m -n -p99 -q -t20 > saida.hst

#2. Execução do teste sem histograma
#2.1 Execução de 10 milhoes de repetições:
./cyclictest -a0 -i100 -l10000000 -m -n -p99 -q -t20 
