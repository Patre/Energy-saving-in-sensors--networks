#!/bin/sh

sed -e 's,[@]filename[@],'$1',g' -e  's,[@]outputname[@],'$2',g'  -e  's,[@]protocol[@],'$3',g'  < @datadir@/activenodes.in | gnuplot


