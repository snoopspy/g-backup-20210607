COMMAND=$1
INTERFACE=$2

function usage() {
	echo "usage : dummy <install|uninstall|add|del> [<interface>]"
	echo "example :"
	echo "  dummy.sh install"
	echo "  dummy.sh uninstall"
	echo "  dummy.sh add dum0"
	echo "  dummy.sh del dum0"
}

if [ -z "$COMMAND" ]; then
  usage
  exit 1
fi

if [ "$COMMAND" == "install" ]; then
  # echo "Installing dummy kernel module"
  sudo modprobe dummy
  exit 0
fi

if [ $COMMAND == "uninstall" ]; then
  # echo "Uninstalling dummy kernel module"
  sudo rmmod dummy
  exit 0
fi

if [ "$COMMAND" == "add" ]; then
  # echo "Adding dummy interface"
  if [ -z "$INTERFACE" ]; then
    echo "Interface not specified"
    exit 1
  fi
  sudo ip link set name "$INTERFACE" dev dummy0
  sudo ifconfig "$INTERFACE" up
  exit 0
fi

if [ "$COMMAND" == "del" ]; then
  # echo "Deleting dummy interface"
  if [ -z "$INTERFACE" ]; then
    echo "Interface not specified"
    exit 1
  fi
  sudo ip link delete "$INTERFACE" type dummy
  exit 0
fi

echo "Invalid command"
usage

