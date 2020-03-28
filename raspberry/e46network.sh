#!/bin/bash
iw dev wlan0 interface add wlan0_ap type __ap
macchanger -e wlan0_ap
create_ap wlan0_ap wlan0 TomiMetria
