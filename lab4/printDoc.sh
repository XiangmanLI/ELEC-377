#!/bin/bash

string="==================================="
echo $string

for f in `find ./labs -type f -name "*.[ch]"`; do
	echo $string
	sed -e '/\/\/+/,/\/\/-/p' -e 'd' $f | sed -e 's/\/\///' | sed -e 's/[+-]//'
done