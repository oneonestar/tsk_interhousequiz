#!/usr/bin/expect -f
spawn telnet 127.0.0.1 9000   
send "Score Update A 10\n"
interact
