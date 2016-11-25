FROM=$1
TO=$2

function usage() {
	echo "usage : grename.sh <from> <to>"
	echo "example :"
	echo "  grename.sh enp2s0 eth0"
}

if [ -z "$FROM" -o -z "$TO" ]; then
  usage
  exit 1
fi

sudo ifconfig "$FROM" down
sudo ip link set name "$TO" dev "$FROM"
sudo ifconfig "$TO" up

