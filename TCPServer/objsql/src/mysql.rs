
use tokio_postgres::{NoTls, Error};


pub struct myenvdata {
    pub temperature : f64,
    pub humidity : f64,
}

impl myenvdata {
    pub fn new(temperature : f64,humidity : f64) -> myenvdata {
        myenvdata {
            temperature,
            humidity,
        }
    }
}

// 连接到 PostgreSQL 数据库，并插入数据
async fn insert_data(temperature : f64, humidity : f64) -> Result<(), Error> {
    // 连接到数据库
    let (client, connection) = tokio_postgres::connect("host=192.168.31.26 port=5432 dbname=sqltest user=sqluser password=123456", NoTls)
        .await?;

    // 异步等待连接的建立
    tokio::spawn(async move {
        if let Err(e) = connection.await {
            eprintln!("连接错误: {}", e);
        }
    });

    // 创建一条数据


    //准备 SQL 语句
    let statement = client.prepare("INSERT INTO DongChaZheDATA2 (temperature, humidity) VALUES ($1, $2)").await?;




    //执行 SQL 语句，插入数据
    client.execute(&statement, &[&temperature, &humidity]).await?;

    //println!("成功插入数据");

    Ok(())
}

// 主函数
#[tokio::main]
pub async fn sql_main(envdata : myenvdata) {
    let temperature: f64 = envdata.temperature;
    let humidity : f64 = envdata.humidity;

    
    if let Err(e) = insert_data(temperature,humidity).await {
        eprintln!("错误: {}", e);
    }
}
