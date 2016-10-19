{
    "connections": [
        {
            "receiver": "dnsProcessor1",
            "sender": "netFilter1",
            "signal": "captured(GPacket*)",
            "slot": "process(GPacket*)"
        },
        {
            "receiver": "ipFlowMgr1",
            "sender": "netFilter1",
            "signal": "captured(GPacket*)",
            "slot": "process(GPacket*)"
        },
        {
            "receiver": "tcpFlowMgr1",
            "sender": "ipFlowMgr1",
            "signal": "processed(GPacket*)",
            "slot": "process(GPacket*)"
        },
        {
            "receiver": "udpFlowMgr1",
            "sender": "ipFlowMgr1",
            "signal": "processed(GPacket*)",
            "slot": "process(GPacket*)"
        },
        {
            "receiver": "dnsFirewall1",
            "sender": "netFilter1",
            "signal": "captured(GPacket*)",
            "slot": "check(GPacket*)"
        }
    ],
    "nodes": [
        {
            "_class": "GNetFilter",
            "_x": -66,
            "_y": -299,
            "acceptVerdict": "REPEAT",
            "autoParse": true,
            "autoRead": true,
            "enabled": true,
            "mark": "1234",
            "objectName": "netFilter1",
            "queueNum": "0",
            "snapLen": "65536"
        },
        {
            "_class": "GCommand",
            "_x": -72,
            "_y": -340,
            "closeCommandList": [
                "iptables -D OUTPUT -m mark --mark 1234 -j ACCEPT",
                "iptables -D OUTPUT -j NFQUEUE --queue-num 0",
                "",
                "iptables -D INPUT -m mark --mark 1234 -j ACCEPT",
                "iptables -D INPUT -j NFQUEUE --queue-num 0",
                ""
            ],
            "objectName": "command1",
            "openCommandList": [
                "iptables -A OUTPUT -m mark --mark 1234 -j ACCEPT",
                "iptables -A OUTPUT -j NFQUEUE --queue-num 0",
                "",
                "iptables -A INPUT -m mark --mark 1234 -j ACCEPT",
                "iptables -A INPUT -j NFQUEUE --queue-num 0",
                ""
            ]
        },
        {
            "_class": "GDnsProcessor",
            "_x": -174,
            "_y": -242,
            "objectName": "dnsProcessor1",
            "port": "53"
        },
        {
            "_class": "GIpFlowMgr",
            "_x": -70,
            "_y": -246,
            "checkInterval": "1",
            "fullTimeout": "180",
            "halfTimeout": "60",
            "objectName": "ipFlowMgr1"
        },
        {
            "_class": "GTcpFlowMgr",
            "_x": -125,
            "_y": -185,
            "checkInterval": "1",
            "finTimeout": "20",
            "fullTimeout": "180",
            "halfTimeout": "60",
            "objectName": "tcpFlowMgr1",
            "rstTimeout": "10"
        },
        {
            "_class": "GUdpFlowMgr",
            "_x": -25,
            "_y": -185,
            "checkInterval": "1",
            "fullTimeout": "180",
            "halfTimeout": "60",
            "objectName": "udpFlowMgr1"
        },
        {
            "_class": "GDnsFirewall",
            "_x": 61,
            "_y": -239,
            "checkInterval": "1",
            "dnsProcessor": "dnsProcessor1",
            "ipFlowMgr": "ipFlowMgr1",
            "objectName": "dnsFirewall1",
            "tcpFlowMgr": "tcpFlowMgr1",
            "udpFlowMgr": "udpFlowMgr1"
        }
    ]
}
