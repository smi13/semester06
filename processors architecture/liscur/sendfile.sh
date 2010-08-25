#!/usr/bin/expect -f

set timeout 100
set PASS "zxc"
set USER "root"

spawn scp liscur root@192.168.111.1:/media/sd
expect "password: "
send "$PASS\r"
expect eof


