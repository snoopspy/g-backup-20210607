COMMAND=$1
INTERFACE=$2

function usage() {
	echo "usage : gdummy.sh <start|stop|add|del> [<interface>]"
	echo "example :"
	echo "  gdummy.sh start"
	echo "  gdummy.sh stop"
	echo "  gdummy.sh add dum0"
	echo "  gdummy.sh del dum0"
}

if [ -z "$COMMAND" ]; then
	usage
	exit 1
fi

if [ "$COMMAND" == "start" ]; then
	# echo "Installing dummy kernel module"
	sudo modprobe dummy
	sudo ip link delete dummy0 type dummy
	exit 0
fi

if [ $COMMAND == "stop" ]; then
	# echo "Uninstalling dummy kernel module"
	sudo modprobe -r dummy
	exit 0
fi

if [ "$COMMAND" == "add" ]; then
	# echo "Adding dummy interface"
	if [ -z "$INTERFACE" ]; then
		echo "Interface not specified"
		exit 1
	fi
	sudo ip link add "$INTERFACE" type dummy
	sudo ifconfig "$INTERFACE" up
	exit 0
fi

if [ "$COMMAND" == "del" ]; then
	# echo "Deleting dummy interface"
	if [ -z "$INTERFACE" ]; then
		echo "Interface not specified"
		exit 1
	fi
	sudo ifconfig "$INTERFACE" down
	sudo ip link delete "$INTERFACE" type dummy
	exit 0
fi

echo "Invalid command"
usage

