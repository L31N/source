while true
do
echo "-------------"
i2cget -y 2 0x60 0x02 b
i2cget -y 2 0x60 0x03 b
done
