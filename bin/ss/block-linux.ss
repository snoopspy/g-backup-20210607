{
    "connections": [
        {
            "receiver": "bpFilter1",
            "sender": "netFilter1",
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
            "_class": "GNetFilter",
            "_x": -46,
            "_y": -78,
            "acceptVerdict": "ACCEPT",
            "autoParse": true,
            "command": {
                "closeCommands": [
                    "sudo iptables -D OUTPUT -j NFQUEUE",
                    "sudo iptables -D INPUT -j NFQUEUE"
                ],
                "objectName": "",
                "openCommands": [
                    "sudo iptables -A OUTPUT -j NFQUEUE",
                    "sudo iptables -A INPUT -j NFQUEUE"
                ]
            },
            "enabled": true,
            "mark": "0",
            "objectName": "netFilter1",
            "queueNum": "0",
            "snapLen": "65536"
        },
        {
            "_class": "GBpFilter",
            "_x": -44,
            "_y": -33,
            "dataLinkType": "Eth",
            "filter": "",
            "objectName": "bpFilter1"
        },
        {
            "_class": "GPacketDebugger",
            "_x": -75,
            "_y": 11,
            "enabled": true,
            "objectName": "packetDebugger1"
        },
        {
            "_class": "GBlock",
            "_x": -39,
            "_y": 52,
            "enabled": true,
            "objectName": "block1"
        }
    ]
}
