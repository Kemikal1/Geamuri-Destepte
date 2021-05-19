#!/bin/sh

SLEEP=3

curl -X GET localhost:9080/properties/heliomare
sleep $SLEEP

curl -X POST localhost:9080/properties/heliomare/20
curl -X GET localhost:9080/properties/heliomare
sleep $SLEEP

curl -X POST localhost:9080/properties/heliomare/60
curl -X GET localhost:9080/properties/heliomare
sleep $SLEEP

curl -X POST localhost:9080/properties/heliomare/70
curl -X GET localhost:9080/properties/heliomare
sleep $SLEEP

curl -X GET localhost:9080/properties/incalzire
sleep $SLEEP

curl -X POST localhost:9080/properties/incalzire/-5
curl -X GET localhost:9080/properties/incalzire
sleep $SLEEP

curl -X POST localhost:9080/properties/incalzire/20
curl -X GET localhost:9080/properties/incalzire
sleep $SLEEP

curl -X GET localhost:9080/properties/trapa
sleep $SLEEP

curl -X POST localhost:9080/actions/trapa/open
curl -X GET localhost:9080/properties/trapa
sleep $SLEEP

curl -X POST localhost:9080/properties/trapa/100
curl -X GET localhost:9080/properties/trapa
sleep $SLEEP

curl -X POST localhost:9080/actions/trapa/close
curl -X GET localhost:9080/properties/trapa
sleep $SLEEP


curl -X GET localhost:9080/properties/stergatoare
sleep $SLEEP

curl -X POST localhost:9080/actions/stergatoare/on
curl -X GET localhost:9080/properties/stergatoare
sleep $SLEEP

curl -X POST localhost:9080/properties/stergatoare/100
curl -X GET localhost:9080/properties/stergatoare
sleep $SLEEP

curl -X POST localhost:9080/properties/stergatoare/50
curl -X GET localhost:9080/properties/stergatoare
sleep $SLEEP

curl -X POST localhost:9080/actions/stergatoare/off
curl -X GET localhost:9080/properties/stergatoare
sleep $SLEEP

curl -X POST localhost:9080/properties/stergatoare/100
curl -X GET localhost:9080/properties/stergatoare
sleep $SLEEP

curl -X POST localhost:9080/properties/stergatoare/50
curl -X GET localhost:9080/properties/stergatoare
