#/****************************************************************************
#*
#* Copyright 2018 Primesoft All Rights Reserved.
#*
#* Filename: 663_sendTemp.py
#* Author: enj.park, yr.kim
#* Release date: 2018/05/02
#* Version: 2.1
#* Modified date : 2018/05/23 by sj.yang
#*
#****************************************************************************/

import artikcloud
from artikcloud.rest import ApiException
import sys, getopt
import time, json
from pprint import pprint

def main(argv):
        DEFAULT_CONFIG_PATH = 'config.json'

        with open(DEFAULT_CONFIG_PATH, 'r') as config_file:
                        config = json.load(config_file)
#       print(config)

        # Configure Oauth2 access token for the client application
        artikcloud.configuration = artikcloud.Configuration();
        artikcloud.configuration.access_token = config['device_token']


        # File In/Out
        ff = open('send_data.txt', 'r')
        adcVal = float(ff.read())
        print "[  SEND  ] ADC value is", str(adcVal)
        print('  ')

        # Create an instance of the API Class
        api_instance = artikcloud.MessagesApi()
        device_message = {}
        device_message['TEMP'] = adcVal
        device_sdid = config['device_id']
        ts = None                         # timestamp

        # Construct a Message Object for request
        data = artikcloud.Message(device_message, device_sdid, ts)

        try:
#               pprint(artikcloud.configuration.auth_settings()) # Debug Print
                api_response = api_instance.send_message(data) # Send Message
#               pprint(api_response)
        except ApiException as e:
                pprint("Exception when calling MessagesApi->send_message: %s\n" % e)

if __name__ == "__main__":
        while True:
                main(sys.argv[1:])
                time.sleep(5)