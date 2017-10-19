#!/bin/bash

#Carga para testes com o programa cyclictest
sudo taskset -c 0 /bin/ping -l100000 -q -s10 -f localhost
