mosquitto_sub -h raspberry.local -p 1883 -t "house/room" -v -u pi -P "password"

sudo systemctl stop mosquitto
sudo service mosquitto stop
sudo systemctl start mosquitto
sudo service mosquitto start
sudo systemctl status mosquitto
sudo service mosquitto status
