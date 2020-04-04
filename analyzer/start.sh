#!/bin/bash
CLASSFILES=*.java

for i in $CLASSFILES; do
	echo "Compiling $i"
	javac $i
done

java AnalyzeGUI
