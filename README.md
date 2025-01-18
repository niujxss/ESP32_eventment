# GetEnvironmentStatus
ESP32-C3+sht41 采集温湿度
服务器数据库POSTGRES
TCP服务器 Rust开发，ESP32-C3 采集完数据后通过TCP发送数据；服务器端RUST TCP服务获取数据；并存储在数据库中；
WEB服务；后端python+Flask; 前端JavaScript,温度数据可以表格呈现；也能折线图显示变化曲线


采集环境数据

启动顺序：
1、启动数据库
    1）运行～/目录下 stopsql.sh 脚本；关闭docker自动启动的数据库
    2）sudo su - postgres 切换到 postgres 用户下；执行 start.sh 启动项目需要的数据库
    3) 退出postgres 用户，回到 honey 用户;执行脚本 sql.sh; 进入数据库;
    4) 在数据库执行 select * from dongchazhedata2 order by id desc limit 10 ;
       查询最后10 条数据；

2、启动TCP服务
    在工程目录下执行：
    cd TCPServer/objsql
    ./target/release/myconnectpsql > log.txt &
3、启动WEB服务
    cd web;
    python main.py
 
