# General

Start the MSC editor in streaming mode (setting the port to listen to):
`./mscstreaming -p 34622`

To run the helper script to send the commands:
In `examples/msc_remote_control`
`./stream.py`

This will send the commands stored in `astream.json`.


# 1 Run simple test

**Steps**

* In the build directory run `bin/mscstreaming -p 34622 &`
  * If you run this in Qt Creator, add "-p 34622" as command line argument on mscstreaming in the Projects tab.
* In the source directory go to `examples/msc_remote_control`
* Run `./stream.py`. On Windows, run `python ./stream.py`

**Check for**

* Check if there are 3 instances. And 10 entities (messages, timers, actions, conditions).
