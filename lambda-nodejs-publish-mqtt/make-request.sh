#!/bin/bash

#curl https://dale.lefthandedcatstudio.com/get-images

# Works
#curl -d '{"foo":"bar"}' -H "Content-Type: application/json" -X POST https://5wespe02e2.execute-api.us-east-2.amazonaws.com/dev/publish-mqtt


curl -d '{"foo":"bar"}' -H "Content-Type: application/json" -X POST https://dale.lefthandedcatstudio.com/iot/publish-mqtt


