#!/bin/bash

#mvn clean package -DskipTests=true

JAR_FILE=`ls -1 target | grep bijoux*`
#echo $JAR_FILE

java -jar ./target/$JAR_FILE

