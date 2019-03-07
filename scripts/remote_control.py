#!/usr/bin/env python

import websocket
import re
import json

try:
    import thread
except ImportError:
    import _thread as thread
import time

def run(*args):
    x = raw_input('Enter command:').strip()

    m = re.search(r'^(\S+)\s*\((.*)\)$', x)
    try:
        command = m.group(1)
        args = m.group(2)
        params = {}

        if args:
            parameters = args.split(',')
            for param in parameters:
                keyValue = param.split('=')
                params[keyValue[0].strip()] = keyValue[1].strip()

    except Exception as e:
        print ("Command syntax: Command(optional parameter1=value1, optional parametr2=value2, ...)")
        return

    if command.lower() == "quit" or command.lower() == "exit":
        print("quiting loop...")
        ws.close()
        return
    
    msg = { 'CommandType': command, 'Parameters': params }  
    
    ws.send(json.dumps(msg))


def on_message(ws, message):
    response = json.loads(message)
    if response['result']:
        print ("SUCCESS")
    else:
        print ("FAIL: " + response.get('errorString', 'unknown error'))
    thread.start_new_thread(run, ())

def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### closed ###")

def on_open(ws):
    print ("Command syntax: command (optional parameter1=value1, optional parametr2=value2, ...)")
    thread.start_new_thread(run, ())


if __name__ == "__main__":
    # websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://localhost:34622/",
                              on_message = on_message,
                              on_error = on_error,
                              on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()