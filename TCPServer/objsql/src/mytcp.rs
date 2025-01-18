use std::{io::Read, net::{TcpListener, TcpStream}};

use crate::mysql::myenvdata;
use crate::mysql::sql_main;






pub fn tcp_main() {
    let listener = TcpListener::bind("0.0.0.0:9000").unwrap();

    for stream in listener.incoming() {
        let stream2 = stream.unwrap();
        processdata(stream2);
    }


}

fn processdata(mut stream : TcpStream)
{
    let mut buffer = [0;520];
    stream.read(&mut buffer).unwrap();

    let data = String::from_utf8_lossy(&buffer).to_string();

    let mut tmp = data.split_whitespace();
    
    let mut gmyenvdata = myenvdata::new(0.0, 0.0);

     if let Some(data1) = tmp.next()
     {
        let f1 = data1.parse::<f64>();
        match f1 {
            Ok(f1) => {
                gmyenvdata.temperature = f1;
            },
            _ => {
                println!("转换失败");
            },
        }
     }

     if let Some(data2) = tmp.next()
     {
        let f2 = data2.parse::<f64>();
        match f2 {
            Ok(f2) => {
                gmyenvdata.humidity = f2;
            },
            _ => {
                println!("转换失败");
            },
        }
     }

     sql_main(gmyenvdata);


    // let numbers: Result<Vec<f64>, _> = data
    //     .split_whitespace()  // 按空格拆分字符串
    //     .map(|s| s.parse::<f64>())  // 尝试将每个部分转换为 f64
    //     .collect();  // 收集结果到一个向量中

    //     match numbers {
    //         Ok(nums) => {
    //             // 成功转换
    //             for num in nums {
    //                 println!("{}", num);
    //             }
    //         }
    //         Err(e) => {
    //             // 转换过程中出现错误
    //             eprintln!("Error parsing string to f64: {}", e);
    //         }
    //     }

    //println!("receive data is {}",data);
}
