{
    "connections": [
        {
            "receiver": "bpFilter1",
            "sender": "winDivert1",
            "signal": "captured(GPacket*)",
            "slot": "check(GPacket*)"
        },
        {
            "receiver": "packetDebugger1",
            "sender": "bpFilter1",
            "signal": "filtered(GPacket*)",
            "slot": "debug(GPacket*)"
        },
        {
            "receiver": "block1",
            "sender": "packetDebugger1",
            "signal": "debugged(GPacket*)",
            "slot": "block(GPacket*)"
        }
    ],
    "nodes": [
        {
            "_class": "GWinDivert",
            "_x": -39,
            "_y": -44,
            "autoParse": true,
            "correctChecksum": true,
            "enabled": true,
            "filter": "true",
            "flags_": "0",
            "layer": "0",
            "objectName": "winDivert1",
            "priority": "0",
            "queueLen_": "8192",
            "queueTime_": "1024",
            "tos": "0"
        },
        {
            "_class": "GBpFilter",
            "_x": -34,
            "_y": -5,
            "dataLinkType": "Eth",
            "filter": "",
            "objectName": "bpFilter1"
        },
        {
            "_class": "GPacketDebugger",
            "_x": -60,
            "_y": 34,
            "enabled": true,
            "objectName": "packetDebugger1"
        },
        {
            "_class": "GBlock",
            "_x": -30,
            "_y": 75,
            "enabled": true,
            "objectName": "block1"
        }
    ]
}
