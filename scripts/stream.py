#!/usr/bin/env python

import websocket
import re
import json
import time


try:
    import thread
except ImportError:
    import _thread as thread
import time

idx = 0

with open('astream.json') as json_file:
    actions = json.load(json_file)['actions']

def run(*args):
    global idx
    print(idx, len(actions))
    time.sleep(0)

    msg = actions[idx]
    idx += 1
    command = msg['CommandType']
    if command.lower() == "quit" or command.lower() == "exit" or idx >= len(actions):
        print("quiting loop...")
        ws.close()
        return

    ws.send(json.dumps(msg))


def on_message(ws, message):
    response = json.loads(message)
    if not response['result']:
        print ("FAIL: " + response.get('errorString', 'unknown error'))
    else:
        print ("SUCCESS")
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
