{
    "connections": [
        {
            "receiver": "tcpFlowMgr1",
            "sender": "netFilter1",
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
            "_class": "GNetFilter",
            "_x": 73,
            "_y": -130,
            "acceptVerdict": "ACCEPT",
            "autoParse": true,
            "command": {
                "closeCommands": [
                    {
                        "arguments": [
                            "iptables",
                            "-D",
                            "OUTPUT",
                            "-p",
                            "tcp",
                            "-j",
                            "NFQUEUE"
                        ],
                        "commandType": "Execute",
                        "objectName": "",
                        "program": "sudo"
                    },
                    {
                        "arguments": [
                            "iptables",
                            "-D",
                            "INPUT",
                            "-p",
                            "tcp",
                            "-j",
                            "NFQUEUE"
                        ],
                        "commandType": "Execute",
                        "objectName": "",
                        "program": "sudo"
                    }
                ],
                "objectName": "",
                "openCommands": [
                    {
                        "arguments": [
                            "iptables",
                            "-A",
                            "OUTPUT",
                            "-p",
                            "tcp",
                            "-j",
                            "NFQUEUE"
                        ],
                        "commandType": "Execute",
                        "objectName": "",
                        "program": "sudo"
                    },
                    {
                        "arguments": [
                            "iptables",
                            "-A",
                            "INPUT",
                            "-p",
                            "tcp",
                            "-j",
                            "NFQUEUE"
                        ],
                        "commandType": "Execute",
                        "objectName": "",
                        "program": "sudo"
                    }
                ]
            },
            "enabled": true,
            "mark": "0",
            "objectName": "netFilter1",
            "queueNum": "0",
            "snapLen": "65536"
        },
        {
            "_class": "GTcpFlowMgr",
            "_x": 61,
            "_y": -64,
            "checkInterval": "1",
            "finTimeout": "20",
            "fullTimeout": "180",
            "halfTimeout": "60",
            "objectName": "tcpFlowMgr1",
            "rstTimeout": "10"
        },
        {
            "_class": "GClientHelloSplit",
            "_x": 50,
            "_y": 1,
            "objectName": "clientHelloSplit1",
            "tcpFlowMgr": "tcpFlowMgr1"
        },
        {
            "_class": "GRawIpSocketWrite",
            "_x": 41,
            "_y": 61,
            "objectName": "rawIpSocketWrite1"
        }
    ]
}
