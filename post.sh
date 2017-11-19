#!/bin/bash

curl -H 'Content-Type: application/json' -X PUT -d '[{"eventId":"00000000-0000-0000-0000-000000000000","eventType":"PlayerCreated","streamId":"player/adam","version":0,"metadata":"e30=","payload":"e30="}]' localhost:5000/Api/Query/
