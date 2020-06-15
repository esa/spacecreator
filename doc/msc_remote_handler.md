# MSC Remote Handler {#RemotehandlerPage}

The remote control is accessible via web socket. It is used to control the MSC document (adding items).
The MSC editor has to be in a special mode for this (**streaming mode** whith user interaction disabled, and no menu etc.).
To enable that mode set the port the editor listens to the commands:

> ./msceditor -p 34622

For documentation of the protocol (JSON objects) have a look at the [msc::RemoteControlWebServer](@ref msc::RemoteControlWebServer) documentation.


## Internal handling

![Remote handling](remote_handler01.png)

[msc::RemoteControlWebServer](@ref msc::RemoteControlWebServer) starts a web socket server to listen for commnds in JSON format.
Those are then processed by [msc::RemoteControlHandler](@ref msc::RemoteControlHandler).
Those commands are using the undo/redo framework of the MSC editor to perform the commands on the msc chart.
The undo/redo does operate on the current [msc::MscChart](@ref msc::MscChart) of the [msc::MscModel](@ref msc::MscModel).
For more information on the msc chart drawing, see the [Acrchitecture - MSC - Main building blocks](@ref MSCMainBuildingBlocks).
