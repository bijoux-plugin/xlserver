#!/bin/bash

#mvn clean package -DskipTests=true

JAR_FILE=`ls -1 target | grep mock-xlclient`

java -jar ./target/$JAR_FILE

