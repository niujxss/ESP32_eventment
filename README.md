# GetEnvironmentStatus

基于 ESP32-C3 + SHT41 的温湿度采集系统：设备端采集 → TCP 上报 → Rust 服务入库 → Flask + ECharts 展示。

## 项目组成

| 组件 | 技术栈 | 目录 |
| --- | --- | --- |
| 采集端 | ESP32-C3 + SHT41（ESP-IDF） | `devices/` |
| TCP 服务与入库 | Rust | `TCPServer/objsql/` |
| 数据库 | PostgreSQL | 表 `dongchazhedata2` |
| Web 服务 | 后端 Python + Flask，前端 JavaScript + ECharts | `web/` |

- ESP32-C3 采集完数据后通过 TCP 发送数据
- 服务器端 Rust TCP 服务获取数据，并存储在数据库中
- Web 端温湿度数据可以表格呈现，也能用折线图显示变化曲线

## 数据流

```text
ESP32-C3 + SHT41
        │  TCP 报文，例如 "26.09 36.17 end"
        ▼
Rust TCP 服务（TCPServer/objsql）
        │  INSERT INTO dongchazhedata2
        ▼
PostgreSQL
        │  SELECT
        ▼
Flask Web 服务
        │  HTTP
        ▼
浏览器（表格 / ECharts 折线图）
```

## 目录结构

```text
.
├── devices/                  ESP32-C3 固件（ESP-IDF 工程）
│   └── main/                 采集任务、I2C 驱动、TCP 客户端
├── TCPServer/
│   └── objsql/               Rust TCP 服务 + PostgreSQL 入库
├── web/                      Flask Web 服务
│   ├── main.py               路由与数据接口
│   ├── templates/            HTML 模板
│   └── static/               JS / CSS / 图表库
├── startTCP.sh               启动 TCP 服务的脚本
└── README.md
```

## 启动顺序

### 1. 启动数据库

1. 运行 `~/` 目录下的 `stopsql.sh` 脚本，关闭 docker 自动启动的数据库
2. `sudo su - postgres` 切换到 postgres 用户下，执行 `start.sh` 启动项目需要的数据库
3. 退出 postgres 用户，回到 `honey` 用户，执行脚本 `sql.sh` 进入数据库
4. 在数据库执行以下语句，查询最后 10 条数据：

   ```sql
   select * from dongchazhedata2 order by id desc limit 10;
   ```

### 2. 启动 TCP 服务

在工程目录下执行：

```bash
cd TCPServer/objsql
./target/release/myconnectpsql > log.txt &
```

也可以直接运行根目录的 `startTCP.sh`。

### 3. 启动 WEB 服务

```bash
cd web
python main.py
```

浏览器访问 `http://<服务器IP>:1024/`。

## 端口与地址

下表摘自当前源码，改动前请先确认实际部署环境：

| 用途 | 地址 | 出处 |
| --- | --- | --- |
| ESP32 上报目标 | `192.168.31.138:9000` | `devices/sdkconfig` |
| Rust TCP 服务监听 | `0.0.0.0:9000` | `TCPServer/objsql/src/mytcp.rs` |
| Flask Web 服务监听 | `0.0.0.0:1024` | `web/main.py` |
| PostgreSQL（Rust 侧连接） | `192.168.31.26:5432` | `TCPServer/objsql/src/mysql.rs` |
| PostgreSQL（Web 侧连接） | `127.0.0.1:5432` | `web/main.py` |


