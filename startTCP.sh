#!/bin/bash
./TCPServer/objsql/target/release/myconnectpsql & > /dev/null

echo "请查看启动情况"
ps -ef |grep myconnectpsql |grep -v grep
