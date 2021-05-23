{
    "connections": [
        {
            "receiver": "tcpFlowMgr1",
            "sender": "asyncNetFilter1",
            "signal": "captured(GPacket*)",
            "slot": "process(GPacket*)"
        },
        {
            "receiver": "clientHelloSplit1",
            "sender": "tcpFlowMgr1",
            "signal": "processed(GPacket*)",
            "slot": "split(GPacket*)"
        },
        {
            "receiver": "rawIpSocketWrite1",
            "sender": "clientHelloSplit1",
            "signal": "splitted(GPacket*)",
            "slot": "write(GPacket*)"
        }
    ],
    "nodes": [
        {
            "_class": "GAsyncNetFilter",
            "_x": -41,
            "_y": -91,
            "acceptVerdict": "ACCEPT",
            "autoParse": true,
            "command": {
                "closeCommands": [
                    {
                        "commandType": "Execute",
                        "commands": [
                            "su -c 'iptables -D OUTPUT -d 127.0.0.1 -j ACCEPT'",
                            "su -c 'iptables -D INPUT -d 127.0.0.1 -j ACCEPT'",
                            "su -c 'iptables -D OUTPUT -j NFQUEUE'",
                            "su -c 'iptables -D INPUT -j NFQUEUE'"
                        ],
                        "objectName": ""
                    }
                ],
                "objectName": "",
                "openCommands": [
                    {
                        "commandType": "Execute",
                        "commands": [
                            "su -c 'iptables -F'",
                            "su -c 'iptables -A OUTPUT -d 127.0.0.1 -j ACCEPT'",
                            "su -c 'iptables -A INPUT -d 127.0.0.1 -j ACCEPT'",
                            "su -c 'iptables -A OUTPUT -j NFQUEUE'",
                            "su -c 'iptables -A INPUT -j NFQUEUE'"
                        ],
                        "objectName": ""
                    }
                ]
            },
            "enabled": true,
            "mark": "0",
            "objectName": "asyncNetFilter1",
            "queueNum": "0"
        },
        {
            "_class": "GTcpFlowMgr",
            "_x": -33,
            "_y": -34,
            "checkInterval": "1",
            "finTimeout": "20",
            "fullTimeout": "180",
            "halfTimeout": "60",
            "objectName": "tcpFlowMgr1",
            "rstTimeout": "10"
        },
        {
            "_class": "GClientHelloSplit",
            "_x": -43,
            "_y": 17,
            "objectName": "clientHelloSplit1",
            "tcpFlowMgr": "tcpFlowMgr1"
        },
        {
            "_class": "GRawIpSocketWrite",
            "_x": -53,
            "_y": 69,
            "objectName": "rawIpSocketWrite1"
        }
    ]
}
